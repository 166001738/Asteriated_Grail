#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H
#include "CardEntity.h"
#include <QObject>
#include "main.h"

#define NONCHARACTER 0
#define BLADEMASTER  1
#define BERSERKER    2
#define BOWLADY      3

class BackgroundEngine;

//�����
class PlayerEntity:public QObject
{
    Q_OBJECT
public:
    PlayerEntity(BackgroundEngine* engine,int id,int color);
    //�����źŲ�
    void makeConnection(BackgroundEngine* engine);
    //�������Ʋ���
    void addHandCards(QList<CardEntity*> newCard,int harmed,bool fromPile=true);
    //���ƴ���
    void cardsOverLoad(int harmed);
    //�Ƴ����Ʋ���
    void removeHandCards(QList<CardEntity*> oldCard,bool show,bool toDiscardPile = true);
    //���Ʋ���
    void giveHandCards(QList<CardEntity*> oldCard,PlayerEntity* to);

    //�趨���������Ƿ�����
    void setHandCardsMaxFixed(bool fixed, int howmany=6);
    //�������Ʊ仯
    void addHandCardsRange(int howMany);
    //��������
    void setCrossNum(int howMany, int atMost=-1);
    void setGem(int howMany);
    void setCrystal(int howMany);
    //���õ�ǰ�غ��Ƿ�Ϊ����һغ�
    void setYourTurn(bool yes);
    void setSeatNum(int num){this->seatNum = num;}
    int getSeatNum(){return this->seatNum;}
    //���ú���
    void setTap(bool tap){this->tap = tap;}
    void setToken(int id,int howMany){token[id]=howMany<=tokenMax[id]?howMany:tokenMax[id];}
    //������һ�����
    void setNext(PlayerEntity* nextPlayer){this->nextPlayer = nextPlayer;}
    //�ڸ����ǰ���ӻ���Ч��
    void addBasicEffect(CardEntity* effectCard){this->basicEffect << effectCard;}

    //�Ƴ�����Ч��
    bool removeBasicEffect(CardEntity* effect,int toWho=-1,int toWhere=DISCARDPILE);
    int getID();
    QString getName();
    int getHandCardMax();
    int getHandCardNum();
    int getCrossNum();
    int getCrossMax();
    int getGem();
    int getCrystal();
    int getEnergy();
    int getColor();
    QList<CardEntity*> getCoverCards(){return this->coverCards;}
    int getToken(int id){return token[id];}
    //��ȡ�������ǰ�Ļ���Ч����
    QList<CardEntity*> getBasicEffect(){return this->basicEffect;}
    //�����������
    int getStar(){return this->star;}
    PlayerEntity* getNext(){return this->nextPlayer;}
    QList<CardEntity*> getHandCards(){return this->handCards;}
    int getRoleID(){return characterID;}
    bool tapped(){return this->tap;}
    bool isHandCardsMaxFixed(){return this->handCardsMaxFixed;}
    bool getYourturn();
    //void setInfo(int roleID);
    //QString getInfo();
    //static void loseMorale(int num,int isRed);
    bool removeOneHandCard(CardEntity* card){return this->handCards.removeOne(card);}
    bool removeOneCoverCard(CardEntity* card){return this->coverCards.removeOne(card);}
    bool removeOneEffectCard(CardEntity* card){return this->basicEffect.removeOne(card);}
    void addCardsToCover(QList<CardEntity*> cards);
signals:
    //�����ź�
    void overLoadCardsSIG();
    //�������ź�
    void discardWithFaceUpSIG();
    //����ѯ���ź�(��ʵûʲô��)
    void askForOverLoadSIG(QList<CardEntity*> hand,int overNum);
    //���Ʊ仯�źţ���Ůʹ�ã�
    void handCardsChange(PlayerEntity* dst);
    //ʿ���½�ǰ��������ʿ��Ѫ����״̬��
    void beforeLoseMoralSIG(int harmed, int *howMany, PlayerEntity *dst);
    //ʿ���½��ź�
    void loseMoraleSIG(int harmed,int* howMany,PlayerEntity* dst);
    //��ʵʿ���½��źţ���Ů��������״̬ʹ�ã�
    void trueLoseMoraleSIG(int harmed, int* howMany, PlayerEntity* dst);
    //�������ź�
    void checkEndSIG();
    //����ͨѶ��Ϣ
    void sendMessageSIG(int,QString);
    //�ƶ����Ƶ�������(���ӵ�BackgroundEngine)
    void toDiscardPileSIG(QList<CardEntity*> cards,bool show);
    void showHandCards(QList<CardEntity*>,PlayerEntity*);


protected:
    int id;//���id
    int characterID;
    QString name;
    int handCardsMax;
    int handCardsRange;
    int handCardsMin;//��������֮��ʹ��


    int crossNum;
    int crossMax;
    int gem;
    int crystal;
    int energyMax;
    int color;
    float star;
    bool tap;//����״̬
    bool handCardsMaxFixed;//�Ƿ�������������
    bool yourTurn;
    int seatNum;
    int token[3];
    int tokenMax[3];
    PlayerEntity* nextPlayer;//�¼�
    //QList<Status*> status;
    //QString info;
    QList<CardEntity*> handCards;//����
    QList<CardEntity*> basicEffect;//����Ч����
    QList<CardEntity*> exclusiveEffect;//ר��Ч��
    QList<CardEntity*> coverCards;//������
    //��Ϸ���棬�������engine�ĸ����
    BackgroundEngine* engine;

};

#endif // PLAYERENTITY_H
