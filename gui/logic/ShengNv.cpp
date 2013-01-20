#include "ShengNv.h"

ShengNv::ShengNv()
{
    makeConnection();
    Button *zhiLiaoShu, *zhiYuZhiGuang, *shengLiao;
    zhiLiaoShu = new Button(3,tr("������"));
    buttonArea->addButton(zhiLiaoShu);
    connect(zhiLiaoShu,SIGNAL(buttonSelected(int)),this,SLOT(ZhiLiaoShu()));

    zhiYuZhiGuang=new Button(4, tr("����֮��"));
    buttonArea->addButton(zhiYuZhiGuang);
    connect(zhiYuZhiGuang,SIGNAL(buttonSelected(int)),this,SLOT(ZhiYuZhiGuang()));

    shengLiao=new Button(5,tr("ʥ ��"));
    buttonArea->addButton(shengLiao);
    connect(shengLiao,SIGNAL(buttonSelected(int)),this,SLOT(ShengLiao()));
}

void ShengNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(tr("������")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(tr("����֮��")))
        buttonArea->enable(4);
    //ʥ��
    if (myself->getEnergy()>0 && !onceUsed)
        buttonArea->enable(5);
    unactionalCheck();
}


void ShengNv::attackOrMagic()
{
    Role::attackOrMagic();
    Player* myself=dataInterface->getMyself();
    if (handArea->checkSpecility(tr("������")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(tr("����֮��")))
        buttonArea->enable(4);
    if (myself->getEnergy()>0 && !onceUsed)
        buttonArea->enable(5);
}


void ShengNv::ZhiLiaoShu()
{
    state=602;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(tr("������"));
}

void ShengNv::ZhiYuZhiGuang()
{
    state=603;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();


    handArea->setQuota(1);
    playerArea->setQuota(1,3);
    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(tr("����֮��"));
}

void ShengNv::LianMin()
{
    state=604;
    tipArea->setMsg(tr("�Ƿ񷢶�������"));
    QList<Card*> handcards=dataInterface->getHandCards();
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
            else if(handcards[i]->getProperty()==tr("ʥ"))
            {
                flag=true;
                break;
            }
    }
    if(flag||dataInterface->getMyself()->getEnergy()>1)
        decisionArea->enable(0);
}
void ShengNv::ShengLiao()
{
    int gem, crystal;
    Player*myself=dataInterface->getMyself();
    gem=myself->getGem();
    crystal=myself->getCrystal();

    state=651;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    playerArea->enableAll();

    tipArea->setMsg(tr("ÿ�ε�ѡһ�˷���һ�����ƣ�������"));
    if(crystal>=1)
        tipArea->addBoxItem(tr("1.ˮ��"));
    if(gem>=1)
        tipArea->addBoxItem(tr("2.��ʯ"));

    tipArea->showBox();
}
void ShengNv::ShengLiao2()
{
    state=652;
    onceUsed=true;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    buttonArea->reset();

    tipArea->setMsg(tr("��ѡ�����ڶ�������"));
    playerArea->enableAll();
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->disable(1);
    decisionArea->disable(0);

}
void ShengNv::ShengLiao3()
{
    state=653;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    buttonArea->reset();

    tipArea->setMsg(tr("��ѡ��������������"));
    playerArea->enableAll();
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->disable(1);
    decisionArea->disable(0);

}
void ShengNv::BingShuangDaoYan()
{
    state=601;

    tipArea->setMsg(tr("��ѡ���˪���Ե�Ŀ��"));
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShengNv::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case 602:
        playerArea->enableAll();
        break;
    case 603:
        playerArea->enableAll();
        break;
    }
}

void ShengNv::onOkClicked()
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
//�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        //���������ж�
        if(text[0]=='1'){
            actionFlag=4;
            actions.removeOne(tr("1.���������ж���ʥ�ƣ�"));
            attackOrMagic();
        }
        break;
//��˪����
    case 601:
        command="601;";
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        emit sendCommand(command);
        gui->reset();
        break;
//������
    case 602:
        command="602;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//����֮��
    case 603:
        command="603;";
        int n;
        n=selectedPlayers.size();
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        command+=cardID+";"+QString::number(myID)+";"+QString::number(n)+";";
        for(int i=0; i<n; i++)
            command+=QString::number(selectedPlayers[i]->getID())+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//����
    case 604:
        start=true;
        command="604;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//ʥ��
    case 651:        
        command="605;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+="0;";
        else
            command+="1;";
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        ShengLiao2();
        break;
    case 652:
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        ShengLiao3();
        break;
    case 653:
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        sourceID=QString::number(myID);
        command+=sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}


void ShengNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//����
    case 604:
        command="604;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//�����ж�
    case 1:
//��˪����
//������
    case 602:
//����֮��
    case 603:
//ʥ��
    case 651:
        if(actionFlag==0)
            normal();
        else if(actionFlag==4)
            attackOrMagic();
        break;
    }
}

void ShengNv::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch (arg[0].toInt())
    {
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
//������Ӧѯ��
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            gui->setEnable(1);
            if(flag==tr("��˪����"))
                BingShuangDaoYan();
            else if(flag==tr("����"))
                LianMin();
        }
        break;
//�����ж�ѯ��
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {
            if(state==653){
                actions.append(tr("1.���������ж���ʥ�ƣ�"));
            }
            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();
            state=42;
        }
    }
}
