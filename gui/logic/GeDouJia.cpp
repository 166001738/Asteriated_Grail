#include "GeDouJia.h"

GeDouJia::GeDouJia()
{
    makeConnection();
    setMyRole(this);
}

void GeDouJia::CangYanXuLi()
{
    Player* myself=dataInterface->getMyself();
    state=2001;
    decisionArea->enable(0);
    decisionArea->enable(1);
    if(myself->getTap()==0)
        baiShiUsed=false;
    tipArea->setMsg(tr("��ѡ��Ҫ�����ļ��ܣ�"));
    if(myself->getToken(0)<6 && !baiShiUsed)
        tipArea->addBoxItem(tr("1.����һ��"));
    if(myself->getToken(0)<6 && baiShiUsed)
        tipArea->addBoxItem(tr("1.����һ�������˳�����ʽ����ȭ��"));
    if(myself->getToken(0)>0)
        tipArea->addBoxItem(tr("2.����֮��"));
    tipArea->showBox();
}

void GeDouJia::NianDan1()
{
    state=2002;
    tipArea->setMsg(tr("�Ƿ񷢶����"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void GeDouJia::NianDan2()
{
    state=2052;

    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->enableEnemy();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void GeDouJia::BaiShiHuanLongQuan()
{
    state=2003;
    tipArea->setMsg(tr("�Ƿ񷢶���ʽ����ȭ��"));
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
    }
    if(flag)
        decisionArea->enable(0);
}

void GeDouJia::DouShenTianQu()
{
    state=2004;
    tipArea->setMsg(tr("�Ƿ񷢶��������������ǣ���ѡ��Ҫ������"));
    QList<Card*> handcards=dataInterface->getHandCards();
    Player *myself=dataInterface->getMyself();
    cardReady=false;
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
    }
    if(flag)
    {

        if(n<4)
            decisionArea->enable(0);
        else{
            handArea->enableAll();
            handArea->setQuota(n-3);
            cardReady=true;
            }
    }
}

void GeDouJia::cardAnalyse()
{
    tipArea->reset();
    Role::cardAnalyse();
    QList<Card*> handcards=dataInterface->getHandCards();
    int n=handcards.size();
    switch(state)
    {
    case 2004:
        if(cardReady)
            decisionArea->enable(0);
        break;
    }
}

void GeDouJia::playerAnalyse()
{
    Role::playerAnalyse();
    Player*myself = dataInterface->getMyself();
    QList<Card*> selectedCards=handArea->getSelectedCards();
    QList<Player*>selectedPlayers=playerArea->getSelectedPlayers();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    switch(state)
    {
    case 1:
    case 10:
    case 12:
        if(baiShiUsed)
        {
            tipArea->reset();
            if(selectedCards[0]->getType()=="magic")
            {
                tipArea->setMsg(tr("����˳�����ʽ����ȭ�����Ƿ�ȷ����"));
            }
            else if(baiShiFirst)
            {
                baiShiTargetID=selectedPlayers[0]->getID();
            }
            else if(selectedPlayers[0]->getID()!=baiShiTargetID)
            {
                tipArea->setMsg(tr("����˳�����ʽ����ȭ�����Ƿ�ȷ����"));
            }
            decisionArea->enable(0);
        }
        break;
    }
}

void GeDouJia::buy()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::buy();
    else{
        Team*team=dataInterface->getMyTeam();

        int energy=team->getEnergy();

        state=4;
        decisionArea->enable(0);
        decisionArea->enable(1);
        handArea->reset();
        playerArea->reset();
        tipArea->reset();

        if(energy<4)
            tipArea->setMsg(tr("�㽫���˳�����ʽ����ȭ��,ִ�С�����"));
        if(energy==4)
        {
            tipArea->setMsg(tr("�㽫���˳�����ʽ����ȭ��,ִ�С�����"));
            tipArea->addBoxItem(tr("1.һ����ʯ"));
            tipArea->addBoxItem(tr("2.һ��ˮ��"));
            tipArea->showBox();
        }
        if(energy==5)
        {
            tipArea->setMsg(tr("�㽫���˳�����ʽ����ȭ��,ִ�С�����"));
        }
    }
}

void GeDouJia::synthetize()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::synthetize();
    else{
        int gem,crystal;
        Team* team;

        team=dataInterface->getMyTeam();
        gem=team->getGem();
        crystal=team->getCrystal();

        state=5;
        decisionArea->enable(0);
        decisionArea->enable(1);
        tipArea->reset();
        handArea->reset();
        playerArea->reset();

        tipArea->setMsg(tr("�㽫���˳�����ʽ����ȭ��,ִ�С��ϳɡ�"));
        if(crystal>=3)
            tipArea->addBoxItem(tr("1.����ˮ��"));
        if(crystal>=2&&gem>=1)
            tipArea->addBoxItem(tr("2.����ˮ����һ����ʯ"));
        if(crystal>=1&&gem>=2)
            tipArea->addBoxItem(tr("3.һ��ˮ����������ʯ"));
        if(gem>=3)
            tipArea->addBoxItem(tr("4.������ʯ"));
        tipArea->showBox();
    }
}

void GeDouJia::extract()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::extract();
    else{
        int gem,crystal;
        Team* team;
        Player* myself=dataInterface->getPlayerList().at(myID);

        team=dataInterface->getMyTeam();
        gem=team->getGem();
        crystal=team->getCrystal();

        state=6;
        decisionArea->enable(0);
        decisionArea->enable(1);
        tipArea->reset();
        handArea->reset();
        playerArea->reset();

        tipArea->setMsg(tr("�㽫���˳�����ʽ����ȭ��,ִ�С�������"));
        switch(myself->getEnergyMax()-myself->getEnergy())
        {
        case 4:
        case 3:
        case 2:
            if(gem>=2)
                tipArea->addBoxItem(tr("1.������ʯ"));
            if(crystal>=2)
                tipArea->addBoxItem(tr("2.����ˮ��"));
            if(gem>=1&&crystal>=1)
                tipArea->addBoxItem(tr("3.һ����ʯ��һ��ˮ��"));
        case 1:
            if(gem>=1)
                tipArea->addBoxItem(tr("4.һ����ʯ"));
            if(crystal>=1)
                tipArea->addBoxItem(tr("5.һ��ˮ��"));
        }
        tipArea->showBox();
    }
}

void GeDouJia::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;
    QList<Card*>selectedCards;

    QString command;
    QString sourceID;
    QString targetID;
    QString text;
    selectedPlayers=playerArea->getSelectedPlayers();
    selectedCards=handArea->getSelectedCards();

    switch(state)
    {
    case 1:
    case 10:
    case 12:
        if(baiShiUsed)
            baiShiFirst=false;
        break;
    case 2001:
        command="2001;1;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+"0;";
        else
            command+="1;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2002:
        NianDan2();
        break;
    case 2052:
        command="2002;1;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2003:
        command="2003;1;";
        baiShiUsed=true;
        baiShiFirst=true;
        start=true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 2004:
        command="2004;1;";
        start=true;
        command+=QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void GeDouJia::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 2001:
        command="2001;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2002:
        command="2002;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2052:
        NianDan1();
        break;
    case 2003:
        command="2003;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2004:
        command="2004;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void GeDouJia::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("��������"))
        CangYanXuLi();
    else if(skill==tr("�"))
        NianDan1();
    else if(skill==tr("��ʽ����ȭ"))
        BaiShiHuanLongQuan();
    else if(skill==tr("��������"))
        DouShenTianQu();
}

