#ifndef SERVER_H
#define SERVER_H

#include "main.h"
#include <QtNetwork>
#include <QTcpSocket>

class messageBuffer
{
public:
    static void init();
    static void writeBatInfor(BatInfor content);
    static void writeCardInfor(QList<int> cardID);
    static void writeInfor(int infor);
    static void writeMsg(QString);
    static QString readMsg();
    static BatInfor readBatInfor();
    static QList<CardEntity*> readCardID(int sum);
    static int readInfor();    
private:
    static QSemaphore cardBufferFree;
    static QSemaphore cardBufferUsed;
    static int cardBuffer[];
    static int cardBufferTop,cardBufferBottom;

    static QSemaphore inforBufferReadLocker;
    static QSemaphore inforBufferWriteLocker;
    static QSemaphore msgBufferReadLocker;
    static QSemaphore msgBufferWriteLocker;
    static int inforBuffer;

    static QSemaphore batBufferReadLocker;
    static QSemaphore batBufferWriteLocker;
    static BatInfor batBuffer;
    static QString msg;

};

//�ͻ����׽���
class ClientSocket : public QTcpSocket
{
    Q_OBJECT
    friend class Server;
public:
    explicit ClientSocket(QObject *parent = 0);
    int getID(){return id;}
signals:
    void getMessage(int id,QString message);
    void disconnectedSIN(ClientSocket*);
public slots:
    void readMessage();
    void onDisconnected(){emit disconnectedSIN(this);}
private:
    int id;
};

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0, bool isIPv4=0, int port=0);
    void toDisplayPublic(QString content){emit this->toDisplay(content);}

signals:
    void gameStartSIG();
    void seatArrangeSIG();
    void characterNotice();
    void toDisplay(QString);
public slots:
    //������
    void decoder(int id,QString message);
    //���͸�ģ���ͨѶ��Ϣ
    void sendMessage(int num,QString message);
    void onDisconnectedSIG(ClientSocket*);
protected:
    //��ȡ����
    void incomingConnection ( int socketDescriptor );
private:
    QList<ClientSocket*> clientSocketList;
    QList<bool> ready;
    QList<int> socketDescriptorList;
};
#endif // SERVER_H
