#include "ShenGuan.h"
#include <QApplication>

ShenGuan::ShenGuan()
{
    makeConnection();
    setMyRole(this);
    Button *shenShengQiFu, *shuiZhiShenLi, *shenShengLingYu;
    shenShengQiFu = new Button(3,tr("��ʥ����"));
    buttonArea->addButton(shenShengQiFu);
    connect(shenShengQiFu,SIGNAL(buttonSelected(int)),this,SLOT(ShenShengQiFu()));

    shuiZhiShenLi = new Button(4,tr("ˮ֮����"));
    buttonArea->addButton(shuiZhiShenLi);
    connect(shuiZhiShenLi,SIGNAL(buttonSelected(int)),this,SLOT(ShuiZhiShenLi1()));

    shenShengLingYu = new Button(5,tr("��ʥ����"));
    buttonArea->addButton(shenShengLingYu);
    connect(shenShengLingYu,SIGNAL(buttonSelected(int)),this,SLOT(ShenShengLingYu1()));
}

void ShenGuan::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    int qiFu = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getType() == tr("magic"))
            qiFu++;
    }
    if(qiFu>1)
        buttonArea->enable(3);
    if(handArea->checkElement("water"))
        buttonArea->enable(4);
    if(myself->getEnergy()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void ShenGuan::ShenShengQiShi()
{
    state = 36;
    tipArea->setMsg(tr("�Ƿ񷢶���ʥ��ʾ��"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void ShenGuan::ShenShengQiFu()
{
    state = 1502;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableMagic();
}

void ShenGuan::ShuiZhiShenLi1()
{
    state = 1531;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}

void ShenGuan::ShuiZhiShenLi2()
{
    state = 1532;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg("���Ŀ���ɫһ����");

    handArea->setQuota(1);
    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();
}

void ShenGuan::ShenShengQiYue1()
{
    state = 1541;
    tipArea->setMsg(tr("�Ƿ񷢶���ʥ��Լ"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag = false;
    switch(handcards.size())
    {
    case 1:
        if(handcards[0]->getElement() != "light")
            flag = true;
        break;
    case 0:
        if(dataInterface->getMyself()->getEnergy()>1)
            flag = true;
        break;
    default:
        flag = true;
    }
    if(flag)
        decisionArea->enable(0);
    decisionArea->enable(1);
}

void ShenGuan::ShenShengQiYue2()
{
    state = 1542;
    Player* myself=dataInterface->getMyself();
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    int cross = myself->getCrossNum();
    if (cross>4)
        cross = 4;
    for(;cross>0;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->setMsg("��ѡ��Ҫת�Ƶ�������Ŀ");
    tipArea->showBox();

    playerArea->setQuota(1);
    playerArea->enableMate();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShenGuan::ShenShengLingYu1()
{
    state = 1551;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(tr("����ѡ��һ�"));
    if(dataInterface->getMyself()->getCrossNum()>0)
        tipArea->addBoxItem(tr("1.���Ƴ�1���ƣ���Ŀ���ɫ���2�㷨���˺�"));
    tipArea->addBoxItem(tr("2.����2���ƣ�Ŀ���������1����"));
    tipArea->showBox();
}

void ShenGuan::ShenShengLingYu2()
{
    state = 1552;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    QList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>1)
        handArea->setQuota(2);
    else if(handcards.size()==1)
        handArea->setQuota(1);
    else
    {
        if(lingYu==1)
            playerArea->enableAll();
        else
            playerArea->enableMate();
    }

    handArea->enableAll();
}

void ShenGuan::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case 1502:
        decisionArea->enable(0);
        break;
    case 1531:
        playerArea->enableMate();
        break;
    case 1532:
        decisionArea->enable(0);
        break;
    case 1552:
        if(lingYu==1)
            playerArea->enableAll();
        else
            playerArea->enableMate();
        break;
    }
}

void ShenGuan::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    static QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    case 1502:
        command = "1502;";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        command+=QString::number(myID);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1531:
        command = "1531;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+cardID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1532:
        command = "1532;";
        cardID=QString::number(selectedCards[0]->getID());
        command+=cardID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1541:
        command = "1504;1;";
        ShenShengQiYue2();
        break;
    case 1542:
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        text=tipArea->getBoxCurrentText();
        command += targetID+";"+text+";"+sourceID+";";
        start = true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 1551:
        if(tipArea->getBoxCurrentText()[0]=='1')
            lingYu = 1;
        else
            lingYu = 2;
        ShenShengLingYu2();
        break;
    case 1552:
        command = "1505;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=QString::number(lingYu)+";"+targetID+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        for(int i=0;i<(2-selectedCards.size());i++)
            command+="-1;";
        command+=sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void ShenGuan::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 1502:
    case 1531:
    case 1551:
    case 1552:
        normal();
        break;
    case 1541:
        command = "1504;0;0;0;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1542:
        gui->reset();
        ShenShengQiYue1();
        break;
    }
}

void ShenGuan::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("��ʥ��ʾ"))
        ShenShengQiShi();
    else if(skill==tr("��ʥ��Լ"))
        ShenShengQiYue1();
    else if(skill==tr("ˮ֮��������"))
        ShuiZhiShenLi2();
}

void ShenGuan::cure(int cross,int harmPoint, int type)
{
    int min=cross<harmPoint?cross:harmPoint;
    if(min>1)
        min = 1;
    QString msg=tr("���ܵ�")+QString::number(harmPoint)+tr("��");
    if(type==1)
        msg+=tr("����");
    else
        msg+=tr("����");
    msg+=tr("�˺���Ҫʹ�ö��ٸ����Ƶ���֮��");

    state=9;
    decisionArea->enable(0);
    decisionArea->enable(1);

    tipArea->setMsg(msg);
    for(;min>=0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
    QApplication::alert((QWidget*)playerArea->window());
}