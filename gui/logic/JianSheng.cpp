#include "JianSheng.h"

JianSheng::JianSheng()
{
    makeConnection();
}

void JianSheng::LieFengJi()
{
    state=36;
    tipArea->setMsg(tr("�Ƿ񷢶��ҷ缼��"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void JianSheng::LianXuJi()
{
    //�Ƚ��û���Ķ��⹥���ж�״̬
    state=10;
    onceUsed=true;
    gui->reset();
    handArea->setQuota(1);
    handArea->enableElement("wind");
    handArea->disableMagic();
    playerArea->setQuota(1);
    decisionArea->enable(3);
}

void JianSheng::JianYing(){
    attackAction();
    Player*myself=dataInterface->getMyself();
    onceUsed2=true;
    tipArea->setMsg(tr("��ѡ��ʹ�õ�������"));
    if(myself->getCrystal()>=1)
        tipArea->addBoxItem(tr("1.ˮ��"));
    if(myself->getGem()>=1)
        tipArea->addBoxItem(tr("2.��ʯ"));

    tipArea->showBox();
}

void JianSheng::onOkClicked()
{
    if(state==10&&flag==2)
        state=101;
    if(state==10&&flag==3)
        state=102;
    QList<Card*> selectedCards=handArea->getSelectedCards();
    QList<Player*> selectedPlayers=playerArea->getSelectedPlayers();
    Role::onOkClicked();
    QString command,text,cardID,targetID,sourceID;

    switch(state)
    {
//NORMALACTION
    case 1:
//׷���ж�
    case 10:
    case 12:
        if(selectedCards[0]->getSpecialityList().contains(tr("���缼")))
            JiFengJi++;
        break;
//�ҷ缼ѯ��
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//�����ж�ѯ��
    case 42:
        text=tipArea->getBoxCurrentText();
        flag=text[0].digitValue();
        switch (flag){
        case 1:
            JiFengJi--;
            actions.removeOne(tr("1.�����ж������缼��"));
            attackAction();            
            break;
        case 2:
            actions.removeOne(tr("2.������"));
            LianXuJi();
            break;
        case 3:
            actions.removeOne(tr("3.��Ӱ"));
            JianYing();
            break;
        }
        break;
//������
    case 101:
        cardID=QString::number(selectedCards[0]->getID());
        targetID=QString::number(selectedPlayers[0]->getID());
        sourceID=QString::number(myID);
        command="101;"+cardID+";"+targetID+";"+sourceID+";";
        if(selectedCards[0]->getSpecialityList().contains(tr("���缼")))
            JiFengJi++;
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        usedAttack=true;
        usedMagic=usedSpecial=false;
        emit sendCommand(command);
        break;
//��Ӱ
    case 102:
        cardID=QString::number(selectedCards[0]->getID());
        targetID=QString::number(selectedPlayers[0]->getID());
        sourceID=QString::number(myID);
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command="102;"+cardID+";"+targetID+";"+sourceID+";0;";
        else
            command="102;"+cardID+";"+targetID+";"+sourceID+";1;";
        if(selectedCards[0]->getSpecialityList().contains(tr("���缼")))
            JiFengJi++;
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        usedAttack=true;
        usedMagic=usedSpecial=false;
        emit sendCommand(command);
        break;
    }
}

void JianSheng::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//�ҷ缼ѯ��
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void JianSheng::decipher(QString command)
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
            JiFengJi=0;
        }
        break;
//�ҷ缼ѯ��
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            gui->setEnable(1);
            if(flag==tr("�ҷ缼"))
                LieFengJi();
        }
        break;
//�����ж�ѯ��
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {
            QList<Card*> handcards=dataInterface->getHandCards();

            if(JiFengJi>0 && !actions.contains(tr("1.�����ж������缼��"))){
                actions.append(tr("1.�����ж������缼��"));
            }
            if(!onceUsed&&usedAttack && !actions.contains(tr("2.������")))
            {
                foreach(Card*ptr,handcards)
                    if(ptr->getElement()=="wind" && ptr->getType()=="attack")
                    {
                        actions.append(tr("2.������"));
                        break;
                    }
            }
            if(!onceUsed2 && dataInterface->getMyself()->getEnergy()>0 &&usedAttack &&!actions.contains(tr("3.��Ӱ"))){
                actions.append(tr("3.��Ӱ"));
            }
            if(actions.size()==0)
                decisionArea->disable(0);
            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();

            state=42;

        }
        break;
    }
}
