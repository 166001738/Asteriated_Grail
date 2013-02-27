#include "JianDi.h"

JianDi::JianDi()
{
    makeConnection();
    setMyRole(this);

    Button *checkCover;
    checkCover = new Button(10,tr("�鿴����"));
    buttonArea->addOutsideTurnButton(checkCover);

    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));
}

void JianDi::JianHunShouHu(int cardID)
{
    state=1901;
    tipArea->setMsg(tr("�Ƿ񷢶������ػ���"));
    shouhuID=cardID;
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void JianDi::JianQiZhan(int targetID)
{
    Player* myself=dataInterface->getMyself();
    state=1902;
    tipArea->setMsg(tr("��ѡ��Ҫ�Ƴ��Ľ�������"));
    int min=myself->getToken(0)<3?myself->getToken(0):3;
    for(;min>0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
    jianqiID=targetID;
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void JianDi::JianQiZhan2()
{
    state=1952;
    tipArea->reset();
    decisionArea->reset();
    playerArea->reset();

    playerArea->enableAll();
    playerArea->disablePlayerItem(jianqiID);
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void JianDi::TianShiZhiHun()
{
    state=1903;
    tipArea->setMsg(tr("�Ƿ񷢶���ʹ֮�ꣿ�緢����ѡ�񽣻꣺"));
    decisionArea->enable(1);
    decisionArea->disable(0);
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
}

void JianDi::EMoZhiHun()
{
    state=1904;
    tipArea->setMsg(tr("�Ƿ񷢶���ħ֮�ꣿ�緢����ѡ�񽣻꣺"));
    decisionArea->enable(1);
    decisionArea->disable(0);
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
}

void JianDi::BuQuYiZhi()
{
    state=1905;
    tipArea->setMsg(tr("�Ƿ񷢶�������־��"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void JianDi::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("�����ػ�"))
        JianHunShouHu(command.split(';').at(3).toInt());
    else if(skill==tr("����ն"))
        JianQiZhan(command.split(';').at(3).toInt());
    else if(skill==tr("��ʹ֮��"))
        TianShiZhiHun();
    else if(skill==tr("��ħ֮��"))
        EMoZhiHun();
    else if(skill==tr("������־"))
        BuQuYiZhi();
}

void JianDi::additionalAction()
{
    Role::additionalAction();
    if(buquUsed)
        tipArea->addBoxItem(tr("1.�����ж���������־��"));
}

void JianDi::coverCardAnalyse()
{
    switch(state)
    {
    case 1903:
    case 1904:
        decisionArea->enable(0);
        break;
    }
}

void JianDi::playerAnalyse()
{
    Role::playerAnalyse();
    switch(state)
    {
    case 1902:
        decisionArea->enable(0);
        break;
    }
}

void JianDi::turnBegin()
{
    Role::turnBegin();
    buquUsed=false;
}

void JianDi::onOkClicked()
{
    Role::onOkClicked();

    QString command;
    QString sourceID;
    QString targetID;
    QString text,cardID;
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    switch(state)
    {
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            buquUsed=false;
            emit sendCommand("1906;"+QString::number(myID)+";");
            attackAction();
        }
        break;
    case 1901:
        command="1901;1;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1902:
        jianqiPoint=tipArea->getBoxCurrentText();
        JianQiZhan2();        
        break;
    case 1952:
        command="1902;1;";
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        text=jianqiPoint;
        command+=text+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1903:
        command="1903;1;";
        cardID = QString::number(selectedCoverCards[0]->getID());
        command += cardID + ";";
        emit sendCommand(command);

        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    case 1904:
        command="1904;1;";
        cardID = QString::number(selectedCoverCards[0]->getID());
        command += cardID + ";";
        emit sendCommand(command);

        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    case 1905:
        command="1905;1;";
        buquUsed=true;
        emit sendCommand(command);
        gui->reset();
        break;
    }
    gui->showCoverArea(false);
}

void JianDi::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 1901:
        command="1901;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1902:
        command="1902;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1952:
        JianQiZhan(jianqiID);
        break;
    case 1903:
        command="1903;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1904:
        command="1904;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1905:
        command="1905;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
    gui->showCoverArea(false);
}
