#include "ZhongCai.h"

ZhongCai::ZhongCai()
{
    makeConnection();
setMyRole(this);

    Button *moRiShenPan;
    moRiShenPan=new Button(3,tr("ĩ������"));
    buttonArea->addButton(moRiShenPan);
    connect(moRiShenPan,SIGNAL(buttonSelected(int)),this,SLOT(MoRiShenPan()));

    Button *panJueTianPing;
    panJueTianPing=new Button(4,tr("�о���ƽ"));
    buttonArea->addButton(panJueTianPing);
    connect(panJueTianPing,SIGNAL(buttonSelected(int)),this,SLOT(PanJueTianPing()));
}

void ZhongCai::normal()
{
    Role::normal();
    Player *myself=dataInterface->getMyself();
    //ĩ������
    if(myself->getToken(0)>0)
        buttonArea->enable(3);
    //�о���ƽ
    if(myself->getEnergy()>0)
        buttonArea->enable(4);
    //ǿ��ĩ��
    if(myself->getToken(0)==4)
    {
        handArea->disableAll();
        buttonArea->disable(0);
        buttonArea->disable(1);
        buttonArea->disable(2);
        buttonArea->disable(4);
    }
    unactionalCheck();
}

void ZhongCai::YiShiZhongDuan()
{
    state=1401;
    gui->reset();
    tipArea->setMsg(tr("�Ƿ񷢶���ʽ�жϣ�"));
    QList<Card*> handcards=dataInterface->getHandCards();
    Player *myself=dataInterface->getMyself();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
        if(myself->getToken(0)>0 || myself->getEnergy()>0)
            flag=true;
    }
    if(flag)
        decisionArea->enable(0);
}

void ZhongCai::ZhongCaiYiShi()
{
    state=1402;
    gui->reset();
    tipArea->setMsg(tr("�Ƿ񷢶��ٲ���ʽ��"));
    QList<Card*> handcards=dataInterface->getHandCards();
    Player *myself=dataInterface->getMyself();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
        if(myself->getToken(0)>0 || myself->getEnergy()>0)
            flag=true;
    }
    if(flag)
        decisionArea->enable(0);
}

void ZhongCai::MoRiShenPan()
{
    state=1403;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ZhongCai::PanJueTianPing()
{
    state=1404;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    decisionArea->enable(0);
    decisionArea->enable(1);

    tipArea->setMsg(tr("��ѡ��һ�"));
    tipArea->addBoxItem(tr("1.���������������"));
    tipArea->addBoxItem(tr("2.�����Ʋ������ޣ���ս������+1����ʯ��"));
    tipArea->showBox();
}

void ZhongCai::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;

    QString command;
    QString sourceID;
    QString targetID;
    QString text;

    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    case 1401:
        command="1401;1;";
        start=true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 1402:
        command="1402;1;";
        start=true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 1403:
        command="1403;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1404:
        command="1404;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            command+="0;";
            foreach(Card*ptr,dataInterface->getHandCards())
                dataInterface->removeHandCard(ptr);
        }
        else
            command+="1;";
        command+=QString::number(myID)+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void ZhongCai::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    //ĩ������
    case 1403:
    //�о���ƽ
    case 1404:
        normal();
        break;
    //��ʽ�ж�
    case 1401:
        command="1401;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    //�ٲ���ʽ
    case 1402:
        command="1402;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}
void ZhongCai::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("��ʽ�ж�"))
        YiShiZhongDuan();
    else if(skill==tr("�ٲ���ʽ"))
        ZhongCaiYiShi();
}
