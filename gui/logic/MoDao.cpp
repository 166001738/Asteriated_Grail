#include "MoDao.h"

MoDao::MoDao()
{
    makeConnection();
    setMyRole(this);

    Button *moDanRongHe,*moBaoChongJi,*huiMeiFengBao;
    moDanRongHe=new Button(3,tr("ħ���ں�"));
    buttonArea->addButton(moDanRongHe);
    connect(moDanRongHe,SIGNAL(buttonSelected(int)),this,SLOT(MoDanRongHe()));

    moBaoChongJi=new Button(4,tr("ħ�����"));
    buttonArea->addButton(moBaoChongJi);
    connect(moBaoChongJi,SIGNAL(buttonSelected(int)),this,SLOT(MoBaoChongJi()));

    huiMeiFengBao=new Button(5,tr("����籩"));
    buttonArea->addButton(huiMeiFengBao);
    connect(huiMeiFengBao,SIGNAL(buttonSelected(int)),this,SLOT(HuiMeiFengBao()));

    Player*myself=dataInterface->getMyself();
    myID=myself->getID();

    int myColor=myself->getColor();

    QString queue=dataInterface->getQueue();
    int playerMax=queue.count()/2;
    QList<Player*>playerList=dataInterface->getPlayerList();
    int i;
    //find myPos
    for(i=0;i<playerMax;i++)
        if(queue[i].digitValue()==myID)
            break;
    int ptr;
    do
    {
        i--;
        if(i==-1)
            i=playerMax-1;
        ptr=queue[i].digitValue();
    }while(playerList[ptr]->getColor()==myColor);
    nextClockwise=ptr;
}

void MoDao::MoDanRongHe()
{
    if(firstMoDan&&isMyTurn)
        state=801;
    else
        state=802;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("earth");
    handArea->enableElement("fire");
}

void MoDao::MoBaoChongJi()
{
    state=803;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableMagic();
}

void MoDao::HuiMeiFengBao()
{
    state=804;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);

    decisionArea->enable(1);
    decisionArea->disable(0);

    playerArea->enableEnemy();
}

void MoDao::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
//ħ���ں�
    if (handArea->checkElement("earth")||handArea->checkElement("fire"))
        buttonArea->enable(3);
//ħ�����
    if (handArea->checkType("magic"))
        buttonArea->enable(4);
//����籩
    if(myself->getGem()>0)
        buttonArea->enable(5);
    unactionalCheck();
}
void MoDao::turnBegin()
{
    Role::turnBegin();
    firstMoDan=true;
}

void MoDao::moDaned(int nextID,int sourceID, int howMany)
{
    moDanHarm=howMany;
    Role::moDaned(nextID,sourceID,howMany);
    firstMoDan=false;

    if (handArea->checkElement("earth")||handArea->checkElement("fire"))
        buttonArea->enable(3);
}

void MoDao::cardAnalyse()
{
    Role::cardAnalyse();

    QList<Card*> selectedCards=handArea->getSelectedCards();
    QString cardName;

    switch (state)
    {
//normal action
    case 1:
        cardName=selectedCards[0]->getName();
        if(cardName==tr("ħ��"))
            playerArea->enablePlayerItem(nextClockwise);//�����ټ���һ���ϼҵ���
        break;
//ħ���ں�(�״�ħ����
    case 801:
        playerArea->enablePlayerItem(nextClockwise);
        playerArea->enablePlayerItem(nextCounterClockwise);
        break;
//ħ���ں�(���״Σ�
    case 802:
        playerArea->enablePlayerItem(moDanNextID);
        break;
//ħ�����
    case 803:
        playerArea->enableEnemy();
        break;
    }
}

void MoDao::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString targetID2;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//ħ���ں�(�غ��ڣ�
    case 801:        
        command="801;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//ħ���ں�(�غ��⣩
    case 802:
        command="802;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//ħ�����
    case 803:
        command="803;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        targetID2=QString::number(selectedPlayers[1]->getID());
        command+=cardID+";"+targetID+";"+targetID2+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//����籩
    case 804:
        command="804;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        targetID2=QString::number(selectedPlayers[1]->getID());
        command+=targetID+";"+targetID2+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
    firstMoDan=false;
}

void MoDao::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
//ħ���ں�(�غ��ڣ�
    case 801:
//ħ�����
    case 803:
//����籩
    case 804:
        normal();
        break;
//ħ���ں�(�غ��⣩
    case 802:
        moDaned(moDanNextID,sourceID,moDanHarm);
        break;
    }
}

