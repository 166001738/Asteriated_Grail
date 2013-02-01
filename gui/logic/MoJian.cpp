#include "MoJian.h"

MoJian::MoJian()
{
    makeConnection();
    setMyRole(this);

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
//�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            emit sendCommand("901;"+QString::number(myID)+";");            
            XiuLuoLianZhan();
        }
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
void MoJian::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("��Ӱ����"))
        AnYingNingJu();
    else if(skill==tr("�ڰ����"))
        HeiAnZhenChan();
}

void MoJian::additionalAction()
{
    Role::additionalAction();
    if(usedAttack&&!onceUsed)
        tipArea->addBoxItem(tr("1.������ն"));
}

void MoJian::attacked(QString element, int hitRate)
{
    Role::attacked(element,hitRate);
    if(isMyTurn)
        handArea->disableMagic();
}
