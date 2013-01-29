#include "Role.h"
#include <QStringList>
#include <QApplication>
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include "logic/Logic.h"

Role::Role(QObject *parent) :
    QObject(parent)
{
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
        i++;
        if(i==playerMax)
            i=0;
        ptr=queue[i].digitValue();        

    }while(playerList[ptr]->getColor()==myColor);
    nextCounterClockwise=ptr;
    handArea=gui->getHandArea();
    playerArea=gui->getPlayerArea();
    buttonArea=gui->getButtonArea();
    decisionArea=gui->getDecisionArea();
    tipArea=gui->getTipArea();
    teamArea=gui->getTeamArea();

}
void Role::makeConnection()
{
    connect(logic->getClient(),SIGNAL(getMessage(QString)),this,SLOT(decipher(QString)));
    connect(this,SIGNAL(sendCommand(QString)),logic->getClient(),SLOT(sendMessage(QString)));
    connect(decisionArea,SIGNAL(okClicked()),this,SLOT(onOkClicked()));
    connect(decisionArea,SIGNAL(cancelClicked()),this,SLOT(onCancelClicked()));
    connect(decisionArea,SIGNAL(exchangeClicked()),this,SLOT(exchangeCards()));
    connect(decisionArea,SIGNAL(resignClicked()),this,SLOT(resign()));
    connect(buttonArea->getButtons().at(0),SIGNAL(buttonSelected(int)),this,SLOT(buy()));
    connect(buttonArea->getButtons().at(1),SIGNAL(buttonSelected(int)),this,SLOT(synthetize()));
    connect(buttonArea->getButtons().at(2),SIGNAL(buttonSelected(int)),this,SLOT(extract()));
    connect(buttonArea,SIGNAL(buttonUnselected()),this,SLOT(onCancelClicked()));
    connect(handArea,SIGNAL(cardReady()),this,SLOT(cardAnalyse()));
    connect(playerArea,SIGNAL(playerReady()),this,SLOT(playerAnalyse()));
}

void Role::cardAnalyse()
{
    int i;
    QList<Card*> selectedCards=handArea->getSelectedCards();
    Player* myself=dataInterface->getMyself();
    QString cardName;
    switch (state)
    {
//normal action
    case 1:
//׷�ӹ��������ж�
    case 11:
    case 12:
        if(selectedCards[0]->getType()=="magic" )
        {
            cardName=selectedCards[0]->getName();
            if(cardName==tr("�ж�"))
                playerArea->enableAll();
            if(cardName==tr("����"))
            {
                playerArea->reset();
                for(i=0;i<dataInterface->getPlayerMax();i++)
                    if(!dataInterface->getPlayerList().at(i)->checkStatus(1))
                        playerArea->enablePlayerItem(i);
            }
            if(cardName==tr("ʥ��"))
            {
                playerArea->reset();
                for(i=0;i<dataInterface->getPlayerMax();i++)
                    if(!dataInterface->getPlayerList().at(i)->checkStatus(2))
                        playerArea->enablePlayerItem(i);
            }
            if(cardName==tr("ħ��"))
            {
                playerArea->reset();
                playerArea->enablePlayerItem(nextCounterClockwise);
            }
        }
//        if(myself->getID()==14&&myself->getToken(0)==4)
//            handArea->disableAll();
        else
    case 10:
        {
            playerArea->reset();
            playerArea->enableEnemy();
            QList<Player*> players=dataInterface->getPlayerList();
            for(i=0;i<players.size();i++)
                if(players[i]->getRoleID()==5 && players[i]->getTap()==1){
                    playerArea->disablePlayerItem(i);
                    break;
                }
            if(myself->getSpecial(1) == 1)
            {
                playerArea->disableAll();
                for(i=0;i<players.size();i++)
                    if(players[i]->getRoleID()==21){
                        playerArea->enablePlayerItem(i);
                        break;
                    }
            }
        }
    break;
//attacked reply
    case 2:
        if(selectedCards[0]->getType()=="attack" )
        {
            playerArea->reset();
            playerArea->enableEnemy();
            playerArea->disablePlayerItem(sourceID);
        }
        if(selectedCards[0]->getElement()=="light")
        {
            playerArea->reset();
            decisionArea->enable(0);
        }
    break;
//drop cards
    case 3:
//��ʹף������
    case 751:
//ħ���������
    case 851:
        decisionArea->enable(0);
    break;
//ħ��reply
    case 8:
        cardName=selectedCards[0]->getName();
        if(cardName==tr("ħ��"))
        {
            playerArea->reset();
            playerArea->enablePlayerItem(moDanNextID);
        }
        else if(cardName==tr("ʥ��"))
        {
            playerArea->reset();
            decisionArea->enable(0);
        }
        break;
    }
}

void Role::playerAnalyse()
{
    decisionArea->enable(0);
}

void Role::exchangeCards()
{
    QString command="30;";
    QList<Card*> handcards=dataInterface->getHandCards();
    int i;
    int n=handcards.count();
    for(i=0;i<n;i++)
        dataInterface->removeHandCard(handcards[i]);
    emit sendCommand(command);
}

void Role::resign()
{
    emit sendCommand("4;-1");
    gui->reset();
}

//�����ж�
void Role::normal()
{
    gui->reset();

    QList<Card*> handcards=dataInterface->getHandCards();
    Player* myself=dataInterface->getMyself();
    Team* myTeam=dataInterface->getMyTeam();
    int n=handcards.count();

    state=1;
    actionFlag=0;
    playerArea->setQuota(1);
    handArea->setQuota(1);

    handArea->enableAll();
    handArea->disableElement("light");

    if(n+3 <= myself->getHandCardMax() && start==false)
    {
//����
        buttonArea->enable(0);
        if(myTeam->getEnergy()>=3)
//�ϳ�
            buttonArea->enable(1);
    }
//����
    if(myself->getEnergy()<myself->getEnergyMax() && myTeam->getEnergy()>0 && start==false)
        buttonArea->enable(2);
    tipArea->setMsg(tr("�ֵ�����"));
    unactionalCheck();
}

//�޷��ж����
void Role::unactionalCheck()
{
    bool actional=false;
    QList<Button*>buttons=buttonArea->getButtons();
    QList<CardItem*>cardItems=handArea->getHandCardItems();
    for(int i=0;i<buttons.size();i++)
        if(buttons[i]->isEnabled()&&i!=2)
        {
            actional=true;
            break;
        }
    for(int i=0;i<cardItems.size()&&!actional;i++)
        if(cardItems[i]->isEnabled())
        {
            actional=true;
            break;
        }
    if(!actional)
        decisionArea->enable(2);
    else
        decisionArea->disable(2);
}

void Role::attackAction()
{
    gui->reset();

    state=10;
    actionFlag=1;
    playerArea->setQuota(1);
    handArea->setQuota(1);
    handArea->enableAttack();
    decisionArea->enable(3);
}

void Role::magicAction()
{
    gui->reset();

    state=11;
    actionFlag=2;
    playerArea->setQuota(1);
    handArea->setQuota(1);
    handArea->enableMagic();
    handArea->disableElement("light");
    decisionArea->enable(3);

}

void Role::attackOrMagic()
{
    gui->reset();

    state=12;
    actionFlag=4;
    playerArea->setQuota(1);
    handArea->setQuota(1);
    handArea->enableAll();
    handArea->disableElement("light");
    decisionArea->enable(3);
}

void Role::attacked(QString element,int hitRate)
{
    state=2;
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    if(hitRate==0)
    {
        handArea->enableElement(element);
        handArea->enableElement("darkness");
    }
    handArea->disableMagic();
    handArea->enableElement("light");
}

void Role::drop(int howMany)
{
    state=3;
    handArea->setQuota(howMany);
    handArea->enableAll();
}

void Role::buy()
{
    Team*team=dataInterface->getMyTeam();

    int energy=team->getEnergy();

    state=4;
    decisionArea->enable(0);
    decisionArea->enable(1);
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    if(energy<4)
        tipArea->setMsg(tr("����3���ƣ��㷽ս������һ��ʯһˮ��"));
    if(energy==4)
    {
        tipArea->setMsg(tr("ս������ʯ����4������ѡ��Ҫ�������ʯ��"));
        tipArea->addBoxItem(tr("1.һ����ʯ"));
        tipArea->addBoxItem(tr("2.һ��ˮ��"));
        tipArea->showBox();
    }
    if(energy==5)
    {
        tipArea->setMsg(tr("ս������ʯ��Ŀ�Ѵ����ޣ����򽫲���������ʯ"));
    }
}

void Role::synthetize()
{
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

    tipArea->setMsg(tr("��ѡ�������ϳɵ���ʯ��"));
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

void Role::extract()
{
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

    tipArea->setMsg(tr("��ѡ��Ҫ��ȡ����ʯ����"));
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

void Role::moDaned(int nextID,int sourceID,int howMany)
{
    state=8;
    gui->reset();
    QString msg=dataInterface->getPlayerList().at(sourceID)->getName()+tr("��")+tr("��")+tr("ʹ����ħ����Ŀǰ�˺�Ϊ��")+QString::number(howMany)+tr("��");
    tipArea->setMsg(msg);
    playerArea->setQuota(1);
    handArea->setQuota(1);

    handArea->enableElement("light");
    handArea->enableMoDan();
    decisionArea->enable(1);
    moDanNextID=nextID;
}

void Role::cure(int cross,int harmPoint, int type)
{
    int min=cross<harmPoint?cross:harmPoint;
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
}

void Role::turnBegin()
{
    isMyTurn=true;
    onceUsed=false;
    start=false;
    usedAttack=usedMagic=usedSpecial=false;
    QApplication::alert((QWidget*)playerArea->window());
}

void Role::additionalAction(){
    gui->reset();
    tipArea->setMsg(tr("�Ƿ�ִ�ж����ж���"));
    if(dataInterface->getMyself()->checkStatus(5))
        gui->getTipArea()->addBoxItem("0.Ѹ�ݴ͸�");
    state=42;
    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->enable(3);
}

void Role::askForSkill(QString skill)
{
    if(skill==tr("�����͸�"))
        WeiLi();
}

void Role::TianShiZhuFu(int n)
{
    gui->reset();
    state=751;
    tipArea->setMsg(tr("������ʹ")+QString::number(n)+tr("����"));
    handArea->setQuota(n);
    handArea->enableAll();
}

void Role::MoBaoChongJi()
{
    gui->reset();
    state=851;
    tipArea->setMsg(tr("��һ�ŷ����ƻ��ܵ����㷨���˺�"));
    handArea->setQuota(1);
    handArea->enableMagic();
    decisionArea->enable(1);
}

void Role::WeiLi()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶������͸���"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void Role::onCancelClicked()
{
    QString command;
    switch(state)
    {
//normal
    case 1:
    case 4:
    case 5:
    case 6:
        gui->reset();
        myRole->normal();
        break;
//ATTACKEDREPLY
    case 2:
        command="6;2;;;"+QString::number(myID)+";";
        emit sendCommand(command);
        gui->reset();
        break;
//����
    case 7:
        command="23;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//ħ��Ӧ��
    case 8:
        command="27;2;;;";
        emit sendCommand(command);
        gui->reset();
        break;
//����Ӧ��
    case 9:
        command="34;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//�򵥵ļ��ܷ���ѯ��
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//ħ���������
    case 851:
        command="851;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void Role::onOkClicked()
{
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;
    int i,boxCurrentIndex;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//NORMALACTION
    case 1:
//׷���ж�
    case 10:
    case 11:
    case 12:
        cardID=QString::number(selectedCards[0]->getID());
        targetID=QString::number(selectedPlayers[0]->getID());
        sourceID=QString::number(myID);
        if(selectedCards[0]->getType()=="attack"){
            command="4;1;"+cardID+";"+targetID+";"+sourceID+";";
            usedAttack=true;
            usedMagic=usedSpecial=false;
        }
        else
        {
            QString cardName;
            cardName=selectedCards[0]->getName();
            if(cardName==tr("�ж�")||cardName==tr("����")||cardName==tr("ʥ��")||cardName==tr("ħ��"))
                command="4;2;0;"+cardID+";"+targetID+";"+sourceID+";";
            usedMagic=true;
            usedAttack=usedSpecial=false;
        }
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
//ATTACKEDREPLY
    case 2:
        command="6;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        if(selectedCards[0]->getType()=="attack")
        {
            targetID=QString::number(selectedPlayers[0]->getID());
            command+="0;"+cardID+";"+targetID+";"+sourceID+";";
        }
        else if(selectedCards[0]->getElement()=="light")
            command+="1;"+cardID+";;"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
//DROPREPLY
    case 3:
        cardID=QString::number(selectedCards[0]->getID());
        command="8;"+cardID;
        for(i=1;i<selectedCards.count();i++)
        {
            cardID=QString::number(selectedCards[i]->getID());
            command+=","+cardID;
        }
        command+=";";
        for(i=0;i<selectedCards.count();i++)
        {
            dataInterface->removeHandCard(selectedCards[i]);
        }
        gui->reset();
        emit sendCommand(command);
        break;
//BUY
    case 4:
        command="4;3;0;";
        boxCurrentIndex=tipArea->getBoxCurrentIndex();

        switch(boxCurrentIndex)
        {
        case -1:
            if(dataInterface->getMyTeam()->getEnergy()<4)
                command+="1;1;";
            else
                command+="0;0;";
            break;
        case 0:
            command+="1;0;";
            break;
        case 1:
            command+="0;1;";
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(command);
        break;
//SYNTHETIZE
    case 5:
        command="4;3;1;";
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            command+="0;3;";
            break;
        case 2:
            command+="1;2;";
            break;
        case 3:
            command+="2;1;";
            break;
        case 4:
            command+="3;0;";
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(command);
        break;
//EXTRACT
    case 6:
        command="4;3;2;";
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            command+="2;0;";
            break;
        case 2:
            command+="0;2;";
            break;
        case 3:
            command+="1;1;";
            break;
        case 4:
            command+="1;0;";
            break;
        case 5:
            command+="0;1;";
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(command);
        break;
//����
    case 7:
        command="23;0;";
        gui->reset();
        emit sendCommand(command);
        break;
//ħ��Ӧ��
    case 8:
        if(selectedCards[0]->getName()==tr("ʥ��"))
        {
            cardID=QString::number(selectedCards[0]->getID());
            command="27;1;"+cardID+";;";
        }
        else if(selectedCards[0]->getName()==tr("ħ��"))
        {
            cardID=QString::number(selectedCards[0]->getID());
            command="27;0;"+cardID+";";
            targetID=QString::number(selectedPlayers[0]->getID());
            command+=targetID+";";
        }
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
//����Ӧ��
    case 9:
        text=tipArea->getBoxCurrentText();
        command="34;"+text+";";
        gui->reset();
        emit sendCommand(command);
        break;
//�򵥵ļ��ܷ���ѯ��
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//�����ж���Ѹ�ݣ�
    case 42:
        if(tipArea->getBoxCurrentText().at(0).digitValue()==0)
        {
            emit sendCommand("1607;"+QString::number(myID)+";");
            myRole->attackAction();
        }
        break;
//��ʹף��
    case 751:
        command="751;"+QString::number(selectedCards.size())+";";
        cardID=QString::number(selectedCards[0]->getID());
        dataInterface->removeHandCard(selectedCards[0]);
        command+=cardID+";";
        if(selectedCards.size()==2)
        {
            cardID=QString::number(selectedCards[1]->getID());
            command+=cardID+";";
            dataInterface->removeHandCard(selectedCards[1]);
        }
        gui->reset();
        emit sendCommand(command);
        break;
//ħ�����
    case 851:
        cardID=QString::number(selectedCards[0]->getID());
        command="851;1;"+cardID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
    }
}

void Role::decipher(QString command)
{
    this->command=command;
    QStringList arg=command.split(';');
    QStringList cardIDList;
    int targetID,targetArea;
    int sourceArea;
    int cardID;
    int hitRate;
    int i,howMany;
    int team,gem,crystal;

    Card*card;
    Player*player;
    Team *red=dataInterface->getRedTeam();
    Team *blue=dataInterface->getBlueTeam();
    QList<Card*> cards;
    QString msg;
    QString flag;
    QString cardName;
    QList<Player*>playerList=dataInterface->getPlayerList();
    ShowArea* showArea=gui->getShowArea();

    switch (arg[0].toInt())
    {
//�غϿ�ʼ
    case 3:
        targetID=arg[1].toInt();
        gui->logAppend("-----------------------------------");
        gui->logAppend(playerList[targetID]->getName()+tr("�غϿ�ʼ"));
        playerArea->setCurrentPlayerID(targetID);
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
        }
        else{
            myRole->turnBegin();
        }
        break;
//Ӧսѯ��
    case 5:
        hitRate=arg[1].toInt();
        cardID=arg[2].toInt();
        targetID=arg[3].toInt();
        sourceID=arg[4].toInt();
        card=dataInterface->getCard(cardID);

        if(targetID!=myID)
        {
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            msg=playerList[sourceID]->getName()+tr("��")+tr("��")+tr("ʹ����")+card->getName();
            if (hitRate==2)
                return;

            if(hitRate==1)
                msg+=tr("���ù����޷�Ӧս");

            gui->reset();
            tipArea->setMsg(msg);
            myRole->attacked(card->getElement(),hitRate);
        }
        break;
//����ѯ��
    case 7:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        flag=arg[3];
        msg=playerList[targetID]->getName()+tr("��Ҫ��")+arg[2]+tr("������");
        if(flag=="y")
            gui->logAppend(msg+tr("(����)"));
        else
            gui->logAppend(msg+tr("(����)"));
        if(targetID!=myID)
        {
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            gui->reset();
            drop(howMany);
            tipArea->setMsg(tr("����Ҫ��")+arg[2]+tr("������"));
        }
        break;
//����
    case 9:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        gui->logAppend(playerList[targetID]->getName()+tr("��ȡ")+arg[2]+tr("������"));

        if(targetID==myID)
        {
            cardIDList=arg[3].split(',');
            for(i=0;i<howMany;i++)
            {
                cardID=cardIDList[i].toInt();
                card=dataInterface->getCard(cardID);
                dataInterface->addHandCard(card);
            }
        }
        break;
//�ƶ���ϴ
    case 10:
        gui->logAppend(tr("�ƶ�����ϴ"));
        break;
//ʿ���ı�
    case 11:
        team=arg[1].toInt();
        howMany=arg[2].toInt();
        if(team)
        {
            red->setMorale(howMany);
            teamArea->update();
        }
        else
        {
            blue->setMorale(howMany);
            teamArea->update();
        }

        break;
//��Ϸ����
    case 12:
        team=arg[1].toInt();
        msg=tr("��Ϸ����,");
        if(team)
            msg+=tr("��ӻ�ʤ");
        else
            msg+=tr("���ӻ�ʤ");
        gui->logAppend(msg);
        tipArea->setMsg(msg);
        break;
//���ƹ���
    case 13:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        flag=arg[3];
        msg=playerList[targetID]->getName()+tr("������")+arg[2]+tr("������");
        if(flag=="y")
        {
            cardIDList=arg[4].split(',');
            msg+=":";
            cards.clear();
            for(i=0;i<howMany;i++)
            {
                cardID=cardIDList[i].toInt();
                card=dataInterface->getCard(cardID);
                msg+=card->getName()+"-"+card->getProperty()+" ";
                cards<<card;
            }
            showArea->showCards(cards);
        }
        gui->logAppend(msg);
        break;
//���й���
    case 14:
        flag=arg[1];
        targetID=arg[3].toInt();
        sourceID=arg[4].toInt();
        if(flag=="1")
            msg=playerList[sourceID]->getName()+tr("δ����")+playerList[targetID]->getName();
        else
            msg=playerList[sourceID]->getName()+tr("����")+playerList[targetID]->getName();
        gui->logAppend(msg);
        break;
//��ʯ�ı�
    case 15:
        team=arg[1].toInt();
        gem=arg[2].toInt();
        crystal=arg[3].toInt();
        if(team)
        {
            red->setGem(gem);
            red->setCrystal(crystal);
            teamArea->update();
        }
        else
        {
            blue->setGem(gem);
            blue->setCrystal(crystal);
            teamArea->update();
        }
        break;
//�Ǳ��ı�
    case 17:
        team=arg[1].toInt();
        howMany=arg[2].toInt();
        if(team)
        {
            red->setGrail(howMany);
            teamArea->update();
        }
        else
        {
            blue->setGrail(howMany);
            teamArea->update();
        }
        break;
//�����ı�
    case 18:
        targetID=arg[1].toInt();
        gem=arg[2].toInt();
        crystal=arg[3].toInt();
        player=playerList.at(targetID);
        player->setGem(gem);
        player->setCrystal(crystal);
        playerArea->update();
        break;
//����ͨ��
    case 19:
        howMany=arg[1].toInt();
        cardIDList=arg[2].split(",");
        sourceID=arg[3].toInt();
        sourceArea=arg[4].toInt();
        targetID=arg[5].toInt();
        targetArea=arg[6].toInt();
        if(sourceID!=-1)
        {
            switch(sourceArea)
            {
            case 4:
                player=playerList.at(sourceID);
                player->changeHandCardNum(-howMany);
                break;
            case 5:
                cardID=cardIDList[0].toInt();
                card=dataInterface->getCard(cardID);
                playerList[sourceID]->removeStatus(card);
                break;
            }
        }
        if(targetID!=-1)
        {
            switch(targetArea)
            {
            case 4:
                player=playerList.at(targetID);
                player->changeHandCardNum(howMany);
                break;
            case 5:
                cardID=cardIDList[0].toInt();
                card=dataInterface->getCard(cardID);
                cardName=card->getName();
                if(cardName==tr("�ж�"))
                    playerList[targetID]->addStatus(0,card);
                if(cardName==tr("����"))
                    playerList[targetID]->addStatus(1,card);
                if(cardName==tr("ʥ��")||card->getSpecialityList().contains(tr("��ʹ֮ǽ")))
                    playerList[targetID]->addStatus(2,card);
                if(card->getType()=="attack"&&card->getProperty()==tr("��"))
                    playerList[targetID]->addStatus(3,card);
                if(card->getSpecialityList().contains(tr("�����͸�")))
                    playerList[targetID]->addStatus(4,card);
                if(card->getSpecialityList().contains(tr("Ѹ�ݴ͸�")))
                    playerList[targetID]->addStatus(5,card);
                break;
            }
        }
        playerArea->update();
        break;
//����ͨ��
    case 20:
        targetID=arg[1].toInt();
        sourceID=arg[3].toInt();
        gui->logAppend(playerList[sourceID]->getName()+tr("��")+playerList[targetID]->getName()+tr("�����")+arg[2]+tr("�㹥���˺�"));
        break;
//����ͨ��
    case 21:
        targetID=arg[1].toInt();
        sourceID=arg[3].toInt();
        gui->logAppend(playerList[sourceID]->getName()+tr("��")+playerList[targetID]->getName()+tr("ʹ�õ�")+arg[4]+tr("�����")+arg[2]+tr("�㷨���˺�"));
        break;
//����ѯ��
    case 22:
        targetID=arg[1].toInt();
        gui->logAppend(tr("�ȴ�")+playerList[targetID]->getName()+tr("������Ӧ"));
        if(targetID!=myID)
        {
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            gui->reset();
            state=7;
            decisionArea->enable(0);
            decisionArea->enable(1);
            tipArea->setMsg(tr("�㱻�����ˣ�ѡ�������ж��׶Σ�ѡ����")+arg[2]+tr("����"));
        }
        break;
//�������
    case 24:
        targetID=arg[1].toInt();
        if(arg[2]=="0")
            gui->logAppend(playerList[targetID]->getName()+tr("ѡ�������ж��׶�"));
        else
            gui->logAppend(playerList[targetID]->getName()+tr("ѡ����")+arg[3]+tr("��������"));
        break;
//ʥ���Ƴ�ͨ��
    case 25:
        targetID=arg[1].toInt();
        gui->logAppend(playerList[targetID]->getName()+tr("��ʥ�ܱ��Ƴ�"));
        break;
//ħ��ѯ��
    case 26:
        int nextID;
        targetID=arg[1].toInt();
        sourceID=arg[2].toInt();
        howMany=arg[3].toInt();
        nextID=arg[4].toInt();
        gui->logAppend(playerList[sourceID]->getName()+tr("��")+playerList[targetID]->getName()+tr("ʹ����ħ����Ŀǰ�˺�Ϊ��")+arg[3]+tr("��"));
        if(targetID!=myID)
        {
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);            
            myRole->moDaned(nextID,sourceID,howMany);
        }
        break;
//����ͨ��
    case 28:
        cardID=arg[1].toInt();
        targetID=arg[2].toInt();
        sourceID=arg[3].toInt();
        flag=arg[4];
        card=dataInterface->getCard(cardID);
        cardName=card->getName();
        if(flag=="1"){
            cards.clear();
            cards<<card;
            showArea->showCards(cards);
        }
        if(card->getElement()!="light")
            gui->logAppend(playerList[sourceID]->getName()+tr("��")+playerList[targetID]->getName()+tr("ʹ����")+cardName);
        else
            gui->logAppend(playerList[sourceID]->getName()+tr("ʹ��ʥ�����"));
        break;
//�ж��׶� flag 0-�����ж���1-�����ж���2-�����ж���3-�����ж���4-���������ж�
    case 29:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
            actionFlag=-1;
        }
        else
        {
            gui->setEnable(1);
            if(flag=="0")
                myRole->normal();
            else if(flag=="1")
                myRole->attackAction();
            else if(flag=="2")
                myRole->magicAction();
            else if(flag=="4")
                myRole->attackOrMagic();
            if(arg[3]=="1")
                decisionArea->enable(3);
        }
        break;
//�޷��ж�����
    case 31:
        targetID=arg[1].toInt();
        msg=playerList[targetID]->getName()+tr("�����޷��ж�");
        gui->logAppend(msg);
        break;
//���Ƹı�
    case 32:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        playerList[targetID]->setCrossNum(howMany);
        break;
//����ѯ��
    case 33:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        flag=arg[3];
        gui->reset();
        if(targetID==myID)
            myRole->cure(playerList[myID]->getCrossNum(),howMany,flag.toInt());
        break;
//������Ӧѯ��
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        msg=tr("�ȴ�")+playerList[targetID]->getName()+flag+tr("��Ӧ");
        gui->logAppend(msg);
        gui->reset();
        gui->setEnable(0);
        if(targetID==myID)
        {
            gui->setEnable(1);
            myRole->askForSkill(flag);
        }
        else
            gui->setEnable(0);
        break;
//��Ϣͨ��
    case 38:
        gui->logAppend(arg[1]);
        break;
//��ɫ��̬ת��֪ͨ
    case 39:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(flag=="0")
        {
            playerList[targetID]->setTap(0);
            msg=playerList[targetID]->getName()+tr("����")+arg[3];
        }
        else
        {
            playerList[targetID]->setTap(1);
            msg=playerList[targetID]->getName()+tr("����")+arg[3];
        }
        playerArea->update();
        gui->logAppend(msg);
        gui->reset();
        gui->setEnable(0);
        break;
//�������ޱ��֪ͨ
    case 40:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        playerList[targetID]->setHandCardsMax(howMany);
        playerArea->update();
        break;
//�������ͨ��
    case 41:
        targetID=arg[1].toInt();
        flag=arg[2];
        howMany=arg[3].toInt();
        gui->logAppend(playerList[targetID]->getName()+tr("���")+arg[3]+tr("������"));

        if(targetID==myID)
        {
            cardIDList=arg[4].split(',');
            for(i=0;i<howMany;i++)
            {
                cardID=cardIDList[i].toInt();
                card=dataInterface->getCard(cardID);
                dataInterface->addHandCard(card);
            }
        }
        break;
//�����ж�ѯ��
    case 42:
        targetID=arg[1].toInt();
        gui->logAppend(tr("�ȴ�")+playerList[targetID]->getName()+tr("�����ж���Ӧ"));
        if(targetID==myID)
        {
            gui->setEnable(1);
            myRole->additionalAction();
        }
        else
            gui->setEnable(0);
        break;
//ר�����
    case 43:
        targetID=arg[1].toInt();
        msg=arg[2];
        playerList.at(targetID)->setSpecial(msg.toInt(),arg[3].toInt());
        playerArea->update();
        break;
//��Ǳ��
    case 45:
        targetID=arg[1].toInt();
        flag=arg[2];
        howMany=arg[3].toInt();
        playerList.at(targetID)->setToken(flag.toInt(),howMany);
        playerArea->update();
        break;
//��ʹף��
    case 750:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        msg=tr("�ȴ�")+playerList[targetID]->getName()+tr("��ʹף�������ƣ���Ӧ");
        gui->logAppend(msg);
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            TianShiZhuFu(howMany);
        }
        break;
//ħ����������ƣ�
    case 850:
        targetID=arg[1].toInt();
        msg=tr("�ȴ�")+playerList[targetID]->getName()+tr("ħ����������ƣ���Ӧ");
        gui->logAppend(msg);
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            MoBaoChongJi();
        }
        break;

    }
}


