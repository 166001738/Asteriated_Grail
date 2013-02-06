#include "data/Player.h"
#include <QTextStream>
#include <QFile>
Player::Player(int ID, int isRed):id(ID),handCardsMax(6),crossNum(0),crossMax(2),energyMax(3),color(isRed),tap(0)
{
    gem=0;
    crystal=0;
    handCardsNum=0;
    tokenMax[1]=tokenMax[0]=token[1]=token[0]=0;
    for(int i=0;i<5;i++ )
        specials[i]=false;
}
void Player::setRole(int roleID)
{
    this->roleID=roleID;
    faceSource="role/"+QString::number(roleID)+".png";
    switch(roleID)
    {
    case 1:
        name=tr("[��ʥ]");
        break;
    case 2:
        name=tr("[��սʿ]");
        break;
    case 3:
        name=tr("[��֮Ů��]");
        break;
    case 4:
        name=tr("[��ӡʦ]");
        break;
    case 5:
        name=tr("[��ɱ��]");
        tapSource="resource/qianxing.png";
        break;
    case 6:
        name=tr("[ʥŮ]");
        tapSource="resource/lianmin.png";
        break;
    case 7:
        name=tr("[��ʹ]");
        break;
    case 8:
        name=tr("[ħ��ʦ]");
        break;
    case 9:
        name=tr("[ħ��]");
        tapSource="resource/anying.png";
        break;    
    case 10:
        name=tr("[ʥǹ]");
        crossMax=3;
        break;
    case 11:
        name=tr("[Ԫ��ʦ]");
        tokenName[0]=tr("Ԫ��");
        tokenMax[0]=3;
        break;
    case 12:
        name=tr("[ð�ռ�]");
        break;
    case 13:
        name=tr("[���鷨ʦ]");
        crossMax = 5;
        break;
    case 14:
        name=tr("[�ٲ���]");
        tapSource="resource/shenpan.png";
        tokenName[0]=tr("����");
        tokenMax[0]=4;
        crystal=2;
        break;
    case 15:
        name=tr("[���]");
        crossMax=6;
        break;
    case 16:
        name=tr("[��ʦ]");
        tapSource="resource/qidao.png";
        tokenName[0]=tr("����");
        tokenMax[0]=3;
        break;
    case 17:
        name=tr("[����]");
        energyMax=4;
        break;
    case 20:
        name=tr("[�񶷼�]");
        tapSource="resource/baishi.png";
        tokenName[0]=tr("����");
        tokenMax[0]=6;
        break;
    case 21:
        name=tr("[����]");
        tapSource="resource/jingpilijie.png";
        tokenName[0]=tr("ŭ��");
        tokenMax[0]=4;
        tokenName[1]=tr("֪��");
        tokenMax[1]=4;
        crystal=2;
        break;
    case 23:
        name=tr("[��Ů]");
        tapSource="resource/liuxue.png";
        break;
    }
}

void Player::addStatus(int type,Card* card)
{
    Status*status=new Status(type,card);
    statusList<<status;
    emit addStatusSIG(status);
}

void Player::removeStatus(Card* card)
{
    int i;
    for(i=0;i<statusList.count();i++)
        if(statusList.at(i)->card==card)
        {
            emit removeStatusSIG(statusList.at(i));
            statusList.removeAt(i);
        }
}

bool Player::checkStatus(int type)
{
    foreach(Status* ptr,statusList)
        if(ptr->type==type)
            return 1;
    return 0;
}

bool Player::checkStatus(QString status)
{
    foreach(Status* ptr,statusList)
        if(ptr->getCard()->getSpecialityList().contains(status))
            return 1;
    return 0;
}

void Player::setHandCardsMax(int howMany)
{
    handCardsMax=howMany;
}
void Player::setCrossNum(int howMany)
{
    crossNum=howMany;
}

void Player::setGem(int howMany)
{
    gem=howMany;
}

void Player::setCrystal(int howMany)
{
    crystal=howMany;
}

void Player::setEnergyMax(int howMany)
{
    energyMax=howMany;
}

void Player::setSpecial(int type,bool flag)
{
    specials[type]=flag;
}

int Player::getID()
{
    return id;
}

QString Player::getName()
{
    return name;
}

int Player::getHandCardMax()
{
    return handCardsMax;
}

int Player::getHandCardNum()
{
    return handCardsNum;
}

int Player::getCrossNum()
{
    return crossNum;
}
int Player::getCrossMax()
{
    return crossMax;
}

int Player::getGem()
{
    return gem;
}

int Player::getCrystal()
{
    return crystal;
}

int Player::getEnergy()
{
    return gem+crystal;
}

int Player::getColor()
{
    return color;
}

int Player::getEnergyMax()
{
    return energyMax;
}

