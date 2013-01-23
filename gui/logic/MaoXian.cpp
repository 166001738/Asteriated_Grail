#include "MaoXian.h"
MaoXian::MaoXian()
{
    makeConnection();
    connect(playerArea,SIGNAL(playerUnready()),this,SLOT(onUnready()));

    Button *qiZha,*touTianHuanRi,*teShuJiaGong;
    qiZha=new Button(3,tr("�� թ"));
    buttonArea->addButton(qiZha);
    connect(qiZha,SIGNAL(buttonSelected(int)),this,SLOT(QiZha()));

    touTianHuanRi=new Button(4,tr("͵�컻��"));
    buttonArea->addButton(touTianHuanRi);
    connect(touTianHuanRi,SIGNAL(buttonSelected(int)),this,SLOT(TouTianHuanRi()));

    teShuJiaGong=new Button(5,tr("����ӹ�"));
    buttonArea->addButton(teShuJiaGong);
    connect(teShuJiaGong,SIGNAL(buttonSelected(int)),this,SLOT(TeShuJiaGong()));
}

void MaoXian::buy()
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
        tipArea->setMsg(tr("����3���ƣ��㷽ս����������ʯ"));
    if(energy==4)
        tipArea->setMsg(tr("ս������ʯ����4��������ֻ����һ��ʯ"));
    if(energy==5)
        tipArea->setMsg(tr("ս������ʯ��Ŀ�Ѵ����ޣ����򽫲���������ʯ"));
}

void MaoXian::extract()
{
    state=1204;
    tipArea->reset();
    handArea->reset();
    playerArea->reset();
    decisionArea->disable(0);
    decisionArea->enable(1);
    playerArea->enableMate(true);
    playerArea->setQuota(1);
    tipArea->setMsg(tr("����ѡ��Ҫ����Ķ���"));
}

void MaoXian::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if(dataInterface->getMyTeam()->getEnergy()>0)
        buttonArea->enable(2);
//��թ
    QList<Card*>handcards=dataInterface->getHandCards();
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                buttonArea->enable(3);
                break;
            }

//͵�컻�� ����ӹ�
    if(myself->getEnergy()>0){
        if(dataInterface->getOtherTeam()->getGem()>0 && !onceUsed2)
            buttonArea->enable(4);
        if(!onceUsed)
            buttonArea->enable(5);
    }
    unactionalCheck();
}

void MaoXian::attackOrMagic()
{
    Role::attackOrMagic();
    Player* myself=dataInterface->getMyself();
    QList<Card*>handcards=dataInterface->getHandCards();
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                buttonArea->enable(3);
                break;
            }
    if(myself->getEnergy()>0){
        if(dataInterface->getOtherTeam()->getGem()>0 && !onceUsed2)
            buttonArea->enable(4);
        if(!onceUsed)
            buttonArea->enable(5);
    }
}

void MaoXian::attackAction()
{
    Role::attackAction();
    QList<Card*>handcards=dataInterface->getHandCards();
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                buttonArea->enable(3);
                break;
            }
}

void MaoXian::QiZha()
{
    state=10;
    qizha=true;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(2,3);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();
    tipArea->setMsg(tr("��ѡ����թ�����ԣ�3��ͬϵ�Զ���Ϊ���𣩣�"));
    tipArea->addBoxItem(tr("1.��"));
    tipArea->addBoxItem(tr("2.ˮ"));
    tipArea->addBoxItem(tr("3.��"));
    tipArea->addBoxItem(tr("4.��"));
    tipArea->addBoxItem(tr("5.��"));
    tipArea->showBox();
}

void MaoXian::TeShuJiaGong()
{
    state=1202;
    Player*myself=dataInterface->getMyself();

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(tr("��ѡ��ʹ�õ�������"));
    if(myself->getCrystal()>=1)
        tipArea->addBoxItem(tr("1.ˮ��"));
    if(myself->getGem()>=1)
        tipArea->addBoxItem(tr("2.��ʯ"));

    tipArea->showBox();
}

void MaoXian::TouTianHuanRi()
{
    state=1203;
    Player*myself=dataInterface->getMyself();

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(tr("��ѡ��ʹ�õ�������"));
    if(myself->getCrystal()>=1)
        tipArea->addBoxItem(tr("1.ˮ��"));
    if(myself->getGem()>=1)
        tipArea->addBoxItem(tr("2.��ʯ"));

    tipArea->showBox();
}

void MaoXian::cardAnalyse()
{
    Role::cardAnalyse();
    QList<Card*> selectedCards=handArea->getSelectedCards();
    switch (state)
    {
    case 10:
        foreach(Card*ptr,selectedCards)
            if(ptr->getElement()!=selectedCards[0]->getElement()){
                playerArea->disableAll();
                break;
            }            
        break;
    }
}

void MaoXian::playerAnalyse()
{
    Player*dst;
    int gem;
    int crystal;
    switch (state)
    {
    case 1204:
        dst=playerArea->getSelectedPlayers().at(0);
        gem=dataInterface->getMyTeam()->getGem();
        crystal=dataInterface->getMyTeam()->getCrystal();
        decisionArea->enable(0);
        tipArea->reset();
        tipArea->setMsg(tr("��ѡ��Ҫ��ȡ����ʯ����"));
        switch(dst->getEnergyMax()-dst->getEnergy())
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
        break;
    default:
        decisionArea->enable(0);
        break;
    }
}

void MaoXian::onOkClicked()
{
    if(qizha)
        state=1201;
    if(state==4)
        state=1205;
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
//�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            actionFlag=4;
            actions.removeOne(tr("1.���������ж�������ӹ���"));
            attackOrMagic();
        }
        else if(text[0]=='2'){
            actionFlag=4;
            actions.removeOne(tr("2.���������ж���͵�컻�գ�"));
            attackOrMagic();
        }
        break;
//��թ
    case 1201:
        command="1201;";
        if(selectedCards.size()==3)
            flag=6;
        else
            flag=tipArea->getBoxCurrentIndex()+1;
        switch(flag)
        {
        case 1:
            cardID=QString::number(66);
            break;
        case 2:
            cardID=QString::number(129);
            break;
        case 3:
            cardID=QString::number(87);
            break;
        case 4:
            cardID=QString::number(45);
            break;
        case 5:
            cardID=QString::number(108);
            break;
        case 6:
            cardID=QString::number(39);
            break;
        }
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";"+QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(command);
        qizha=false;
        gui->reset();
        break;
//����ӹ�
    case 1202:
        command="1202;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+="0;";
        else
            command+="1;";
        command+=QString::number(myID)+";";
        onceUsed=true;
        emit sendCommand(command);
        gui->reset();
        break;
//͵�컻��
    case 1203:
        command="1203;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+="0;";
        else
            command+="1;";
        command+=QString::number(myID)+";";
        onceUsed2=true;
        emit sendCommand(command);
        gui->reset();
        break;
//ð�ռ�����
    case 1204:
        command+="1204;";
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        command+=QString::number(myID)+";";
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
        emit sendCommand(command);
        gui->reset();
        break;
//����
    case 1205:
        command="4;3;0;";
        int stone=dataInterface->getMyTeam()->getEnergy();
        if(stone<4)
            command+="2;0;";
        else if(stone==4)
                command+="1;0;";
        else
            command+="0;0;";
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(command);
        break;
    }
}

void MaoXian::onUnready()
{
    switch(state)
    {
    case 1204:
        tipArea->reset();
        normal();
        break;
    }
}

void MaoXian::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
    case 1:
//��թ
    case 10:
    case 1201:
//����ӹ�
    case 1202:
//͵�컻��
    case 1203:
//ð��������
    case 1204:
        if(actionFlag==4)
            attackOrMagic();
        if(actionFlag==0)
            normal();
        qizha=false;
        break;
    }
}

void MaoXian::decipher(QString command)
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
            onceUsed2=false;
            qizha=false;
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
            if(flag=="1")
                attackAction();
        }
        break;
//�����ж�ѯ��
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {            
            if(state==1202)
                actions.append(tr("1.���������ж�������ӹ���"));
            if(state==1203)
                actions.append(tr("2.���������ж���͵�컻�գ�"));

            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();
            state=42;
        }
        break;
    }
}

