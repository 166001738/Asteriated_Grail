#include "YongZhe.h"

YongZhe::YongZhe()
{
    makeConnection();
setMyRole(this);

    Button *tiaoXin;
    tiaoXin=new Button(3,tr("����"));
    buttonArea->addButton(tiaoXin);
    connect(tiaoXin,SIGNAL(buttonSelected(int)),this,SLOT(TiaoXin()));
}

void YongZhe::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    //����
    if(myself->getToken(0)>0)
        buttonArea->enable(3);
        foreach(Player* ptr,dataInterface->getPlayerList())
            if(ptr->getSpecial(1)==1){
                buttonArea->disable(3);
                break;
            }
    unactionalCheck();
}

void YongZhe::NuHou()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶�ŭ��"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::MingJingZhiShui()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶�����ֹˮ��"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::JinDuanZhiLi()
{
    state=2102;
    tipArea->setMsg(tr("�Ƿ񷢶�����֮����"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::SiDou()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶�������"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::TiaoXin()
{
    state=2101;

    handArea->reset();
    tipArea->reset();
    playerArea->reset();

    playerArea->enableEnemy();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YongZhe::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;

    QString command;
    QString sourceID;
    QString targetID;
    QString text;

    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    //�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0].digitValue()==1)
        {
            emit sendCommand("2103;"+QString::number(myID)+";");
            jinDuanZhiLi--;                     
            attackAction();
        }
        break;
    //����
    case 2101:
        command="2101;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    //����֮��
    case 2102:
        jinDuanZhiLi++;
        command="2102;1;";
        foreach(Card*ptr,dataInterface->getHandCards())
            dataInterface->removeHandCard(ptr);
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void YongZhe::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 2101:
        normal();
        break;
    //����֮��
    case 2102:
        if(jinDuanZhiLi>0)
            jinDuanZhiLi=0;
        command="2102;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}
void YongZhe::turnBegin()
{
    Role::turnBegin();
    jinDuanZhiLi=0;
}

void YongZhe::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("ŭ��"))
        NuHou();
    else if(skill==tr("����ֹˮ"))
        MingJingZhiShui();
    else if(skill==tr("����֮��"))
        JinDuanZhiLi();
    else if(skill==tr("����"))
        SiDou();
}

void YongZhe::additionalAction()
{
    Role::additionalAction();
    if(jinDuanZhiLi>0)
        tipArea->addBoxItem(tr("1.�����ж�����ƣ���ߣ�"));
}

