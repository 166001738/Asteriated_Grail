#ifndef MAIN_H
#define MAIN_H
#include <QtGui>

#include "PlayerEntity.h"
#include "CardEntity.h"
#include "MyThread.h"

#include <QSemaphore>
#include <QMutex>
#include <QList>
#include <QDebug>
#include <iostream>

#define BUFFERSIZE 10

#define RED  1
#define BLUE 0

//Ӧս�ж���ע��BLOCKEDָʥ���Ӧս��HITָ���л�ʥ�ܣ���server�ж���
#define REPLYBATTLE  0
#define BLOCKED      1
#define HIT          2

//�˺�����
#define NOTHARMED  0
#define ATTACKHARM 1
#define MAGICHARM  2

//�ж����
#define ATTACK  1
#define ATTACKSKILL 10
#define MAGIC   2
#define SPECIAL 3
#define ATTACKORMAGIC 4
#define ALLACTION 5
#define FINISH -1
//���������ͨ���޷�Ӧս���ض�����
#define NORMAL  0
#define NOREPLY 1
#define NOMISS  2

//�����ж�
#define BUY     0
#define SYNTHESIZE 1
#define EXTRACT    2

//ͨѶЭ���
#define COMMONMAGIC 0
#define BEGINNOTICE 2
#define TURNBEGINNOTICE 3
#define ACTIONCOMMAND 4
#define ASKFORREBAT 5
#define REBATCOMMAND 6
#define ASKFORDISCARD 7
#define DISCARDCOMMAND 8
#define DRAWNOTICE 9
#define RESHUFFLE 10
#define MORALENOTICE 11
#define ENDNOTICE 12
#define DISCARDNOTICE 13
#define HITNOTICE 14
#define STONENOTICE 15
#define READYBEGIN 16
#define CUPNOTICE 17
#define ENERGYNOTICE 18
#define MOVECARDNOTICE 19
#define ATTACKHURTNOTICE 20
#define MAGICHURTNOTICE 21
#define ASKFORWEAK 22
#define WEAKCOMMAND 23
#define WEAKNOTICE 24
#define SHIELDNOTICE 25
#define ASKFORMISSILE 26
#define MISSILECOMMAND 27
#define USECARDNOTICE 28
#define ASKFORACTION 29
#define CROSSCHANGENOTICE 32
#define ASKFORCROSS 33
#define ANSFORCROSS 34
#define ASKFORSKILL 35
#define ANSFORSKILL 36
#define CHARACTERNOTICE 37
#define UNACTIONAL 30
#define UNACTIONALNOTICE 31
#define NOTICE 38

//���������＼�����
//��֮Ů��
#define BOWLADY_ANSFORTRANSFIXTION 301
#define BOWLADY_TRAP 302
#define BOWLADY_SNIPE 303

#define TOQSTR(x)  QString::number(x)
#define CARDSUM 150

//��ͨѶЭ��ĸ�ʽ���Ӹ�QString
QString combMessage(QString item1,QString item2 = (QChar)0,QString item3 = (QChar)0,QString item4 = (QChar)0,QString item5 = (QChar)0,QString item6 = (QChar)0,QString item7 = (QChar)0);

//������,�༭����ͨѶ��Ϣ
class Coder:public QObject
{
    Q_OBJECT
public:
    Coder():QObject(){}
    void beginNotice(QString seatCode){emit this->sendMessage(-1,"2;" + seatCode + ";");}
    void turnBegineNotice(int ID){emit this->sendMessage(-1,"3;" + TOQSTR(ID) + ";");}
    void askForReBat(int type,int cardID,int dstID,int srcID){emit this->sendMessage(-1,combMessage("5",TOQSTR(type),TOQSTR(cardID),TOQSTR(dstID),TOQSTR(srcID)));}
    void askForDiscard(int ID,int sum,QString show){emit this->sendMessage(-1,combMessage("7",TOQSTR(ID),TOQSTR(sum),show));}
    void askForDiscover(int ID, int sum,QString show){emit this->sendMessage(-1,combMessage("49",TOQSTR(ID),TOQSTR(sum),show));}
    void drawNotice(int ID,int sum,QList<CardEntity*> cards);
    void reshuffleNotice(int howManyNew){emit this->sendMessage(-1,combMessage("10",TOQSTR(howManyNew)));}
    void moraleNotice(int color,int value){emit this->sendMessage(-1,combMessage("11",TOQSTR(color),TOQSTR(value)));}
    void endNotice(int winColor){emit this->sendMessage(-1,"12;" + TOQSTR(winColor) + ";");}
    void discardNotice(int ID,int sum,QString show,QList<CardEntity*> cards);
    void hitNotice(int result,int isActiveAttack,int dstID,int srcID){emit this->sendMessage(-1,combMessage("14",TOQSTR(result),TOQSTR(isActiveAttack),TOQSTR(dstID),TOQSTR(srcID)));}
    void stoneNotice(int color,int gem,int crystal){emit this->sendMessage(-1,combMessage("15",TOQSTR(color),TOQSTR(gem),TOQSTR(crystal)));}
    void cupNotice(int color,int cup){emit this->sendMessage(-1,combMessage("17",TOQSTR(color),TOQSTR(cup)));}
    void energyNotice(int ID,int gem,int crystal){emit this->sendMessage(-1,combMessage("18",TOQSTR(ID),TOQSTR(gem),TOQSTR(crystal)));}
    void moveCardNotice(int sum,QList<CardEntity*> cards,int srcID,int srcArea,int dstID,int dstArea);
    void getCardNotice(int sum,QList<CardEntity*> cards,int dstID,bool show);
    void attackHurtNotice(int dstID,int sum,int srcID){emit this->sendMessage(-1,combMessage("20",TOQSTR(dstID),TOQSTR(sum),TOQSTR(srcID)));}
    void magicHurtNotice(int dstID,int sum,int srcID,QString reason){emit this->sendMessage(-1,combMessage("21",TOQSTR(dstID),TOQSTR(sum),TOQSTR(srcID),tr(reason.toStdString().c_str())));}
    void askForWeak(int ID,int howMany){emit this->sendMessage(-1,"22;" + TOQSTR(ID) + ";"+ TOQSTR(howMany) + ";");}
    void weakNotice(int ID,int act,int howMany=3){emit this->sendMessage(-1,combMessage("24",TOQSTR(ID),TOQSTR(act),TOQSTR(howMany)));}
    void shieldNotic(int ID){emit this->sendMessage(-1,"25;" + TOQSTR(ID) + ";");}
    void askForMissile(int dstID,int srcID,int hurtSum,int nextID){emit this->sendMessage(-1,combMessage("26",TOQSTR(dstID),TOQSTR(srcID),TOQSTR(hurtSum),TOQSTR(nextID)));}
    void useCardNotice(QList<CardEntity*> cards,int dstID,int srcID, int realCard=1);
    void askForAction(int playerID,int actionTypeAllowed,bool acted);
    void askForAdditionalAction(int playerID){emit this->sendMessage(-1,"42;"+TOQSTR(playerID)+";");}
    void crossChangeNotice(int playerID,int newValue){emit this->sendMessage(-1,combMessage(TOQSTR(CROSSCHANGENOTICE),TOQSTR(playerID),TOQSTR(newValue)));}
    void askForCross(int playerID,int hurtPoint,int type, int crossAvailable){emit this->sendMessage(-1,combMessage(TOQSTR(ASKFORCROSS),TOQSTR(playerID),TOQSTR(hurtPoint),TOQSTR(type),TOQSTR(crossAvailable)));}
    void askForSkill(int playerID,QString content,QString args=(QChar)0){emit this->sendMessage(-1,combMessage(TOQSTR(ASKFORSKILL),TOQSTR(playerID),tr(content.toStdString().c_str()),args));}
    void roleNotice(int playerID,int roleID,int show=0){emit this->sendMessage(-1,combMessage(TOQSTR(CHARACTERNOTICE),TOQSTR(playerID),TOQSTR(roleID),TOQSTR(show)));}
    void unactionalNotice(int playerID){emit this->sendMessage(-1,combMessage(TOQSTR(UNACTIONALNOTICE),TOQSTR(playerID)));}
    void notice(QString content){emit this->sendMessage(-1,combMessage(TOQSTR(NOTICE),tr(content.toStdString().c_str())));}
    void askForDiscardMagic(int ID){emit this->sendMessage(-1,combMessage("850",TOQSTR(ID)));}
    void askToGiveCard(int ID,int n){emit this->sendMessage(-1,combMessage("750",TOQSTR(ID),TOQSTR(n)));}
    void askForChongYing(int ID,int color){emit this->sendMessage(-1,combMessage("2950",TOQSTR(ID),TOQSTR(color)));}
    void handcardMaxNotice(int ID,int howMany){emit this->sendMessage(-1,combMessage("40",TOQSTR(ID),TOQSTR(howMany)));}
    void tapNotice(int ID,int flag,QString content){emit this->sendMessage(-1,combMessage("39",TOQSTR(ID),TOQSTR(flag),tr(content.toStdString().c_str())));}
    void specialNotice(int ID,int type,int flag){emit this->sendMessage(-1,combMessage("43",TOQSTR(ID),TOQSTR(type),TOQSTR(flag)));}
    void tokenNotice(int ID,int tokenID,int howMany){emit this->sendMessage(-1,combMessage("45",TOQSTR(ID),TOQSTR(tokenID),TOQSTR(howMany)));}
    void askForRolePick(int ID,int howMany,QList<int> *roles);
    void coverCardNotice(int playerID,int howMany,QList<CardEntity*> cards,bool remove,bool show);
    void askForSkillNumber(int playerID,int skillNum){emit this->sendMessage(playerID,combMessage(TOQSTR(skillNum)));}
    void optionalRoleNotice(int num, QList<int> *roles);
    void askForBan(int ID);
    void banNotice(int ID, int role);
    void askForPick(int ID);
    void pickNotice(int ID, int role);
signals:
    void sendMessage(int,QString);

};
//ս����
class TeamArea
{
public:
    TeamArea(){this->initialTeam();}
    void initialTeam();
    void setMorale(int color,int value);
    void setGem(int color,int value);
    void setCrystal(int color,int value);
    void setCup(int color,int value);
    int getMorale(int color){return (color == RED)?moraleRED:moraleBLUE;}
    int getGem(int color){return (color == RED)?gemRED:gemBLUE;}
    int getCrystal(int color){return (color == RED)?crystalRED:crystalBLUE;}
    int getCup(int color){return (color == RED)?cupRED:cupBLUE;}

private:

    int moraleRED,moraleBLUE;
    int gemRED,gemBLUE;
    int crystalRED,crystalBLUE;
    int cupRED,cupBLUE;
};

//ս���ṹ��
struct BatInfor
{
    //�ж����ʣ���Ӧս�У�REPLYBATTLE��ʾӦս��BLOCKED��ʾʥ���赲��HIT��ʾ�޻�Ӧ
    //�������ж��У�ATTACK��ʾ����Ϊ�����ƣ�MAGIC��ʾΪ�������������������＼�ܣ���FINISH��ʾ�����ж�
    int reply;

    //����ʹ�õĿ��ƣ��������ƻ�ʥ��
    int CardID;

    //��Դ��Ŀ�����ID��ע���������Դ��Ŀ����ָ���Ƶ���Դ��Ŀ��
    int srcID;
    int dstID;
    //������Ϣ
    //��Ҫ���ڷ����ͼ��ܡ�һ��infor1��ż��ܴ��ţ���BOWLADY_TRAP�����������ڴ�ż�����Ϣ���������ġ�������ĵȣ�
    int infor1;
    int infor2;
    int infor3;
    int infor4;
    int infor5;
    QString inforstr;
    QString inforstrp;
};

class BackgroundEngine;
class Server;
class TestUIex;

//������
class MyRoom:public QObject
{
    Q_OBJECT
public:
    MyRoom(Server*);
    ~MyRoom(){}
public slots:
    void makeConnect(int);
private:
    MyThread *engineThr;
    MyThread *listenerThr;
    BackgroundEngine *backEngine;
    Server *serverModule;
    TestUIex *testUI;

};

extern TeamArea teamArea;
extern Coder coder;
extern QList<CardEntity*> cardList;
extern int playerSum;
CardEntity* getCardByID(int ID);


#endif // MAIN_H
