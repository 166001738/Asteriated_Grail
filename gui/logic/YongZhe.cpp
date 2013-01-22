#include "YongZhe.h"

YongZhe::YongZhe()
{
    makeConnection();

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
    //ŭ���
    //����ֹˮ��
    //������
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
    //�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0].digitValue()==1)
        {
            jinDuanZhiLi--;         
            actions.removeOne(tr("1.�����ж�����ƣ���ߣ�"));
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
    case 1:
    case 2101:
        normal();
        break;
    //ŭ���
    //����ֹˮ��
    //������
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
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

void YongZhe::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch(arg[0].toInt())
    {
    //�غϿ�ʼ
    case 3:
        targetID=arg[1].toInt();
        if(targetID==myID)
            jinDuanZhiLi=0;
        break;
    //�ж��׶�
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
            if(flag==tr("ŭ��"))
                NuHou();
            else if(flag==tr("����ֹˮ"))
                MingJingZhiShui();
            else if(flag==tr("����֮��"))
                JinDuanZhiLi();
            else if(flag==tr("����"))
                SiDou();
        }
        break;
    //�����ж�ѯ��
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {
            if(jinDuanZhiLi>0)
                actions.append(tr("1.�����ж�����ƣ���ߣ�"));

            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();
            state=42;
        }
        break;
    }
}
