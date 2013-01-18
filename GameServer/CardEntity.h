#ifndef CARDENTITY_H
#define CARDENTITY_H

#include <QList>
#include <QStringList>
#include <QObject>

#define SHIELDCARD 0
#define POISONCARD 1
#define WEAKCARD 2
#define MISSILECARD 3
#define LIGHTCARD 4

#define NOTEXIST           0
#define PILE               1
#define DISCARDPILE        2
#define DISCARDPILECOVERED 3
#define HAND               4
#define EFFECT             5
#define COVERED            6

#define OWNERLESS            -1

extern QList<int> shieldCardID;
extern QList<int> weakCardID;
extern QList<int> poisonCardID;
extern QList<int> missileCardID;

//������
class CardEntity:public QObject
{
    Q_OBJECT
public:
    //����,��������ǰ���������Ϣ��һ��String
    CardEntity(QStringList *cardEntry);

    int getID();
    //��ȡ���ƶ��м���Ϣ
    int getHasSpeciality();
    //��ȡ����/�������
    QString getType();
    //��ȡ��������
    QString getElement();
    //��ȡ���Ƽ���Ѫ��ӽ��ʥ�������
    QString getProperty();
    //��ȡ�������ƣ�ע��������
    QString getName();
    //��ȡ����ͼƬ��ַ
    QString getSource();
    //��ȡ���ƶ��м��б�
    QStringList getSpecialityList();
    //������������תΪint
    int getMagicName(){return this->magicName;}
    void setMagicName(int name){this->magicName = name;}
    //��ȡʹ����
    int getSrcUser(){return this->srcUser;}
    //��ȡ���Ƶ�ǰ����λ�ã����ơ����ơ�Ч�������ƶѡ������ƶѡ������ƶѣ�
    int getPlace(){return this->place;}
    //��ȡ���Ƶ�ǰ����˭�����򣨰������ơ����ơ�Ч�����ȣ���A��Bʹ���ж����ж���srcUser��A��owner��B��
    int getOwner(){return this->owner;}

    void setPlace(int newPlace){this->place = newPlace;}
    void setOwner(int newOwner){this->owner = newOwner;}
    void setSrcUser(int srcID){this->srcUser = srcID;}
    //�˲鿨���Ƿ��ǻ���������
    int checkBasicMagic(QString cardName);
protected:
    int id;
    int hasSpeceiality;
    QString type;
    QString element;
    QString property;
    QString name;
    QStringList specialityList;
    QString source;
    int srcUser;
    int owner;
    int place;
    int magicName;
};

#endif // CARDENTITY_H
