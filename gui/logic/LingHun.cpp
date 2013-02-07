#include "LingHun.h"

LingHun::LingHun()
{

    makeConnection();
    setMyRole(this);
    lianJieUsed=false;
    Button *linghunzhaohuan;
    linghunzhaohuan=new Button(3,"����ٻ�");
    buttonArea->addButton(linghunzhaohuan);
    connect(linghunzhaohuan,SIGNAL(buttonSelected(int)),this,SLOT(LingHunZhaoHuan()));

    Button *linghunjingxiang;
    linghunjingxiang=new Button(4,"��꾵��");
    buttonArea->addButton(linghunjingxiang);
    connect(linghunjingxiang,SIGNAL(buttonSelected(int)),this,SLOT(LingHunJingXiang()));

    Button *linghunzhenbao;
    linghunzhenbao=new Button(5,"�����");
    buttonArea->addButton(linghunzhenbao);
    connect(linghunzhenbao,SIGNAL(buttonSelected(int)),this,SLOT(LingHunZhenBao()));

    Button *linghunciyu;
    linghunciyu=new Button(6,"������");
    buttonArea->addButton(linghunciyu);
    connect(linghunciyu,SIGNAL(buttonSelected(int)),this,SLOT(LingHunCiYu()));
}

void LingHun::normal()
{
    Role::normal();
    Player *myself=dataInterface->getMyself();
    if (handArea->checkType("magic"))
        buttonArea->enable(3);
    if(myself->getToken(0)>=2)
        buttonArea->enable(4);
    if (handArea->checkSpecility(tr("�����")) && myself->getToken(0)>=3)
        buttonArea->enable(5);
    if (handArea->checkSpecility(tr("������")) && myself->getToken(1)>=3)
        buttonArea->enable(6);
    unactionalCheck();
}

void LingHun::LingHunZhaoHuan()
{
    state=2201;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableMagic();
    handArea->setQuota(1,7);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void LingHun::LingHunJingXiang()
{
    state=2202;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);

    if(handArea->getHandCardItems().size()>3)
    {
        handArea->enableAll();
        handArea->setQuota(3);
    }
    else
        playerArea->enableAll();
    playerArea->setQuota(1);
}

void LingHun::LingHunZhenBao()
{
    state=2203;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    handArea->enableSpecility(tr("�����"));
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void LingHun::LingHunCiYu()
{
    state=2204;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    handArea->enableSpecility(tr("������"));
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void LingHun::LingHunZhuanHuan()
{
    state=2205;
    Player *myself=dataInterface->getMyself();
    tipArea->setMsg(tr("��ѡ��Ҫת������꣺"));
    decisionArea->enable(0);
    decisionArea->enable(1);

    if(myself->getToken(0)>0)
        tipArea->addBoxItem(tr("1.���ƻ�ת��Ϊ����"));
    if(myself->getToken(1)>0)
        tipArea->addBoxItem(tr("2.������ת��Ϊ�ƻ�"));
    tipArea->showBox();
}

void LingHun::LingHunLianJie(int harmPoint)
{
    state=2206;
    Player *myself=dataInterface->getMyself();
    decisionArea->enable(1);
    decisionArea->disable(0);
    if(!lianJieUsed){
    tipArea->setMsg(tr("�Ƿ񷢶�������ӣ�������ѡ��Ŀ��"));
    playerArea->enableMate();
    playerArea->setQuota(1);
    }
    else
    {
        decisionArea->enable(0);
        tipArea->setMsg(tr("��ѡ��Ҫת�Ƶ��˺���"));
        int min=myself->getToken(1)<harmPoint?myself->getToken(1):harmPoint;
        for(;min>=0;min--)
            tipArea->addBoxItem(QString::number(min));
        tipArea->showBox();
    }
}

void LingHun::LingHunZengFu()
{
    state=2207;
    tipArea->setMsg(tr("�Ƿ񷢶����������"));
    decisionArea->enable(1);
    decisionArea->enable(0);
}

void LingHun::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case 2201:
        decisionArea->enable(0);
        break;
    case 2203:
    case 2204:
    case 2202:
        playerArea->enableAll();
        break;
    }
}

void LingHun::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;
    int flag;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    case 2201:
        command="2201;";
        sourceID=QString::number(myID);
        command+=sourceID+";"+QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+":";
            dataInterface->removeHandCard(ptr);
        }
        command+=";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2202:
        command="2202;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        if(handArea->getHandCardItems().size()<=3)
        {
            command+="0";
            foreach(Card*ptr,dataInterface->getHandCards())
                dataInterface->removeHandCard(ptr);
        }
        else
        {
            command+=QString::number(selectedCards.size())+";";
            foreach(Card*ptr,selectedCards){
                command+=QString::number(ptr->getID())+":";
                dataInterface->removeHandCard(ptr);
            }
            command+=";";
        }
        emit sendCommand(command);
        gui->reset();
        break;
    case 2203:
        command="2203;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        cardID=QString::number(selectedCards[0]->getID());
        dataInterface->removeHandCard(selectedCards[0]);
        command+=targetID+";"+sourceID+";"+cardID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2204:
        command="2204;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        cardID=QString::number(selectedCards[0]->getID());
        dataInterface->removeHandCard(selectedCards[0]);
        command+=targetID+";"+sourceID+";"+cardID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2205:
        command="2205;1;";
        text=tipArea->getBoxCurrentText();
        if(text[0].digitValue()==1)
            command+="0;";
        else
            command+="1;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2206:
        command="2206;1;";
        if(!lianJieUsed)
        {
            targetID=QString::number(selectedPlayers[0]->getID());
            start=true;
            lianJieUsed=true;
            command+=targetID+";";
        }
        else
        {
            text=tipArea->getBoxCurrentText();
            command+=text+";";
        }
        emit sendCommand(command);
        gui->reset();
        break;
    case 2207:
        command="2207;1;";
        start=true;
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void LingHun::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 2201:
    case 2202:
    case 2203:
    case 2204:
        normal();
        break;
    case 2205:
        command="2205;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2206:
        command="2206;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2207:
        command="2207;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void LingHun::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("���ת��"))
        LingHunZhuanHuan();
    else if(skill==tr("�������"))
        LingHunLianJie(command.split(';').at(3).toInt());
    else if(skill==tr("�������"))
        LingHunZengFu();
}


