#include "KuangZhan.h"

KuangZhan::KuangZhan()
{
    makeConnection();
setMyRole(this);
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

void KuangZhan::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("˺��"))
        SiLie();
    else if(skill==tr("Ѫ������"))
        XueXingPaoXiao();
    else if(skill==tr("ѪӰ��"))
        XueYingKuangDao();
}
