#ifndef BACKGROUNDENGINE_H
#define BACKGROUNDENGINE_H
#include <QObject>
#include "main.h"
#include "PlayerEntity.h"
#include "CardEntity.h"

//class PlayerEntity;

//�˺��ṹ��
struct Harm
{
    int harmPoint;//�˺�����
    int type;//ATTACK�����˺���MAGIC�����˺�
};

//��Ϸ��̨��
class BackgroundEngine:public QObject
{
    Q_OBJECT
public:
    BackgroundEngine();
    void randomize(QList<int> *queue);
    //��ȡ��һ�����
    PlayerEntity* getNext(PlayerEntity* player){return player->getNext();}
    //��ȡǰһ�����
    PlayerEntity* getFront(PlayerEntity* player);
    //��������
    void weakProcess(PlayerEntity* player, int howMany=3);
    //ħ������
    void missileProcess(CardEntity* card,int src,int dst);
    //ħ������
    void missilePass(bool rightOrder,int dst,int src,bool *passed,int missilePoint);
    //���ƺ���
    void drawCards(int num,int harmed,class PlayerEntity *player);
    //��ʼ��
    void initial();
    //ϴ��
    void shuffle(bool reShuffle);
    //�����������
    void clearData();
    //�ж�����
    void posionProcess(PlayerEntity* player,CardEntity* card);
    //���û���Ч��
    void effectApply(CardEntity* card,PlayerEntity* user,PlayerEntity* dst);
    //�غϿ�ʼʱ����Ƿ���Ч�������ĺ���
    void checkEffect(PlayerEntity* player);
    //�趨��ǰ���
    void setCurrentPlayer(class PlayerEntity* currentPlayer){this->currentPlayer = currentPlayer;}
    //��ȡ��ǰ���
    PlayerEntity* getCurrentPlayer(){return this->currentPlayer;}
    //�غϿ�ʼ�׶κ���
    void turnBeginPhase(PlayerEntity* currentPlayer);
    //�ж��׶κ���
    void actionPhase();
    //����ָ�������ж�����,����ΪattackLeft,magicLeft,specialLeft,attackOrMagicLeft,actionLeft֮һ
    void addActionNum(int kind);
    //void minusActionNum(int kind);
    //�غϽ����׶κ���
    void turnEndPhase();
    //����ʣ���ж�����
    void resetActionNum(){
                        this->attackLeft = 0;
                        this->magicLeft = 0;
                        this->specialLeft = 0;
                        this->attackOrMagicLeft = 0;
                        this->actionLeft = 1;
                        }
    //ĳ�����ж������,����ʣ���ж�����.����ΪATTACK,MAGIC,SPECIAL֮һ
    void acted(int kind);
    //������ʹ�ÿ���ʱ���ô˺���.���ܴ���"չʾĳ��ʱ"֮����ź�
    void useCard(QList<CardEntity*> cards,PlayerEntity* user,PlayerEntity* dst = NULL,bool stay = false,int realCard=1);
    //�ӿ����ϻ�û����˺�����.Ŀǰֻ�ܴӹ������ϻ������.
    Harm getHarmFromCard(CardEntity* card);//�ӿ����ϻ�û������˺���Ϣ
    //����ID�������
    PlayerEntity* getPlayerByID(int ID);
    //���º����жϵ�ǰ����Ƿ���Ȩ������Ӧ�ж�
    bool allowAttack();
    bool allowMagic();
    bool allowSpecial();
    //��ȡ�������
    int getPlayerNum(){return this->playerNum;}
    //�����������
    void setPlayerNum(int num){this->playerNum = num;}



    bool canAct();//�ж��Ƿ�����ж�,�����
    void reDraw();//�޷��ж�������.�����

    //����Ϊ�˺�ʱ���ắ��
    void timeLine1(CardEntity* attackCard,PlayerEntity* src,PlayerEntity* dst,bool isActiveAttack);
    void timeLine2(CardEntity* harmCard,PlayerEntity* src,PlayerEntity* dst,bool isActiveAttack,int attackType,Harm harm);
    void timeLine3(Harm harm, PlayerEntity *src,PlayerEntity *dst,QString magicReason = "");
    void timeLine4(Harm harm,PlayerEntity *src,PlayerEntity *dst);
    void timeLine5(Harm harm,PlayerEntity *src,PlayerEntity *dst,int cross);
    void timeLine6(Harm harm,PlayerEntity *src,PlayerEntity *dst);

    //�����ƴ�����
    void useMagicCard(int cardID,int srcID,int dstID = -1);
    //����Ϊ�����ú���
    void showTest();
    PlayerEntity* setRole(int roleID, BackgroundEngine *engine, int id, int color);

public slots:
    //�����ò�
    void testSLOT(){qDebug()<<"test by backEngine";}
    //��Ϸ��ʼ��
    void gameStart();
    //λ�����в�
    void seatPrearrange(int id, bool isRed);
    void seatArrange();
    void role3Pick1();
    void role3Pick1Reply(int id,int roleID);
    void roleRandom();
    void seatPostarrange();
    //��Ϸ��������
    bool checkEnd();
    //���Ƽ������ƶѲ�
    void toDiscardPileSLOT(QList<CardEntity*> cards,bool show);

signals:
    //���¸�ʱ�����ź�
    void timeLine1ProSIG(QList<void*> args);
    void timeLine1SIG(QList<void*> args);
    //ʱ����2-δ����
    void timeLine2missedSIG(QList<void*> args);
    //ʱ����2-����
    void timeLine2hitSIG(QList<void*> args);
    void timeLine3SIG(QList<void*> args);
    void timeLine4SIG(QList<void*> args);
    void timeLine5SIG(QList<void*> args);
    void timeLine6SIG(QList<void*> args);
    void timeLine6DrawedSIG(QList<void*> args);
    //�غϿ�ʼ�׶��ź�
    void turnBeginPhaseSIG(QList<void*> args);
    //�غϽ����׶��ź�
    void turnEndPhaseSIG(PlayerEntity*);
    //�ж��׶��ź�
    void weakSIG(PlayerEntity*, bool*);
    void actionPhaseSIG(QList<void*> args);
    void additonalActionSIG(QList<void*> args);
    void tiaoXinPhaseSIG(PlayerEntity*, int*,bool*);
    //������Ҷ����ź�-�۵��ź�
    void makePlayerConnectSIG(int);
    //����ͨѶ��Ϣ
    void sendMessageSIG(int playerID,QString content);
    //ʹ�÷��������ź�
    void skillMagic(QList<void*> args);
    void skillAttack(QList<void*> args);
    void skillSpecial(QList<void*> args);
    void shieldSIG(QList<void*> args);
    void showHandCards(QList<CardEntity*> cards,PlayerEntity* user);

    //���Ʊ仯�źţ���Ůʹ�ã�
    void handCardsChange(PlayerEntity* dst);

    //�����ź���������
    //ѯ��Ӧս�����ź�
    void askForReply(QList<CardEntity*> hand,QString element,int userID);
    //ѯ��������Ӧ�ź�
    void askForHeal(Harm harm, PlayerEntity* src, PlayerEntity* dst, int *crossAvailable);
    //ѯ���ж��ź�
    void askForActSIG(QList<CardEntity*> hand,int currentID);

    //����Ϊ�����ж������ź�
    void attackFinishSIG(QList<void*> args);
    void magicFinishSIG(QList<void*> args);
    void specialFinishSIG(QList<void*> args);
    //�����ж���Чǰ
    void beforeMagicSIG(QList<void*> args);
    //ʿ���½�ǰ��������ʿ��Ѫ����״̬��
    void beforeLoseMoralSIG(int harmed, int *howMany, PlayerEntity *dst);
    //ʿ���½��ź�
    void loseMoraleHeChengSIG(int harmed, int *howMany, PlayerEntity *dst);
    void loseMoraleSIG(int harmed,int* howMany,PlayerEntity* dst);
    //��ʵʿ���½��źţ���Ů��������״̬ʹ�á���곤�ƻ�ʹ�á�ħǹ��Ӱ�ǳ���������
    void trueLoseMoraleSIG(int harmed, int* howMany, PlayerEntity* dst);
    void toInforDisplay(QString content);
    void usedShield(int userID);
private:
    //�ƶ������ƶ�
    QList<CardEntity*> pile,discardPile,discardPileCovered;
    //�����
    QList<PlayerEntity*> playerList;
    PlayerEntity *currentPlayer;
    //�����ж��������ж��������ж������������ж��������ж���ʣ�����
    int attackLeft,magicLeft,specialLeft,attackOrMagicLeft,actionLeft;
    int playerNum;
    //��Ϸ�����б��
    bool playing;
    QList<int> red,blue;
    QString queue;
    QList<int> roles;
};

#endif // BACKGROUNDENGINE_H
