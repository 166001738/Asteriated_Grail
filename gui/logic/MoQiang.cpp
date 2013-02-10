#include "MoQiang.h"

MoQiang::MoQiang()
{
    makeConnection();
    setMyRole(this);

    Button *chongying;
    chongying=new Button(3,"��ӯ");
    buttonArea->addButton(chongying);
    connect(chongying,SIGNAL(buttonSelected(int)),this,SLOT(ChongYing()));
}

void MoQiang::normal()
{
    Role::normal();
    handArea->disableMagic();
    if(!jieFangFirst)
    {
        if(handArea->checkElement("thunder") || handArea->checkType("magic"))
            buttonArea->enable(3);
    }
}

void MoQiang::AnZhiJieFang()
{
    state=2901;
    tipArea->setMsg(tr("�Ƿ񷢶���֮��ţ�"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getType()!="magic")
            {
                flag=true;
                break;
            }
    }
    if(flag)
        decisionArea->enable(0);
}

void MoQiang::HuanYingXingChen(int flag)
{
    state=2902;
    huanYingFlag=flag;
    if(flag==0){
    tipArea->setMsg(tr("�Ƿ񷢶���Ӱ�ǳ���"));
    decisionArea->enable(1);
    decisionArea->enable(0);
    }
    else
    {
        decisionArea->disable(1);
        decisionArea->disable(0);

        playerArea->enableAll();
        playerArea->setQuota(1);
    }
}

void MoQiang::AnZhiBiZhang()
{
    state=2903;
    tipArea->setMsg(tr("�Ƿ񷢶���֮���ϣ�"));
    handArea->setQuota(1,7);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("thunder");
    handArea->enableMagic();
}

void MoQiang::QiHeiZhiQiang()
{
    state=2904;
    tipArea->reset();
    tipArea->setMsg(tr("�Ƿ񷢶����֮ǹ��������ѡ�񷢶���������"));
    decisionArea->enable(0);
    decisionArea->enable(1);

    Player* myself=dataInterface->getMyself();
    int min=myself->getEnergy();
    for(;min>0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
}

void MoQiang::ChongYing()
{
    state=2905;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableElement("thunder");
    handArea->enableMagic();
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void MoQiang::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case 2903:
    case 2905:
        decisionArea->enable(0);
        break;
    }
}

void MoQiang::playerAnalyse()
{
    Role::playerAnalyse();
    switch(state)
    {
    case 2902:
        decisionArea->enable(0);
        break;
    }
}

void MoQiang::turnBegin()
{
    Role::turnBegin();
    jieFangFirst=false;
    chongYingUsed=false;
}

void MoQiang::additionalAction()
{
    Role::additionalAction();
    if(chongYingUsed)
        tipArea->addBoxItem("1.���⹥���ж�����ӯ��");
}

void MoQiang::moDaned(int nextID, int sourceID, int howMany)
{
    Role::moDaned(nextID,sourceID,howMany);
    handArea->disableMagic();
}

void MoQiang::attacked(QString element, int hitRate)
{
    Role::attacked(element,hitRate);
    handArea->disableMagic();
}

void MoQiang::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("��֮���"))
        AnZhiJieFang();
    else if(skill==tr("��Ӱ�ǳ�"))
        HuanYingXingChen(command.split(';').at(3).toInt());
    else if(skill==tr("��֮����"))
        AnZhiBiZhang();
    else if(skill==tr("���֮ǹ"))
        QiHeiZhiQiang();
}

void MoQiang::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    case 42:
        chongYingUsed=false;
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            emit sendCommand("2906;"+QString::number(myID)+";");
            attackAction();
        }
        break;
    case 2901:
        command="2901;1;";
        start=true;
        jieFangFirst=true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 2902:
        command="2902;1;";
        start=true;
        command+=QString::number(huanYingFlag)+";";
        if(huanYingFlag==1)
        {
            targetID=QString::number(selectedPlayers[0]->getID());
            sourceID=QString::number(myID);
            command+=targetID+";"+sourceID+";";
        }
        emit sendCommand(command);
        gui->reset();
        break;
    case 2903:
        command="2903;1;";
        command+=QString::number(selectedCards.size())+";";
        for(int i=0;i<selectedCards.size();i++)
        {
            command+=QString::number(selectedCards[i]->getID())+":";
            dataInterface->removeHandCard(selectedCards[i]);
        }
        command+=";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2904:
        command="2904;1;";
        text=tipArea->getBoxCurrentText();
        command+=text+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2905:
        chongYingUsed=true;
        command="2905;";
        sourceID=QString::number(myID);
        command+=sourceID+";";
        cardID=QString::number(selectedCards[0]->getID());
        command+=cardID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void MoQiang::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    switch(state)
    {
    case 2901:
        command="2901;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2902:
        command="2902;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2903:
        command="2903;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2904:
        command="2904;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2905:
        normal();
        break;
    }
}

