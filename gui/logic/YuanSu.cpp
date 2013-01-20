#include "YuanSu.h"

YuanSu::YuanSu()
{
    makeConnection();

    Button *yuanSuFaShu,*yuanSuDianRan,*yueGuang;
    yuanSuFaShu=new Button(3,tr("Ԫ�ط���"));
    buttonArea->addButton(yuanSuFaShu);
    connect(yuanSuFaShu,SIGNAL(buttonSelected(int)),this,SLOT(YuanSuFaShu1()));

    yuanSuDianRan=new Button(4,tr("Ԫ�ص�ȼ"));
    buttonArea->addButton(yuanSuDianRan);
    connect(yuanSuDianRan,SIGNAL(buttonSelected(int)),this,SLOT(YuanSuDianRan()));

    yueGuang=new Button(5,tr("�¹�"));
    buttonArea->addButton(yueGuang);
    connect(yueGuang,SIGNAL(buttonSelected(int)),this,SLOT(YueGuang()));
}

void YuanSu::YuanSuFaShu1()
{
    state=1100;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg(tr("����ѡ��ӽϵ�ƺͷ���Ŀ��"));
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableProperty(tr("ӽ"));
    handArea->disableElement("darkness");
    handArea->disableMagic();
}

void YuanSu::YuanSuFaShu2()
{
    state=1101;
    magicCard=handArea->getSelectedCards().takeFirst();
    magicTargetID=playerArea->getSelectedPlayers().takeFirst()->getID();

    playerArea->reset();
    handArea->reset();
    handArea->setQuota(0,1);
    handArea->enableElement(magicCard->getElement());
    foreach(CardItem*ptr, handArea->getHandCardItems())
        if(ptr->getCard()->getID()==magicCard->getID())
        {
            ptr->setEnabled(0);
            ptr->setY(-20);
        }

    if(magicCard->getElement()=="water")
    {
        playerArea->enableAll();        
        decisionArea->disable(0);
        tipArea->setMsg(tr("��ѡ������Ŀ�ꣻ������1��ͬϵ�ƣ������˺�+1"));
    }
    else
        tipArea->setMsg(tr("������1��ͬϵ�ƣ������˺�+1"));
}

void YuanSu::YuanSuDianRan()
{
    state=1102;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YuanSu::YueGuang()
{
    state=1103;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YuanSu::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
//Ԫ�ط���
    foreach(Card*ptr, dataInterface->getHandCards())
        if (ptr->getType()=="attack"&&ptr->getProperty()==tr("ӽ")&&ptr->getElement()!="darkness")
            buttonArea->enable(3);
//Ԫ�ص�ȼ
    if(myself->getToken(0)==3)
        buttonArea->enable(4);
//�¹�
    if(myself->getGem()>0)
        buttonArea->enable(5);
}

void YuanSu::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();
//Ԫ�ط���
    foreach(Card*ptr, dataInterface->getHandCards())
        if (ptr->getType()=="attack"&&ptr->getProperty()==tr("ӽ")&&ptr->getElement()!="darkness")
            buttonArea->enable(3);
//Ԫ�ص�ȼ
    if(myself->getToken(0)==3)
        buttonArea->enable(4);
//�¹�
    if(myself->getGem()>0)
        buttonArea->enable(5);
}

void YuanSu::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
//Ԫ�ط���1
    case 1100:
        playerArea->enableAll();
        break;
    }
}

void YuanSu::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;    
    bool water=false;
    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            magicAction();
            actions.removeOne(tr("1.�����ж�����ʯ��"));
            break;
        case 2:
            magicAction();
            actions.removeOne(tr("2.�����ж���Ԫ�ص�ȼ��"));
            break;
        case 3:
            attackAction();
            actions.removeOne(tr("3.�����ж������У�"));
            break;
        }
        break;
//Ԫ�ط���1
    case 1100:
        YuanSuFaShu2();
        break;
//Ԫ�ط���2
    case 1101:
        command="1101;";
        if(magicCard->getElement()=="wind"){
            command+="1;";
            wind=true;
        }
        else if(magicCard->getElement()=="water"){
            command+="2;";
            water=true;
        }
        else if(magicCard->getElement()=="fire")
            command+="3;";
        else if(magicCard->getElement()=="earth"){
            command+="4;";
            earth=true;
        }
        else
            command+="5;";
        cardID=QString::number(magicCard->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(magicTargetID);
        command+=targetID+";"+sourceID+";";
        if(handArea->getSelectedCards().size()>0){
            command+="2;"+cardID+";"+QString::number(selectedCards[0]->getID())+";";
            dataInterface->removeHandCard(selectedCards[0]);
        }
        else
            command+="1;"+cardID+";";
        if(water)
            command+=QString::number(selectedPlayers[0]->getID())+";";
        dataInterface->removeHandCard(magicCard);
        emit sendCommand(command);
        gui->reset();
        break;
//Ԫ�ص�ȼ
    case 1102:
        command="1102;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        ignite=true;
        emit sendCommand(command);
        gui->reset();
        break;
//�¹�
    case 1103:
        command="1103;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void YuanSu::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
    case 1:
    case 1100:
    case 1102:
    case 1103:
        if(actionFlag==0)
            normal();
        else if(actionFlag==2)
            magicAction();
        break;
    case 1101:
        YuanSuFaShu1();
        break;
    }
}

void YuanSu::decipher(QString command)
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
        if(targetID==myID){
            wind=false;
            earth=false;
            ignite=false;
        }
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
//�����ж�ѯ��
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {
            if(earth){
                actions.append(tr("1.�����ж�����ʯ��"));
                earth=false;
            }
            if(ignite){
                actions.append(tr("2.�����ж���Ԫ�ص�ȼ��"));
                ignite=false;
            }
            if(wind){
                actions.append(tr("3.�����ж������У�"));
                wind=false;
            }

            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();

            state=42;
        }
        break;
    }
}
