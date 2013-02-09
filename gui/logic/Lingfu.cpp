
#include "LingFu.h"




LingFu::LingFu()
{
    makeConnection();
    setMyRole(this);

    Button *leiMing,*fengXing,*checkCover;
    leiMing=new Button(3,tr("���-����"));
    fengXing = new Button(4,tr("���-����"));

    buttonArea->addButton(leiMing);
    buttonArea->addButton(fengXing);
    //buttonArea->addButton(checkCover);
    connect(leiMing,SIGNAL(buttonSelected(int)),this,SLOT(leiMing()));
    connect(fengXing,SIGNAL(buttonSelected(int)),this,SLOT(fengXing()));

    checkCover = new Button(10,tr("�鿴����"));
    buttonArea->addOutsideTurnButton(checkCover);

    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));


}

void LingFu::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    //���
    if (handArea->checkElement("thunder"))
        buttonArea->enable(3);
    if (handArea->checkElement("wind"))
        buttonArea->enable(4);

    unactionalCheck();
}



void LingFu::onOkClicked()
{
    Role::onOkClicked();

    QString command;
    QString sourceID;
    QString targetID;
    QString text,cardID;
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    switch(state)
    {
    //����
    case 1801:
        command = "1801;1;";
        command += QString::number(selectedCards[0]->getID()) + ";";
        handArea->removeCardItem(selectedCards[0]);
        coverArea->addCardItem(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    //���
    case 1802:
        command="1802;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";";
        targetID = QString::number(selectedPlayers[1]->getID());
        command += targetID + ";" + sourceID + ";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1803:
        command="1803;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";";
        targetID = QString::number(selectedPlayers[1]->getID());
        command += targetID + ";" + sourceID + ";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    //��������
    case 1804:
        command = "1804;1;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+="0;";
        else
            command+="1;";
        emit sendCommand(command);
        gui->reset();
        break;
    //�ٹ�ҹ��
    case 1805:
        command = "1805;1;";
        cardID = QString::number(selectedCoverCards[0]->getID());
        command += cardID + ";";
        emit sendCommand(command);
        coverArea->removeCardItem(selectedCoverCards[0]);
        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    //�ٹ�ҹ��-ѯ���Ƿ�չʾ������
    case 18050201:
        baiGuiYeXing3(true);
        break;
    //�ٹ�ҹ��-(��)ѡ��Ŀ��
    case 180502:
        command = "180502;";
        command += QString::number(selectedPlayers.count()-1) + ";" + QString::number(selectedPlayers[0]->getID()) + ";";
        if(selectedPlayers.count() > 1)
            command += QString::number(selectedPlayers[1]->getID()) + ";";
        emit sendCommand(command);
//        coverArea->removeCardItem(selectedCoverCards[0]);
//        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    //�ٹ�ҹ��-���գ�ѡ��Ŀ��
    case 180504:
        command = "180504;" + QString::number(selectedPlayers[0]->getID()) + ";";
//        coverArea->removeCardItem(selectedCoverCards[0]);
//        coverArea->reset();

        emit sendCommand(command);


        gui->showCoverArea(false);
        gui->reset();
        break;
    }
}

void LingFu::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)


    {    

    //����
    case 1801:
        command = "1801;0;;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1:
    //���
    case 1802:
    case 1803:
        normal();
        break;
    //��������
    case 1804:
        command = "1804;0;;";
        emit sendCommand(command);
        gui->reset();
        break;
    //�ٹ�ҹ��
    case 1805:
        command = "1805;0;";
        emit sendCommand(command);
        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    //�ٹ�ҹ��-ѯ���Ƿ�չʾ������
    case 18050201:
        baiGuiYeXing3(false);
        break;

    }
}

void LingFu::leiMing()
{
    state=1802;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement(tr("thunder"));
}

void LingFu::fengXing()
{
    state=1803;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement(tr("wind"));
}

void LingFu::nianZhou()
{
    state=1801;
    tipArea->setMsg(tr("�Ƿ񷢶����䣿"));
    handArea->setQuota(1);
    handArea->enableAll();
    decisionArea->enable(1);
}

void LingFu::lingLiBengJie()
{
    state=1804;
    tipArea->setMsg(tr("�Ƿ񷢶��������⣿"));


    decisionArea->enable(0);

    if(dataInterface->getMyself()->getCrystal()>=1)
    {
        tipArea->addBoxItem(tr("1.ˮ��"));
        decisionArea->enable(1);
    }
    if(dataInterface->getMyself()->getGem()>=1)
    {
        tipArea->addBoxItem(tr("2.��ʯ"));
        decisionArea->enable(1);
    }
    tipArea->showBox();
}

void LingFu::baiGuiYeXing()
{
    state = 1805;
    tipArea->setMsg(tr("�Ƿ񷢶��ٹ�ҹ�У�Ҫ������ѡ������"));

    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void LingFu::baiGuiYeXing2()
{
    state = 18050201;
    tipArea->setMsg(tr("�Ƿ�չʾ������������"));

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void LingFu::baiGuiYeXing3(bool show)
{
    state = 180502;
    if(show)
    {
        tipArea->setMsg("��ѡ��������ɫ�����˺���������ɫ����1�㷨���˺�");
        playerArea->setQuota(2);
    }
    else
    {
        tipArea->setMsg("��ѡ��1����ɫ���������1�㷨���˺�");
        playerArea->setQuota(1);
    }

    playerArea->enableAll();

    decisionArea->reset();
}

void LingFu::baiGuiYeXing4()
{
    state = 180504;
    tipArea->setMsg("��ѡ��1����ɫ���������1�㷨���˺�");
    playerArea->setQuota(1);
    playerArea->enableAll();
    decisionArea->reset();
}

void LingFu::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("����"))
        nianZhou();
    else if(skill==tr("��������"))
        lingLiBengJie();
    else if(skill == tr("�ٹ�ҹ��"))
        baiGuiYeXing();
}

void LingFu::coverCardAnalyse()
{
    QList<Card*> selectedCoverCards = this->coverArea->getSelectedCards();
    switch(state)
    {
    //�ٹ�ҹ��
    case 1805:
        decisionArea->enable(0);
    }
}

void LingFu::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {

//����
    case 1801:
        decisionArea->enable(0);
        break;
//����˫��
    case 1802:
    case 1803:
        playerArea->enableAll();
        break;

    }
}

void LingFu::decipher(QString command)
{
    Role::decipher(command);
    this->command=command;
    QStringList arg=command.split(';');
    QStringList cardIDList;

    switch(arg[0].toInt())
    {
    //�ٹ�ҹ��-������-չʾ��Ŀ��ѯ��
    case 180501:
        this->baiGuiYeXing2();
        break;
    //�ٹ�ҹ��-��ͨ����-Ŀ��ѯ��
    case 180503:
        this->baiGuiYeXing4();
        break;
    }

}
