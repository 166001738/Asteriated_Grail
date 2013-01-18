#include "KuangZhan.h"

KuangZhan::KuangZhan()
{
    makeConnection();
}

void KuangZhan::SiLie()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶�˺�ѣ�"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void KuangZhan::XueXingPaoXiao()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶�Ѫ��������"));
    decisionArea->enable(0);
    decisionArea->enable(1);

}

void KuangZhan::XueYingKuangDao()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶�ѪӰ�񵶣�"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void KuangZhan::onOkClicked()
{
    Role::onOkClicked();
    QString command;
    switch(state)
    {
//˺��ѯ��
//ѪӰ����ѯ��
//ѪӰ��ѯ��
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void KuangZhan::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//˺��ѯ��
//ѪӰ����ѯ��
//ѪӰ��ѯ��
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void KuangZhan::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch (arg[0].toInt())
    {
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            gui->setEnable(1);
            if(flag==tr("˺��"))
                SiLie();
            else if(flag==tr("Ѫ������"))
                XueXingPaoXiao();
            else if(flag==tr("ѪӰ��"))
                XueYingKuangDao();
        }
        break;

    }
}
