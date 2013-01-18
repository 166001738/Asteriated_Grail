#include "MoJian.h"

MoJian::MoJian()
{
    makeConnection();

    Button *anYingLiuXing;
    anYingLiuXing=new Button(3,tr("��Ӱ����"));
    buttonArea->addButton(anYingLiuXing);
    connect(anYingLiuXing,SIGNAL(buttonSelected(int)),this,SLOT(AnYingLiuXing()));
}

void MoJian::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    int count=0;
    handArea->disableMagic();
    if(myself->getTap())
        foreach(Card*ptr,dataInterface->getHandCards())
        {
            if(ptr->getType()=="magic")
                count++;
            if(count>=2){
                buttonArea->enable(3);
                break;
            }
        }
    unactionalCheck();
}

void MoJian::AnYingNingJu()
{
    state=903;
    tipArea->setMsg(tr("�Ƿ񷢶���Ӱ���ۣ�"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void MoJian::XiuLuoLianZhan()
{
    //�Ƚ��û���Ķ��⹥���ж�״̬
    state=10;
    onceUsed=true;
    gui->reset();
    handArea->setQuota(1);
    handArea->enableElement("fire");
    handArea->disableMagic();
    playerArea->setQuota(1);
    decisionArea->enable(3);
}

void MoJian::AnYingLiuXing()
{
    state=902;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);
    handArea->enableMagic();
    playerArea->setQuota(1);
    decisionArea->enable(1);
}

void MoJian::HeiAnZhenChan()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶��ڰ������"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void MoJian::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
    case 902:
        playerArea->enableAll();
        break;
    }
}

void MoJian::onOkClicked()
{
    if(state==10&&onceUsed==true)
        state=901;
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID,cardID2;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//�ڰ����ѯ��
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            actions.removeOne(tr("1.������ն"));
            XiuLuoLianZhan();
        }
        break;
//������ն
    case 901:
        command="901;";
        cardID=QString::number(selectedCards[0]->getID());
        targetID=QString::number(selectedPlayers[0]->getID());
        sourceID=QString::number(myID);
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        usedAttack=true;
        usedMagic=usedSpecial=false;
        emit sendCommand(command);
        gui->reset();
        break;
//��Ӱ����
    case 902:
        command="902;";
        cardID=QString::number(selectedCards[0]->getID());
        cardID2=QString::number(selectedCards[1]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+cardID2+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        dataInterface->removeHandCard(selectedCards[1]);
        emit sendCommand(command);
        gui->reset();
        break;
//��Ӱ����
    case 903:
        command="903;1;";
        start=true;
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void MoJian::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//�ڰ����ѯ��
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//�����ж�
    case 1:
//��Ӱ����
    case 902:
        normal();
        break;
//��Ӱ����
    case 903:
        command="903;0;";
        start=false;
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void MoJian::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch (arg[0].toInt())
    {
//Ӧսѯ��
    case 5:
        targetID=arg[3].toInt();
        if(targetID==myID&&isMyTurn)
            handArea->disableMagic();
        break;

//�ж��׶� flag 0-�����ж���1-�����ж���2-�����ж���3-�����ж���4-���������ж�
    case 29:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            if(flag=="0")
                normal();
        }
        break;
//������Ӧѯ��
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            gui->setEnable(1);
            if(flag==tr("��Ӱ����"))
                AnYingNingJu();
            else if(flag==tr("�ڰ����"))
                HeiAnZhenChan();
        }
        break;
//�����ж�ѯ��
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {
            if(usedAttack&&!onceUsed &&!actions.contains(tr("1.������ն"))){
                actions.append(tr("1.������ն"));
                tipArea->showBox();
            }
            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();

            state=42;
        }
        break;
    }
}
