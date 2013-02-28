#include "client/ClientUI.h"
#include "client/ui_ClientUI.h"
#include "data/DataInterface.h"
ClientUI::ClientUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientUI)
{
    tcpSocket=new Client;
    ui->setupUi(this);

    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(link()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(tcpSocket,SIGNAL(readyToStart()),this,SLOT(startGame()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket,SIGNAL(getMessage(QString)),this,SLOT(showMessage(QString)));


    ui->addr->setText("127.0.0.1");

    //ui->addr->setText("2001:5C0:1000:B::7C63");
    ui->port->setText("50000");
    ui->board->setText(tr("�����ӷ���������Ҫ���ӣ�����ѡ�����������"));
    ui->comboBox->addItem(tr("���"));
    ui->comboBox->addItem(tr("���"));
    ui->comboBox->addItem(tr("����"));
}

ClientUI::~ClientUI()
{
    delete ui;
}
void ClientUI::showMessage(QString msg)
{
    QStringList arg=msg.split(';');
    switch (arg[0].toInt())
    {
    case 1:
        ui->board->append(tr("���ID�ǣ�")+arg[1]);
        myID=arg[1].toInt();
        break;
    case 2:
        break;
    default:
        ui->board->append(msg);
    }
}

void ClientUI::startGame()
{
    ui->board->append(tr("�뿪ʼ��Ϸ"));
    ui->startButton->setEnabled(1);
    disconnect(this);
}

void ClientUI::link()
{
    tcpSocket->link(ui->addr->text(),ui->port->text().toInt());
    ui->connectButton->setEnabled(0);
    if(ui->comboBox->currentIndex()==1)
        tcpSocket->sendMessage("0;1;");
    else if(ui->comboBox->currentIndex()==2)
        tcpSocket->sendMessage("0;0;");
}

void ClientUI::displayError(QAbstractSocket::SocketError)
{
     showMessage(tcpSocket->errorString()); //���������Ϣ
}
