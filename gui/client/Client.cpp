#include <QThread>
#include "client/Client.h"
#include "widget/GUI.h"
#include "logic/Logic.h"
Client::Client()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(logic,SIGNAL(gameStart()),this,SIGNAL(readyToStart()));
    connect(logic,SIGNAL(sendCommand(QString)),this,SLOT(sendMessage(QString)));
    connect(this,SIGNAL(getMessage(QString)),logic,SLOT(getCommand(QString)));
    logic->setClient(this);
}
void Client::link(QString addr,int port)
{
    abort();
    connectToHost(addr,port);
}
void Client::readMessage()
{
    QDataStream in(this);
    quint16 nextBlockSize;
    QString message;
    in.setVersion(QDataStream::Qt_4_7);
    while(1){
    if(bytesAvailable() < (int)sizeof(quint16)) return;
    in >> nextBlockSize;
    if(bytesAvailable() < nextBlockSize) return;
    //���û�еõ�ȫ�������ݣ��򷵻أ�������������
    in >> message;
    //�����յ������ݴ�ŵ�������
    emit getMessage(message);
    }
}
void Client::sendMessage(QString message)
{
    QByteArray block; //�����ݴ�����Ҫ���͵�����

    QDataStream out(&block,QIODevice::WriteOnly);

    //ʹ��������д������

    out.setVersion(QDataStream::Qt_4_7);

    //�����������İ汾���ͻ��˺ͷ�������ʹ�õİ汾Ҫ��ͬ

    out<<(quint16) 0;

    out<<message;

    out.device()->seek(0);
    out<<(quint16)(block.size() - sizeof(quint16));
    write(block);

}
