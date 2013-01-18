#include "MoDao.h"

MoDao::MoDao()
{
    makeConnection();

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
    if(firstMoDan)
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
    handArea->setQuota(1);

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

void MoDao::moDaned(int nextID)
{
    Role::moDaned(nextID);
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
    QString text;

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
}

void MoDao::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
//�����ж�
    case 1:
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
        MoDao::moDaned(moDanNextID);
        break;
    }
}

void MoDao::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch (arg[0].toInt())
    {
//�غϿ�ʼ
    case 3:
        targetID=arg[1].toInt();
        if(targetID!=myID)
            firstMoDan=false;
        else
            firstMoDan=true;
        break;
//ħ��ѯ��
    case 26:
        int nextID;
        targetID=arg[1].toInt();
        nextID=arg[4].toInt();
        if(targetID==myID)
            moDaned(nextID);
        break;
//�ж��׶� flag 0-�����ж���1-�����ж���2-�����ж���3-�����ж���4-���������ж�
    case 29:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID && flag=="0")
            normal();
        break;

    }
}

