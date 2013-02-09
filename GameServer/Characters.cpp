#include "Characters.h"
#include "BackgroundEngine.h"
#include "Server.h"

Berserker::Berserker(BackgroundEngine *engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = BERSERKER;
    this->star = 3;
    this->makeConnection(engine);
}

void Berserker::wild(QList<void*> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;

    Harm *harm = (Harm*)args[2];
    if(harm->type != ATTACK)
        return;
    harm->harmPoint++;
    coder.notice("��սʿ�������񻯡�");
}

void Berserker::tear(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(this->getGem() <= 0)
        return;

    //�˴�ѯ���Ƿ�ʹ��
    coder.askForSkill(this->getID(),"˺��");
    if(messageBuffer::readInfor() == 0)
        return;

    coder.notice("��սʿ������˺�ѡ�");
    this->setGem(this->getGem()-1);
    Harm *harm = (Harm*)args[2];
    harm->harmPoint += 2;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
}

void Berserker::wildBladeAsk(QList<void*> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    this->wildBladeUsed = false;
    if(!*(bool*)args[4])
        return;
    CardEntity *card = (CardEntity*)args[3];
    if(!card->getHasSpeciality())
        return;
    if(card->getProperty() != tr("Ѫ"))
        return;
    if(!card->getSpecialityList().contains(tr("ѪӰ��")))
        return;
    int handNum = ((PlayerEntity*)args[1])->getHandCardNum();
    if(handNum != 2 && handNum != 3)
        return;
    coder.askForSkill(this->getID(),"ѪӰ��");
    if(messageBuffer::readInfor() == 1)
        this->wildBladeUsed = true;
    coder.notice("��սʿ������ѪӰ�񵶡�");
}

void Berserker::wildBlade(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    CardEntity *card = (CardEntity*)args[3];
    if(!card->getHasSpeciality())
        return;
    if(card->getProperty() != tr("Ѫ"))
        return;
    if(card->getSpecialityList().contains(tr("ѪӰ��")))
    {
        if(!this->wildBladeUsed)
            return;
        Harm *harm = (Harm*)args[2];
        switch(((PlayerEntity*)args[1])->getHandCards().length())
        {
        case 3:
            harm->harmPoint += 1;
            break;
        case 2:
            harm->harmPoint += 2;
        default:
            break;
        }
    }
}

void Berserker::roar(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    CardEntity *card = (CardEntity*)args[3];
    if(!card->getHasSpeciality())
        return;
    if(card->getProperty() != tr("Ѫ"))
        return;

    if(card->getSpecialityList().contains(tr("Ѫ������")))
    {
        if(((PlayerEntity*)args[1])->getCrossNum() == 2)
        {
            //ѯ���Ƿ񷢶�
            coder.askForSkill(this->getID(),"Ѫ������");
            if(messageBuffer::readInfor() == 0)
                return;
            coder.notice("��սʿ������Ѫ��������");
            *(int*)args[5] = NOMISS;
        }
    }
}

void Berserker::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(wild(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(tear(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(wildBladeAsk(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(wildBlade(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(roar(QList<void*>)));
}

BowLady::BowLady(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = BOWLADY;
    this->star = 3;
    this->makeConnection(engine);
}

void BowLady::thunderArrow(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(card->getElement() == tr("thunder"))
    {
        *((int*)args[5]) = NOREPLY;
        coder.notice("��֮Ů�񷢶����������");
    }
}

void BowLady::transfixtion(QList<void *> args)
{
    if(this !=  (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4] || handCards.size()==0)
        return;
    coder.askForSkill(this->getID(),"�ᴩ���");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;
    else
    {
        CardEntity *card = getCardByID(ans.CardID);
        QList<CardEntity*> cards;
        cards << card;
        this->removeHandCards(cards,true);
        coder.notice("��֮Ů�񷢶����ᴩ�����");
        coder.discardNotice(this->getID(),1,"y",cards);
        Harm transHurt;
        transHurt.harmPoint = 2;
        transHurt.type = MAGIC;
        engine->timeLine3(transHurt,this,(PlayerEntity*)args[1],"�ᴩ���");
    }
}

void BowLady::accurateShoot(QList<void *> args)
{
    //��׼���������timeLine1SIG(QList<void*>)�źţ�ͬ������Ӧ������ϸ�鿴�źŵĲ����б�
    //������Ȼ�Ǽ���Ƿ���ϼ��ܷ�������
    if(this !=  (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(card->getProperty() != tr("��"))
        return;
    if(card->getHasSpeciality() == 0)
        return;
    if(card->getSpecialityList().contains(tr("��׼���")))
    {
        //�˴�ʹ��35��ͨѶЭ��ѯ��client�Ƿ񷢶�����
        coder.askForSkill(this->getID(),"��׼���");
        //��ע��,messageBuffer::readInfor()��ӻ����ж�ȡ�ظ�.���Ǹ���������,ֱ�����ӻ����л�ûظ���Ϣ�Ż᷵��
        if(1 == messageBuffer::readInfor())
        {
            //����36��Э��,�ظ�"1"��ʾ��ҷ����˼���
            //ͨ���źŴ�������ָ���޸��˺�����
            Harm* harm = (Harm*)args[2];
            harm->harmPoint--;
            *(int*)args[5] = NOMISS;
            coder.notice("��֮Ů�񷢶�����׼�����");
        }
    }
}

void BowLady::trap(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != BOWLADY_TRAP)
        return;
    coder.notice("��֮Ů�񷢶����������塿");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    this->engine->useCard(cards,this,dst);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"��������");
}

void BowLady::snipe(QList<void *> args)
{
    BatInfor* magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != BOWLADY_SNIPE)
        return;
    if(this->getGem() + this->getCrystal() < 1)
        return;
    coder.notice("��֮Ů������"+QString::number(magic->dstID)+"�������ѻ���");
    switch(magic->infor2)
    {
    case 0:
        this->crystal--;
        break;
    case 1:
        this->gem--;
        break;
    default:
        break;
    }
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());

    PlayerEntity* dst = this->engine->getPlayerByID(magic->dstID);
    if(dst->getHandCardNum() < 5)
    {
        this->engine->drawCards(5 - dst->getHandCardNum(),NOTHARMED,dst);
    }
    this->engine->addActionNum(ATTACK);
}

void BowLady::snipe2(QList<void*>args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=304)
        return;
    coder.notice("��֮Ů��ʹ�á��ѻ����Ķ��⹥���ж�");
}

void BowLady::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(accurateShoot(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(thunderArrow(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(transfixtion(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(trap(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(snipe(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(snipe2(QList<void*>)));
}

MoDao::MoDao(BackgroundEngine* engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 8;
    this->star = 3;
    this->makeConnection(engine);
}

void MoDao::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoDanRongHe(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoBaoChongJi(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(HuiMeiFengBao(QList<void*>)));
}

void MoDao::MoDanRongHe(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 801 && magic->infor1 != 802)
        return;
    coder.notice("ħ��ʦ������ħ���ںϡ�");
    QList<CardEntity*> cards;
    CardEntity* card;
    card=getCardByID(magic->CardID);
    cards << card;
    engine->useCard(cards,this,engine->getPlayerByID(magic->dstID));
    this->engine->missileProcess(card,magic->srcID,magic->dstID);
}

void MoDao::MoBaoChongJi(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 803)
        return;
    int dst1=magic->dstID;
    int dst2=magic->infor2;
    QString msg;
    msg="ħ��ʦ�����"+QString::number(dst1)+"�����"+QString::number(dst2)+"������ħ�������";
    coder.notice(msg);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);

    bool flag1,flag2,flag;
    flag1=flag2=true;
    flag=false;
    PlayerEntity* ptr = engine->getNext(this);
    do
    {
        if(flag1 && ptr->getID()==dst1)
        {
            flag1=false;
            coder.askForDiscardMagic(dst1);

            BatInfor ans = messageBuffer::readBatInfor();
            if(ans.reply == 0)
            {
                Harm MoBao;
                MoBao.harmPoint = 2;
                MoBao.type = MAGIC;
                engine->timeLine3(MoBao,this,ptr,"ħ�����");
                flag=true;
                if(engine->checkEnd())
                    break;
            }
            else
            {
                CardEntity *card = getCardByID(ans.CardID);
                QList<CardEntity*> cards;
                cards << card;
                ptr->removeHandCards(cards,true);
                coder.discardNotice(dst1,1,"y",cards);
            }
        }
        else if(flag2 && ptr->getID()==dst2)
        {
            flag2=false;
            coder.askForDiscardMagic(dst2);

            BatInfor ans = messageBuffer::readBatInfor();
            if(ans.reply == 0)
            {
                Harm MoBao;
                MoBao.harmPoint = 2;
                MoBao.type = MAGIC;
                engine->timeLine3(MoBao,this,ptr,"ħ�����");
                flag=true;
                if(engine->checkEnd())
                    break;
            }
            else
            {
                CardEntity *card = getCardByID(ans.CardID);
                QList<CardEntity*> cards;
                cards << card;
                ptr->removeHandCards(cards,true);
                coder.discardNotice(dst2,1,"y",cards);
            }
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
    if(flag)
    {
        teamArea.setGem(color,teamArea.getGem(color)+1);
        coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    }

}

void MoDao::HuiMeiFengBao(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 804)
        return;
    int dst1=magic->dstID;
    int dst2=magic->infor2;
    QString msg;
    msg="ħ��ʦ�����"+QString::number(dst1)+"�����"+QString::number(dst2)+"����������籩��";
    coder.notice(msg);
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());

    bool flag1,flag2;
    flag1=flag2=true;
    PlayerEntity* ptr = engine->getNext(this);
    do
    {
        if(flag1 && ptr->getID()==dst1)
        {
            flag1=false;
            Harm HuiMie;
            HuiMie.harmPoint = 2;
            HuiMie.type = MAGIC;
            engine->timeLine3(HuiMie,this,ptr,"����籩");
            if(engine->checkEnd())
                break;
        }
        else if(flag2 && ptr->getID()==dst2)
        {
            flag2=false;
            Harm HuiMie;
            HuiMie.harmPoint = 2;
            HuiMie.type = MAGIC;
            engine->timeLine3(HuiMie,this,ptr,"����籩");
            if(engine->checkEnd())
                break;
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
}
AnSha:: AnSha(BackgroundEngine* engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 5;
    this->star = 3;
    this->makeConnection(engine);
}

void AnSha::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(FanShi(QList<void*>)));
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(ShuiYing(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(QianXing(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(AttackCheck(QList<void*>)));
}

void AnSha::ShuiYing(QList<void*> args)
{
    int howMany;
    if(this != ((PlayerEntity*)args[1])||handCards.size()==0)
        return;
    coder.askForSkill(this->getID(),"ˮӰ");

    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    howMany=arg[0].toInt();
    if(howMany == 0)
        return;
    else
    {
        QList<CardEntity*>cards;
        arg=arg[1].split(",");
        int i;
        for(i=0;i<howMany;i++)
        {
            cards<<getCardByID(arg[i].toInt());
        }
        this->removeHandCards(cards,true);
        coder.notice("��ɱ�߷�����ˮӰ��");
        coder.discardNotice(this->getID(),howMany,"y",cards);
    }
}

void AnSha::QianXing(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap==1)
    {
        setTap(0);
        coder.tapNotice(id,0,"����ͨ��̬��");
        this->addHandCardsRange(1);
        coder.handcardMaxNotice(id,handCardsMax);
    }
    if(getGem()==0)
        return;
    coder.askForSkill(id,"Ǳ��");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    this->gem--;
    coder.energyNotice(id,this->getGem(),this->getCrystal());
    coder.notice("��ɱ�߷�����Ǳ�С�");
    setTap(1);
    coder.tapNotice(id,1,"��Ǳ����̬��");
    this->addHandCardsRange(-1);
    coder.handcardMaxNotice(id,handCardsMax);
}
/*
#define NOTHARMED  0
#define ATTACKHARM 1
#define MAGICHARM  2
*/
void AnSha::FanShi(QList<void*> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type!=1)
            return;
    coder.notice("��ɱ�߷��������ɡ�");
    engine->drawCards(1,0,(PlayerEntity*)args[0]);

}

void AnSha::AttackCheck(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0] || tap==0)
        return;
    if(*(bool*)args[4])
    {
        ((Harm*)args[2])->harmPoint = 2+getEnergy();
        *((int*)args[5]) = NOREPLY;
        coder.notice("��ɱ�߷�����Ǳ�С�Ч��");
    }
}

TianShi::TianShi(BackgroundEngine* engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 7;
    this->star = 3;
    this->makeConnection(engine);
}

void TianShi::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(FengZhiJieJin(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TianShiZhuFu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TianShiZhiQiang(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(TianShiZhiGe(QList<void*>)));
    connect(engine,SIGNAL(loseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(ShenZhiBiHu(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(usedShield(int)),this,SLOT(TianShiJiBan(int)));
}

void TianShi::FengZhiJieJin(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 701)
        return;
    coder.notice("��ʹ��������֮�ྻ��");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);
    int dstID=magic->dstID;
    PlayerEntity* ptr=engine->getPlayerByID(dstID);
    CardEntity*card=getCardByID(magic->infor2);
    if(ptr->removeBasicEffect(card))
    {
        coder.notice("��ʹ�Ƴ����"+QString::number(dstID)+"��ǰ��"+card->getName());
        TianShiJiBan(id);
    }
}

void TianShi::TianShiZhuFu(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 702)
        return;
    bool flag1,flag2;
    int n=2;
    flag1=true;
    flag2=false;
    int howMany=magic->infor2;
    int dst1=magic->dstID;
    int dst2=-1;

    QString msg;
    msg="��ʹ�����"+QString::number(dst1);
    if(howMany==2)
    {
        dst2=magic->infor3;
        msg+="�����"+QString::number(dst2);
        flag2=true;
        n=1;
    }
    msg+="��������ʹף����";
    coder.notice(msg);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);

    PlayerEntity* ptr = engine->getNext(this);
    do
    {
        if(flag1 && ptr->getID()==dst1)
        {
            cards.clear();
            flag1=false;
            if(engine->getPlayerByID(dst1)->getHandCardNum()>=n)
            {
                coder.askToGiveCard(dst1,n);
                cards=messageBuffer::readCardID(n);
                ptr->giveHandCards(cards,this);
            }
            else if(engine->getPlayerByID(dst1)->getHandCardNum()==1)
            {
                coder.askToGiveCard(dst1,1);
                cards=messageBuffer::readCardID(1);
                ptr->giveHandCards(cards,this);
            }
        }
        else if(flag2 && ptr->getID()==dst2)
        {
            cards.clear();
            flag2=false;
            if(engine->getPlayerByID(dst2)->getHandCardNum()>=n)
            {
                coder.askToGiveCard(dst2,n);
                cards=messageBuffer::readCardID(n);
                ptr->giveHandCards(cards,this);
            }
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
}

void TianShi::TianShiZhiQiang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 703)
        return;
    coder.notice("��ʹ��������ʹ֮ǽ��");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}

void TianShi::TianShiZhiGe(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0] || gem+crystal==0)
        return;
    PlayerEntity*ptr=this;
    int i,n=engine->getPlayerNum();
    for(i=0;i<n;i++)
    {
        if(!ptr->getBasicEffect().empty())
            break;
        ptr=ptr->getNext();
    }
    if(i==n)
        return;
    coder.askForSkill(id,"��ʹ֮��");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply==0)
        return;
    if(ans.infor1)
        gem--;
    else
        crystal--;
    coder.energyNotice(id,gem,crystal);
    int dstID=ans.dstID;
    ptr=engine->getPlayerByID(dstID);
    CardEntity*card=getCardByID(ans.CardID);
    if(ptr->removeBasicEffect(card))
    {
        coder.notice("��ʹ�Ƴ����"+QString::number(dstID)+"��ǰ��"+card->getName());
        TianShiJiBan(id);
    }
}

/*
#define NOTHARMED  0
#define ATTACKHARM 1
#define MAGICHARM  2
*/
void TianShi::ShenZhiBiHu(int harmed,int*howMany,PlayerEntity*dst)
{
    if(harmed!=2 || dst->getColor()!=color || gem+crystal==0|| *howMany==0)
        return;
    coder.askForSkill(id,"��֮�ӻ�",TOQSTR(*howMany));
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;
    else
    {
        *howMany-=ans.infor1+ans.infor2;
        gem-=ans.infor1;
        crystal-=ans.infor2;
        coder.energyNotice(this->getID(),gem,crystal);
        coder.notice("��ʹ��������֮�ӻ�������"+TOQSTR(ans.infor1+ans.infor2)+"��ʿ���½�");
    }

}

void TianShi::TianShiJiBan(int userID)
{
    if(userID!=id)
        return;
    coder.askForSkill(id,"��ʹ�");
    BatInfor ans = messageBuffer::readBatInfor();
    int dstID=ans.dstID;
    PlayerEntity* player= engine->getPlayerByID(dstID);
    coder.notice("��ʹ�����"+TOQSTR(dstID)+"��������ʹ�");
    int cross=player->getCrossNum();
    if(player->getCrossMax()>cross)
    {
        player->setCrossNum(cross+1);
        coder.crossChangeNotice(dstID,cross+1);
    }
}
//ʥŮ��ʼ��
Saintness::Saintness(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 6;
    this->star = 3;
    this->makeConnection(engine);
}

//��˪����
void Saintness::prayerOfFrost(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(card->getElement() != tr("water"))
        return;
    coder.askForSkill(this->getID(),"��˪����");
    BatInfor ans = messageBuffer::readBatInfor();
    int dstID= ans.dstID;
    PlayerEntity* player= engine->getPlayerByID(dstID);
    coder.notice("ʥŮ�����"+TOQSTR(dstID)+"��������˪���ԡ�������1������");
    player->setCrossNum(player->getCrossNum()+1);
    coder.crossChangeNotice(dstID,player->getCrossNum());
}

//������
void Saintness::cure(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 602)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    int dstID=dst->getID();
    coder.notice("ʥŮ�����"+TOQSTR(dstID)+"��������������������2������");
    this->engine->useCard(cards,this,dst);        
    dst->setCrossNum(dst->getCrossNum()+2);
    coder.crossChangeNotice(dstID, dst->getCrossNum());
}

//����֮��
void Saintness::healingLight(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 603)
        return;
    coder.notice("ʥŮ����������֮�⡿");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* player = engine->getPlayerByID(magic->dstID);
    this->engine->useCard(cards,this,player);
    int dst[3], n;
    n=magic->infor2;
    dst[0] = magic->dstID;
    if (n>1)
        dst[1] = magic->infor3;
    if (n>2)
        dst[2] = magic->infor4;
    for(int i=0;i<n; i++)
    {
        player = engine->getPlayerByID(dst[i]);
        coder.notice("ʥŮΪ���"+TOQSTR(dst[i])+"����1������");
        player->setCrossNum(player->getCrossNum()+1);
        coder.crossChangeNotice(dst[i],player->getCrossNum());
    }
}

//����
void Saintness::mercy(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap == 1)
        return;
    if(getGem()==0)
        return;
    coder.askForSkill(this->getID(),"����");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("ʥŮ������������");
    setTap(1);
    setHandCardsMaxFixed(true, 7);
    coder.tapNotice(id,1,"��������̬��");
    coder.handcardMaxNotice(id,handCardsMax);
}

//ʥ��
void Saintness::layOnHands(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 605)
        return;
    int dst[3], addcross[3];
    dst[0] = magic->dstID;
    dst[1] = magic->infor2;
    dst[2] = magic->infor3;
    PlayerEntity* player;
    for(int i=0; i<3; i++)
        addcross[i]=0;
    switch(magic->infor4)
    {
    case 0:
        this->crystal--;
        break;
    case 1:
        this->gem--;
        break;
    default:
        break;
    }
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(dst[i] == dst[j])
            {
                addcross[j]++;
                break;
            }
    for (int i=0; i<3; i++)
    {
        if(addcross[i] == 0)
            continue;
        player = engine->getPlayerByID(dst[i]);
        coder.notice("ʥŮʹ�á�ʥ�ơ�Ϊ���"+TOQSTR(dst[i])+"����"+TOQSTR(addcross[i])+"������");
        player->setCrossNum(player->getCrossNum()+addcross[i]);
        coder.crossChangeNotice(dst[i], player->getCrossNum());
    }
    this->engine->addActionNum(ATTACKORMAGIC);
}

void Saintness::layOnHands2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=606)
        return;
    coder.notice("ʥŮʹ�á�ʥ�ơ��Ĺ��������ж�");
}

//
void Saintness::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(prayerOfFrost(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(cure(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(healingLight(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(mercy(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(layOnHands(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(layOnHands2(QList<void*>)));
}

JianSheng::JianSheng(BackgroundEngine *engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 1;
    this->star = 3;
    this->makeConnection(engine);
}

//�ҷ缼
void JianSheng::LieFengJi1(QList<void*> args)
{
    LieFengJiUsed=false;
    if(this != (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(!card->getSpecialityList().contains(tr("�ҷ缼")))
        return;
    PlayerEntity* dst=(PlayerEntity*)args[1];
    bool flag=false;
    for(int i = 0;i < dst->getBasicEffect().size();i++)
        if(dst->getBasicEffect().at(i)->getMagicName() == SHIELDCARD || dst->getBasicEffect().at(i)->getSpecialityList().contains(tr("��ʹ֮ǽ")))
        {
            flag=true;
            break;
        }
    if(!flag)
        return;
    coder.askForSkill(id,"�ҷ缼");
    if(messageBuffer::readInfor() == 0)
        return;
    *((int*)args[5]) = NOREPLY;
    coder.notice("��ʥ�������ҷ缼��");
    LieFengJiUsed=true;
}

void JianSheng::LieFengJi2(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!LieFengJiUsed)
        return;
    *((bool*)args[2]) =  false;
}

//���缼
void JianSheng::JiFengJi1(QList<void *> args)
{
    PlayerEntity* myself=(PlayerEntity*)args[0];
    if(this != myself ||!*(bool*)args[4])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(!card->getSpecialityList().contains(tr("���缼")))
        return;
    coder.notice("��ʥ���������缼��");
    engine->addActionNum(ATTACK);
}
void JianSheng::JiFengJi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=103)
        return;
    coder.notice("��ʥʹ�á����缼���Ķ��⹥���ж�");
}

//������
void JianSheng::LianXuJi1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(getHandCardNum()<=0 ||LianXuJiUsed)
        return;
    engine->addActionNum(ATTACK);
    LianXuJiUsed=true;
}
void JianSheng::LianXuJi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=101)
        return;
    coder.notice("��ʥ��������������");
}

//��Ӱ
void JianSheng::JianYing1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(this->getEnergy() <= 0 || getHandCardNum()<=0 ||JianYingUsed)
        return;
    engine->addActionNum(ATTACK);
    JianYingUsed=true;
}
void JianSheng::JianYing2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=102)
        return;
    if(crystal>0)
        crystal--;
    else
        gem--;
    coder.notice("��ʥ��������Ӱ��");
    coder.energyNotice(id,gem,crystal);
}

void JianSheng::skillReset(QList<void*>args){
    if(this != ((PlayerEntity*)args[0]))
        return;
    JianYingUsed=false;
    LianXuJiUsed=false;
    attackCount=0;
}
//ʥ��
void JianSheng::ShengJian(QList<void *> args){
    if(this != ((PlayerEntity*)args[0])||!*(bool*)args[4])
        return;
    attackCount++;
    if(attackCount==3){
        coder.notice("��ʥ������ʥ����");
        *((int*)args[5]) = NOMISS;
    }
}

void JianSheng::makeConnection(BackgroundEngine *engine)
{    
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(LieFengJi1(QList<void*>)));
    connect(engine,SIGNAL(shieldSIG(QList<void*>)),this,SLOT(LieFengJi2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(JiFengJi1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JiFengJi2(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(LianXuJi1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(LianXuJi2(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(JianYing1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JianYing2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(ShengJian(QList<void*>)));
}

FengYin::FengYin(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 4;
    this->star = 3;
    this->makeConnection(engine);
    shuFuID=-1;
}
//��������
void FengYin::FaShuJiDang1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(getHandCardNum()<=0)
        return;
    engine->addActionNum(ATTACK);
}
void FengYin::FaShuJiDang2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=404)
        return;
    coder.notice("��ӡʦ����������������");
}

//��ӡ����
void FengYin::FengYinFaShu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 401)
        return;
    coder.notice("��ӡʦ��������ӡ������");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}
void FengYin::FengYinFaShu2(QList<CardEntity*> cards,PlayerEntity* user)
{
    foreach(CardEntity*fengYin, user->getBasicEffect())
        if(fengYin->getType()=="attack" && fengYin->getProperty()==tr("��"))
            foreach(CardEntity*ptr, cards)
                if(ptr->getElement()==fengYin->getElement())
                {
                    coder.notice("���"+QString::number(user->getID())+"����"+fengYin->getSpecialityList().at(0));
                    user->removeBasicEffect(fengYin);
                    Harm harm;
                    harm.harmPoint = 3;
                    harm.type = MAGIC;
                    engine->timeLine3(harm,this,user,fengYin->getSpecialityList().at(0));
                    break;
                }
}

//��ϵ����
void FengYin::WuXiShuFu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 402)
        return;
    if(magic->infor2)
        gem--;
    else
        crystal--;
    shuFuID=magic->dstID;
    coder.notice("��ӡʦ�����"+QString::number(shuFuID)+"��������ϵ������");
    coder.energyNotice(id,gem,crystal);
    coder.specialNotice(shuFuID,0,1);
}

void FengYin::WuXiShuFu2(PlayerEntity* player, bool* withoutTrap)
{
    if(player->getID()!=shuFuID)
        return;
    bool weak=false;
    *withoutTrap=true;
    int howMany=2;
    PlayerEntity* ptr=player;
    for(int i=0;i<engine->getPlayerNum();i++)
    { 
        foreach(CardEntity*card,ptr->getBasicEffect())
            if(card->getType()=="attack"&& card->getProperty()==tr("��"))
                howMany++;
        ptr=ptr->getNext();
    }
    if(howMany>4)
        howMany=4;
    for(int i = 0;i < player->getBasicEffect().size();i++)
        if(player->getBasicEffect().at(i)->getMagicName() == WEAKCARD)
        {
            engine->weakProcess(player,3+howMany);
            player->removeBasicEffect(player->getBasicEffect()[i]);
            weak=true;
            break;
        }
    if(!weak)
        engine->weakProcess(player,howMany);
    coder.specialNotice(shuFuID,0,0);
    shuFuID=-1;
}

//��ӡ����
void FengYin::FengYinPoSui(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 403)
        return;
    coder.notice("��ӡʦ��������ӡ���顿");
    if(magic->infor2)
        gem--;
    else
        crystal--;
    coder.energyNotice(id,gem,crystal);
    int dstID=magic->dstID;
    PlayerEntity* ptr=engine->getPlayerByID(dstID);
    CardEntity*card=getCardByID(magic->CardID);
    QList<CardEntity*>cards;
    cards.append(card);
    if(ptr->removeBasicEffect(card,id,HAND))
    {
        coder.notice("��ӡʦ�������"+QString::number(dstID)+"��ǰ��"+card->getName());
        coder.getCardNotice(1,cards,id,true);
        addHandCards(cards,0,false);
    }
}

void FengYin::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(FengYinFaShu1(QList<void*>)));
    connect(engine,SIGNAL(showHandCards(QList<CardEntity*>,PlayerEntity*)),this,SLOT(FengYinFaShu2(QList<CardEntity*>,PlayerEntity*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(WuXiShuFu1(QList<void*>)));
    connect(engine,SIGNAL(weakSIG(PlayerEntity*,bool*)),this,SLOT(WuXiShuFu2(PlayerEntity*,bool*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(FengYinPoSui(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(FaShuJiDang1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(FaShuJiDang2(QList<void*>)));
}

MoJian::MoJian(BackgroundEngine *engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 9;
    this->star = 3.5;
    this->makeConnection(engine);
}

//�ڰ����
void MoJian::HeiAnZhenChan1(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0]||HeiAnZhenChanUsed||gem<=0||!*(bool*)args[4])
        return;
    coder.askForSkill(id,"�ڰ����");
    isHeiAnZhenChan=false;
    if(messageBuffer::readInfor() == 0)
        return;
    *((int*)args[5]) = NOREPLY;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice("ħ���������ڰ������");
    HeiAnZhenChanUsed=true;
    isHeiAnZhenChan=true;
}

void MoJian::HeiAnZhenChan2(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0]||!isHeiAnZhenChan)
        return;
    isHeiAnZhenChan=false;
    engine->drawCards(getHandCardMax()-getHandCardNum(),0,this);
}

void MoJian::HeiAnZhenChan3(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||!isHeiAnZhenChan)
        return;
    isHeiAnZhenChan=false;
}

//������ն
void MoJian::XiuLuoLianZhan1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(getHandCardNum()<=0 ||XiuLuoLianZhanUsed)
        return;
    engine->addActionNum(ATTACK);
    XiuLuoLianZhanUsed=true;
}

void MoJian::XiuLuoLianZhan2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=901)
        return;
    coder.notice("ħ��������������ն��");
}

void MoJian::skillReset(QList<void*>args){
    if(this != ((PlayerEntity*)args[0]))
        return;
    XiuLuoLianZhanUsed=false;
    HeiAnZhenChanUsed=false;
    isHeiAnZhenChan=false;
}

//��Ӱ����
void MoJian::AnYingNingJu(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap==1)
    {
        setTap(0);
        coder.tapNotice(id,0,"����ͨ��̬��");
    }
    coder.askForSkill(this->getID(),"��Ӱ����");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    coder.notice("ħ����������Ӱ���ۡ�");
    Harm anying;
    anying.harmPoint=1;
    anying.type=MAGICHARM;
    engine->timeLine3(anying,this,this,"��Ӱ����");
    setTap(1);
    coder.tapNotice(id,1,"����Ӱ��̬��");
}

//��Ӱ֮��
void MoJian::AnYingZhiLi(QList<void*> args)
{
    if(this != ((PlayerEntity*)args[0])||!tap)
        return;

    Harm *harm = (Harm*)args[2];
    if(harm->type != ATTACK)
        return;
    harm->harmPoint++;
    coder.notice("ħ����������Ӱ֮����");
}

//��Ӱ����
void MoJian::AnYingLiuXing(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 902)
        return;
    coder.notice("ħ����������Ӱ���ǡ�");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    coder.discardNotice(id,2,"y",cards);
    this->removeHandCards(cards,true);

    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"��Ӱ����");
}

void MoJian::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(AnYingLiuXing(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(XiuLuoLianZhan1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(XiuLuoLianZhan2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(HeiAnZhenChan1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(HeiAnZhenChan2(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(HeiAnZhenChan3(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(AnYingNingJu(QList<void*>)));
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(AnYingZhiLi(QList<void*>)));
}

MaoXian::MaoXian(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 12;
    this->star = 3.5;
    this->makeConnection(engine);
}
void MaoXian::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillAttack(QList<void*>)),this,SLOT(QiZha(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TouTianHuanRi(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TeShuJiaGong(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(TouTianHuanRi2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(TeShuJiaGong2(QList<void*>)));
    connect(engine,SIGNAL(skillSpecial(QList<void*>)),this,SLOT(MaoXianZheTianTang(QList<void*>)));
}

void MaoXian::QiZha(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=1201)
        return;
    coder.notice("ð�ռҷ�������թ��");
    setCrystal(crystal+1);
    coder.energyNotice(id,gem,crystal);
    QList<CardEntity*>cards;
    cards.append(getCardByID(skill->infor3));
    cards.append(getCardByID(skill->infor4));
    if(skill->infor2==3)
        cards.append(getCardByID(skill->infor5));
    *(int*)args[1]=0;
    coder.discardNotice(id,skill->infor2,"y",cards);
    this->removeHandCards(cards,true);
}

//͵�컻��
void MaoXian::TouTianHuanRi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id)
        return;
    if(magic->infor1 != 1203)
        return;
    if(magic->infor2)
        gem--;
    else
        crystal--;
    coder.notice("ð�ռҷ�����͵�컻�ա�");
    coder.energyNotice(id,gem,crystal);
    teamArea.setGem(color,teamArea.getGem(color)+1);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    int other=1-color;
    teamArea.setGem(other,teamArea.getGem(other)-1);
    coder.stoneNotice(other,teamArea.getGem(other),teamArea.getCrystal(other));
    engine->addActionNum(ATTACKORMAGIC);
}
void MaoXian::TouTianHuanRi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1203)
        return;
    coder.notice("ð�ռ�ʹ�á�͵�컻�ա��Ķ��⹥���ж�");
}

//����ӹ�
void MaoXian::TeShuJiaGong(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id)
        return;
    if(magic->infor1 != 1202)
        return;
    if(magic->infor2)
        gem--;
    else
        crystal--;
    coder.notice("ð�ռҷ���������ӹ���");
    coder.energyNotice(id,gem,crystal);
    int toChange=teamArea.getCrystal(color);
    teamArea.setCrystal(color,0);
    teamArea.setGem(color,toChange+teamArea.getGem(color));
    coder.stoneNotice(color,teamArea.getGem(color),0);
    engine->addActionNum(ATTACKORMAGIC);
}
void MaoXian::TeShuJiaGong2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1203)
        return;
    coder.notice("ð�ռ�ʹ�á�͵�컻�ա��Ķ��⹥���ж�");
}
//ð��������
void MaoXian::MaoXianZheTianTang(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id)
        return;
    if(magic->infor1 != 1204)
        return;
    coder.notice("ð�ռҶ����"+QString::number(magic->dstID)+"������ð�������á�");
    PlayerEntity*dst=engine->getPlayerByID(magic->dstID);
    dst->setGem(dst->getGem()+magic->infor2);
    dst->setCrystal(dst->getCrystal()+magic->infor3);
    teamArea.setGem(color,teamArea.getGem(color)-magic->infor2);
    teamArea.setCrystal(color,teamArea.getCrystal(color)-magic->infor3);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    coder.energyNotice(magic->dstID,dst->getGem(),dst->getCrystal());
}

YuanSu::YuanSu(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 11;
    this->star = 3.5;
    tokenMax[0]=3;
    ignite=false;
    this->makeConnection(engine);
}
void YuanSu::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(YuanSuFaShu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(YuanSuDianRan(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(YuanSuFaShu2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(YuanSuDianRan2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(YueGuang(QList<void*>)));
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(YuanSuXiShou(QList<void*>)));
}

//Ԫ������
void YuanSu::YuanSuXiShou(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0] || token[0]==3)
        return;
    if(ignite){
        ignite=false;
        return;
    }
    Harm* h=(Harm*)args[2];
    if(h->type==MAGICHARM)
    {
        setToken(0,token[0]+1);
        coder.notice("Ԫ��ʦ������Ԫ�����ա�");
        coder.tokenNotice(id,0,token[0]);
    }
}

//Ԫ�ط���
void YuanSu::YuanSuFaShu(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1101)
        return;
    int harmPoint=magic->infor3;
    QString skill;
    switch(magic->infor2)
    {
    case 1:
        skill="����";
        break;
    case 2:
        skill="����";
        break;
    case 3:
        skill="����";
        harmPoint++;
        break;
    case 4:
        skill="��ʯ";
        break;
    case 5:
        skill="�׻�";
        break;
    }
    coder.notice("Ԫ��ʦ�����"+QString::number(magic->dstID)+"������"+skill+"��");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    if(magic->infor3==2)
        cards << getCardByID(magic->infor4);
    coder.discardNotice(id,magic->infor3,"y",cards);
    this->removeHandCards(cards,true);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = harmPoint;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,skill);
    switch(magic->infor2)
    {
    case 1:
        engine->addActionNum(ATTACK);
        break;
    case 2:
        dst=engine->getPlayerByID(magic->infor5);
        dst->setCrossNum(dst->getCrossNum()+1);
        coder.notice("Ԫ�صġ�������Ϊ���"+QString::number(magic->infor5)+"����1����");
        coder.crossChangeNotice(magic->infor5,dst->getCrossNum());
        break;
    case 4:
        engine->addActionNum(MAGIC);
        break;
    case 5:
        teamArea.setGem(color,teamArea.getGem(color)+1);
        coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
        break;
    }
}
void YuanSu::YuanSuFaShu2(QList<void*> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id)
        return;
    if(skill->infor1==1104)
        coder.notice("Ԫ��ʦʹ�á����С��Ķ��⹥���ж�");
    if(skill->infor1==1105)
        coder.notice("Ԫ��ʦʹ�á���ʯ���Ķ��ⷨ���ж�");
}

//Ԫ�ص�ȼ
void YuanSu::YuanSuDianRan(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id||magic->infor1 != 1102)
        return;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    coder.notice("Ԫ��ʦ�����"+QString::number(magic->dstID)+"������Ԫ�ص�ȼ��");
    setToken(0,0);
    ignite=true;
    coder.tokenNotice(id,0,0);
    engine->timeLine3(harm,this,dst,"Ԫ�ص�ȼ");
    engine->addActionNum(MAGIC);
}
void YuanSu::YuanSuDianRan2(QList<void*> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1106)
        return;
    coder.notice("Ԫ��ʦʹ�á�Ԫ�ص�ȼ���Ķ��ⷨ���ж�");
}

//�¹�
void YuanSu::YueGuang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id||magic->infor1 != 1103)
        return;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    coder.notice("Ԫ��ʦ�����"+QString::number(magic->dstID)+"�������¹⡿");
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    Harm harm;
    harm.harmPoint = gem+crystal+1;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,dst,"�¹�");
}

ZhongCai::ZhongCai(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=14;
    this->star=3.5;
    tokenMax[0]=4;
    crystal=2;
    startUsed=false;
    this->makeConnection(engine);
}

void ZhongCai::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(YiShiZhongDuan(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(ZhongCaiYiShi1(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(ZhongCaiYiShi2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoRiShenPan(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(PanJueTianPing(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(ShenPanLangChao(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

//��ʽ�ж�
void ZhongCai::YiShiZhongDuan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap!=1 || startUsed)
        return;
    coder.askForSkill(this->getID(),"��ʽ�ж�");
    if(messageBuffer::readInfor() == 0)
        return;
    coder.notice("�ٲ��߷�������ʽ�жϡ�");
    setTap(0);
    coder.tapNotice(this->getID(),0,"����ͨ��̬��");
    startUsed=true;
    setHandCardsMaxFixed(false);
    coder.handcardMaxNotice(this->getID(),handCardsMax);
    teamArea.setGem(color,teamArea.getGem(color)+1);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
}

//�ٲ���ʽ����
void ZhongCai::ZhongCaiYiShi1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap!=0 || this->getGem()==0 || startUsed)
        return;
    coder.askForSkill(this->getID(),"�ٲ���ʽ");
    if(messageBuffer::readInfor() == 0)
        return;
    coder.notice("�ٲ��߷������ٲ���ʽ��");
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    setTap(1);
    coder.tapNotice(this->getID(),1,"��������̬��");
    startUsed=true;
    setHandCardsMaxFixed(true, 5);
    coder.handcardMaxNotice(this->getID(),handCardsMax);
}

//�ٲ���ʽ����
void ZhongCai::ZhongCaiYiShi2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap!=1)
        return;
    if(token[0]==4)
        return;
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
}

//ĩ������
void ZhongCai::MoRiShenPan(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1403)
        return;
    QString msg;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    msg="�ٲ��߶����"+QString::number(magic->dstID)+"������ĩ�����С�";
    coder.notice(msg);
    int tokenNum=token[0];
    setToken(0,0);
    coder.tokenNotice(id,0,0);
    Harm harm;
    harm.harmPoint=tokenNum;
    harm.type=MAGICHARM;
    engine->timeLine3(harm,this,dst,"ĩ������");
}

//�о���ƽ
void ZhongCai::PanJueTianPing(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1404)
        return;
    if(crystal>0)
        crystal--;
    else
        gem--;
    setGem(gem);
    setCrystal(crystal);
    coder.energyNotice(id,gem,crystal);
    coder.notice("�ٲ��߷������о���ƽ��");
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
    if(magic->infor2)
    {
        int drawNum=this->getHandCardMax()-this->getHandCardNum();
        engine->drawCards(drawNum,0,this);
        teamArea.setGem(color,teamArea.getGem(color)+1);
        coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    }
    else
    {
        coder.discardNotice(this->getID(),this->getHandCards().length(),"n",this->getHandCards());
        this->removeHandCards(this->getHandCards(),false,true);
    }
}

//�����˳�
void ZhongCai::ShenPanLangChao(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1])
        return;
    coder.notice(tr("�ٲ��߷����������˳���"));
    setToken(0,token[0]+1);
    coder.tokenNotice(this->getID(),0,token[0]);
}

//��������
void ZhongCai::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    startUsed=false;
}

YongZhe::YongZhe(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=21;
    this->star=4.5;
    tokenMax[0]=4;
    tokenMax[1]=4;
    fireNum=0;
    tiaoXinID=-1;
    crystal=2;
    nuHouUsed=false;
    tiaoXinUsed=false;
    tiaoXinChuFa=false;
    jinDuanUsed=false;
    this->makeConnection(engine);
}

void YongZhe::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(NuHou(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(NuHou1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(NuHou2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(MingJingZhiShui(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(JinDuanZhiLi1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(JinDuanZhiLi2(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(SiDou(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TiaoXin1(QList<void*>)));
    connect(engine,SIGNAL(tiaoXinPhaseSIG(PlayerEntity*,int*,bool*)),this,SLOT(TiaoXin2(PlayerEntity*,int*,bool*)));
    connect(engine,SIGNAL(turnEndPhaseSIG(PlayerEntity*)),this,SLOT(TiaoXin3(PlayerEntity*)));
    //connect(engine,SIGNAL(turnEndPhaseSIG(QList<void*>)),this,SLOT(TiaoXin4(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(JingPiLiJie1(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(JingPiLiJie2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JingPiLiJie3(QList<void*>)));
}

//ŭ��ѯ��
void YongZhe::NuHou(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || this->getToken(0)== 0)
        return;
    this->nuHouUsed=false;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(this->getID(),"ŭ��");
    if(messageBuffer::readInfor() == 0)
        return;
    setToken(0,token[0]-1);
    coder.notice("���߷�����ŭ��");
    this->nuHouUsed=true;
    coder.tokenNotice(id,0,token[0]);
}

//ŭ������
void YongZhe::NuHou1(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!this->nuHouUsed)
        return;
    nuHouUsed=false;
    Harm* harm=(Harm*)args[2];
    harm->harmPoint+=2;
}

//ŭ��δ����
void YongZhe::NuHou2(QList<void *> args)
{
    if(this!=(PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!this->nuHouUsed)
        return;
    nuHouUsed=false;
    if(this->getToken(1)!=4)
    {
        setToken(1,token[1]+1);
        coder.tokenNotice(id,1,token[1]);
    }
}

//����ֹˮ
void YongZhe::MingJingZhiShui(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || this->getToken(1)<4)
        return;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(this->getID(),"����ֹˮ");
    if(messageBuffer::readInfor() == 0)
        return;
    setToken(1,0);
    coder.notice("���߷���������ֹˮ��");
    coder.tokenNotice(id,1,0);
    *(int*)args[5] = NOREPLY;
}

//��������
void YongZhe::JinDuanZhiLi1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || this->getEnergy()==0)
        return;
    if(!*(bool*)args[4])
        return;
    if(this->getHandCards().length()== 0)
        return;
    coder.askForSkill(this->getID(),"����֮��");
    if(messageBuffer::readInfor() == 0)
        return;
    if(this->getCrystal()>0)
        setCrystal(crystal-1);
    else
        setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice("���߷���������֮����");
    jinDuanUsed=true;
    for(int i=0;i < this->getHandCards().length();i++)
    {
        if(this->getHandCards().at(i)->getType() != "attack")
        {
            if(this->getToken(0)!=4)
                setToken(0,token[0]+1);
        }
        if(this->getHandCards().at(i)->getElement() == "fire")
            fireNum++;
    }
    coder.tokenNotice(id,0,token[0]);
    coder.discardNotice(this->getID(),this->getHandCards().length(),tr("y"),this->getHandCards());
    this->removeHandCards(this->getHandCards(),true,true);
    Harm* harm=(Harm*)args[2];
    harm->harmPoint+=fireNum;
    Harm jinduanzhili;
    jinduanzhili.harmPoint=fireNum;
    jinduanzhili.type=MAGICHARM;
    if(fireNum != 0){
        engine->timeLine3(jinduanzhili,this,this,"����֮��");
        if(engine->checkEnd())
            return;
    }
    setTap(1);
    fireNum=0;
}

//����δ����
void YongZhe::JinDuanZhiLi2(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || this->getEnergy()==0)
        return;
    if(!*(bool*)args[4])
        return;
    if(this->getHandCards().length()== 0)
        return;
    coder.askForSkill(this->getID(),"����֮��");
    if(messageBuffer::readInfor() == 0)
        return;
    if(this->getCrystal()>0)
        setCrystal(crystal-1);
    else
        setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice("���߷���������֮����");
    jinDuanUsed=true;
    for(int i=0;i < this->getHandCards().length();i++)
    {
        if(this->getHandCards().at(i)->getType() != "attack")
        {
            if(this->getToken(0)!=4)
                setToken(0,token[0]+1);
        }
        if(this->getHandCards().at(i)->getElement() == "water")
        {
            if(this->getToken(1)!=4)
                setToken(1,token[1]+1);
        }
    }
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
    coder.discardNotice(this->getID(),this->getHandCards().length(),tr("y"),this->getHandCards());
    this->removeHandCards(this->getHandCards(),true,true);
    setTap(1);
}

//����
void YongZhe::SiDou(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type != MAGICHARM || this->getGem()==0)
        return;
    coder.askForSkill(this->getID(),"����");
    if(messageBuffer::readInfor() == 0)
        return;
    setGem(gem-1);
    coder.energyNotice(this->getID(),gem,crystal);
    coder.notice("���߷�����������");
    if(token[0]+3<=4)
        setToken(0,token[0]+3);
    else
        setToken(0,4);
    coder.tokenNotice(id,0,token[0]);
}

//���Ʒ���
void YongZhe::TiaoXin1(QList<void *> args)
{
    BatInfor* magic= (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2101)
        return;
    if(tiaoXinUsed)
        return;
    setToken(0,token[0]-1);
    setToken(1,token[1]+1);
    tiaoXinID=magic->dstID;
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
    coder.notice("���߶����"+QString::number(tiaoXinID)+"���������ơ�");
    coder.specialNotice(tiaoXinID,1,1);
    tiaoXinUsed=true;
}

//���ƴ����ж�1
void YongZhe::TiaoXin2(PlayerEntity *player, int *act,bool* canGiveUp)
{
    if(player->getID() != tiaoXinID)
        return;
    if(player->getRoleID()==14 && player->getToken(0)==4)
        *act=-1;
    else
        *act=1;
    *canGiveUp=true;
    if(!tiaoXinUsed)
        return;
    tiaoXinChuFa=true;
}

//�����Ƴ�
void YongZhe::TiaoXin3(PlayerEntity *player)
{
    if(player->getID() != tiaoXinID)
        return;
    if(!tiaoXinUsed)
        return;
    if(!tiaoXinChuFa)
        return;
    coder.specialNotice(tiaoXinID,1,0);
    tiaoXinID=-1;
    tiaoXinUsed=false;
}

//��ƣ���ߴ���
void YongZhe::JingPiLiJie1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(this->tap != 1)
        return;
    if(jinDuanUsed){
    coder.tapNotice(this->getID(),1,"����ƣ���ߡ�");
    setHandCardsMaxFixed(true, 4);
    coder.handcardMaxNotice(id,handCardsMax);
    jinDuanUsed=false;
    engine->addActionNum(ATTACK);
    }
}

//��ƣ���߽���
void YongZhe::JingPiLiJie2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(this->tap != 1)
        return;
    setTap(0);
    coder.tapNotice(this->getID(),0,"����ͨ��̬��");
    setHandCardsMaxFixed(false);
    coder.handcardMaxNotice(id,handCardsMax);
    Harm jingpilijie;
    jingpilijie.harmPoint=3;
    jingpilijie.type=MAGICHARM;
    engine->timeLine3(jingpilijie,this,this,"��ƣ����");
}
void YongZhe::JingPiLiJie3(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=2103)
        return;
    coder.notice("����ʹ�á���ƣ���ߡ��Ķ��⹥���ж�");
}

/******************
  ʥǹ 10
  *******************/
ShengQiang::ShengQiang(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 10;
    this->star = 3.5;
    //��ʥ����
    this->crossMax = 3;
    this->makeConnection(engine);
}

//��ҫ
void ShengQiang::HuiYao(QList<void *> args)
{
     BatInfor *magic = (BatInfor*)args[0];
     if(magic->srcID != this->getID())
         return;
     if(magic->infor1 != 1001)
         return;

     QList<CardEntity*> cards;
     cards << getCardByID(magic->CardID);
     coder.notice("ʥǹ��������ҫ��");
     this->removeHandCards(cards,true);
     coder.discardNotice(this->getID(),1,"y",cards);
     PlayerEntity* dst = this;
     do{
         dst->setCrossNum(dst->getCrossNum()+1);
         coder.crossChangeNotice(dst->getID(), dst->getCrossNum());
         dst = dst->getNext();
     }while(dst!= this);
     this->engine->addActionNum(ATTACK);
}
void ShengQiang::HuiYao2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1003)
        return;
    coder.notice("ʥǹʹ�á���ҫ���Ķ��⹥���ж�");
}

//�ͽ�
void ShengQiang::ChengJie(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1002)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.notice("ʥǹ�����"+QString::number(magic->dstID)+"�������ͽ䡿");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), 1, "y", cards);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    dst->setCrossNum(dst->getCrossNum()-1);
    coder.crossChangeNotice(dst->getID(), dst->getCrossNum());
    coder.notice("���"+QString::number(magic->dstID)+"����1�����ƣ�ʥǹ����1����");
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    this->engine->addActionNum(ATTACK);
}
void ShengQiang::ChengJie2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1004)
        return;
    coder.notice("ʥǹʹ�á��ͽ䡿�Ķ��⹥���ж�");
}
//ʥ����ֻ�жϷ���������
void ShengQiang::ShengJi(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(*(bool*)args[4])
        return;
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    coder.notice("ʥǹ������ʥ����������1����");
}

//��ǹ
void ShengQiang::TianQiang(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    if(this->ShengGuangQiYuUsed)
        return;
    int cross = this->getCrossNum();
    if(cross<2)
        return;
    coder.askForSkill(this->getID(),"��ǹ");
    if(messageBuffer::readInfor() == 0)
        return;
    TianQiangUsed = true;
    this->setCrossNum(getCrossNum()-2);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    coder.notice("ʥǹ��������ǹ��");
    *(int*)args[5] = NOREPLY;
}

//��ǹ����������δ������ǹ�򷢶�ʥ��
void ShengQiang::DiQiang(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    int cross = this->getCrossNum();
    if(cross > 0)
    {
        coder.askForSkill(this->getID(),"��ǹ");
        int reply = messageBuffer::readInfor();
        if(reply != 0)
        {
            this->setCrossNum(getCrossNum()-reply);
            coder.crossChangeNotice(this->getID(), getCrossNum());
            coder.notice("ʥǹ��������ǹ����ʹ��"+QString::number(reply)+"������");
            Harm *harm = (Harm*)args[2];
            harm->harmPoint += reply;
        }
        else
            if(!TianQiangUsed)
        {
            this->setCrossNum(getCrossNum()+1);
            coder.crossChangeNotice(this->getID(), getCrossNum());
            coder.notice("ʥǹ������ʥ����������1����");
        }
    }
    else
    {
        if(TianQiangUsed)
            return;
        this->setCrossNum(getCrossNum()+1);
        coder.crossChangeNotice(this->getID(), getCrossNum());
        coder.notice("ʥǹ������ʥ����������1����");
    }

}

//ʥ������
void ShengQiang::ShengGuangQiYu(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1006)
        return;
    if(getGem()==0)
        return;
    ShengGuangQiYuUsed = true;
    coder.notice("ʥǹ������ʥ��������������2����");
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    this->setCrossNum(getCrossNum()+2,5);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    this->engine->addActionNum(ATTACK);
}
void ShengQiang::ShengGuangQiYu2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1007)
        return;
    coder.notice("ʥǹʹ�á�ʥ���������Ķ��⹥���ж�");
}
void ShengQiang::skillReset(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    ShengGuangQiYuUsed = false;
    TianQiangUsed = false;
}

void ShengQiang::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(HuiYao(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ChengJie(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(HuiYao2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(ChengJie2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShengGuangQiYu(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(ShengGuangQiYu2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(TianQiang(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(ShengJi(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(DiQiang(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

QiDao::QiDao(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 16;
    this->star = 4;
    this->makeConnection(engine);
    tokenMax[0]=3;
}
//������ϫ
void QiDao::FaLiChaoXi1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]) || FaLiChaoXiUsed || getEnergy()==0)
        return;
    if(getHandCardNum()<=0 && getToken(0)==0)
        return;
    FaLiChaoXiUsed=true;
    engine->addActionNum(MAGIC);
}
void QiDao::FaLiChaoXi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=1606)
        return;
    if(crystal>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(id,gem,crystal);
    coder.notice("��ʦ������������ϫ��");
}

//�����͸�
void QiDao::WeiLiCiFu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1602)
        return;
    coder.notice("��ʦ�����������͸���");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}
void QiDao::WeiLiCiFu2(QList<void*> args)
{
    PlayerEntity*user=(PlayerEntity*)args[0];
    foreach(CardEntity*weili, user->getBasicEffect())
        if(weili->getSpecialityList().contains(tr("�����͸�")))
        {
            //�˴�ѯ���Ƿ�ʹ��
            coder.askForSkill(user->getID(),"�����͸�");
            if(messageBuffer::readInfor() == 0)
                return;
            user->removeBasicEffect(weili);
            coder.notice("���"+QString::number(user->getID())+"�����������͸���");
            Harm *harm = (Harm*)args[2];
            harm->harmPoint += 2;
            break;
        }
}

//Ѹ�ݴ͸�
void QiDao::XunJieCiFu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1603)
        return;
    coder.notice("��ʦ������Ѹ�ݴ͸���");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}
void QiDao::XunJieCiFu2(QList<void*> args)
{
    PlayerEntity*user=(PlayerEntity*)args[0];
    foreach(CardEntity*xunjie, user->getBasicEffect())
        if(xunjie->getSpecialityList().contains(tr("Ѹ�ݴ͸�"))){
            engine->addActionNum(ATTACK);
            break;
        }
}
void QiDao::XunJieCiFu3(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->infor1!=1607)
        return;
    PlayerEntity*user=engine->getPlayerByID(skill->srcID);
    foreach(CardEntity*xunjie, user->getBasicEffect())
        if(xunjie->getSpecialityList().contains(tr("Ѹ�ݴ͸�"))){
            user->removeBasicEffect(xunjie);
            break;
        }
    coder.notice("���"+QString::number(skill->srcID)+"������Ѹ�ݴ͸���");
}

//�������
void QiDao::GuangHuiXinYang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1604)
        return;
    coder.notice("��ʦ�����"+QString::number(magic->dstID)+"���������������");
    token[0]--;
    coder.tokenNotice(id,0,token[0]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    coder.discardNotice(id,cards.size(),"n",cards);
    this->removeHandCards(cards,false);
    dst->setCrossNum(dst->getCrossNum()+1);
    coder.crossChangeNotice(magic->dstID,dst->getCrossNum());
    teamArea.setGem(color,teamArea.getGem(color)+1);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
}

//�������
void QiDao::QiHeiXinYang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1605)
        return;
    coder.notice("��ʦ�����"+QString::number(magic->dstID)+"���������������");
    token[0]--;
    coder.tokenNotice(id,0,token[0]);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"�������");
    if(engine->checkEnd())
        return;
    this->engine->timeLine3(harm,this,this,"�������");
}

void QiDao::Pray1(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap == 1)
        return;
    if(getGem()==0)
        return;
    coder.askForSkill(this->getID(),"��");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("��ʦ����������");
    setTap(1);
    coder.tapNotice(id,1,"������̬��");
}
void QiDao::Pray2(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0]||!*(bool*)args[4]||!tap)
        return;
    setToken(0,token[0]+2);
    coder.tokenNotice(id,0,token[0]);
}
void QiDao::skillReset(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    FaLiChaoXiUsed = false;
}

void QiDao::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(WeiLiCiFu1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(WeiLiCiFu2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XunJieCiFu1(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(XunJieCiFu2(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(XunJieCiFu2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(XunJieCiFu3(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(FaLiChaoXi1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(FaLiChaoXi2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(GuangHuiXinYang(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(QiHeiXinYang(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(Pray1(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(Pray2(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}


/******************
  ��� 15
  *******************/
ShenGuan::ShenGuan(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 15;
    this->star = 4;
    //��ʥ����
    this->crossMax = 6;
    this->makeConnection(engine);
}

//��ʥ��ʾ
void ShenGuan::ShenShengQiShi(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    coder.askForSkill(this->getID(), "��ʥ��ʾ");
    if(messageBuffer::readInfor() == 0)
        return;

    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(id, getCrossNum());

    coder.notice("��ٷ�������ʥ��ʾ��������1����");
}
//��ʥ��
void ShenGuan::ShenShengQiFu(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID()||magic->infor1 != 1502)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    coder.notice("��ٷ�������ʥ����������2����");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), 2, "y", cards);
    this->setCrossNum(getCrossNum()+2);
    coder.crossChangeNotice(this->getID(), getCrossNum());
}
void ShenGuan::ShengShiShouHu(Harm harm, PlayerEntity *src, PlayerEntity *dst, int *crossAvailable)
{
    if(dst!=this)
        return;
    if(crossNum>1)
        *crossAvailable = 1;
}

//ˮ֮����
void ShenGuan::ShuiZhiShenLi(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID()||magic->infor1 != 1503)
        return;
    int dst = magic->dstID;
    coder.notice("��ٶ����"+QString::number(dst)+"����ˮ֮����");

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);

    PlayerEntity* ptr = engine->getPlayerByID(dst);
    if(this->getHandCardNum()!=0)
    {
        coder.askForSkill(this->getID(), "ˮ֮��������");
        cards.clear();
        cards << messageBuffer::readCardID(1);
        this->giveHandCards(cards, ptr);
    }
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    ptr->setCrossNum(ptr->getCrossNum()+1);
    coder.crossChangeNotice(dst, ptr->getCrossNum());
    coder.notice("��ٺ�Ŀ�������1����");
}

//��ʥ��Լ
void ShenGuan::ShenShengQiYue(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||this->getCrossNum()==0||this->getEnergy()==0)
        return;
    coder.askForSkill(this->getID(),"��ʥ��Լ");
    BatInfor start = messageBuffer::readBatInfor();
    if(start.infor2==0)
        return;
    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("��ٶ����"+QString::number(start.dstID)+"������ʥ��Լ��ת��"+QString::number(start.infor3)+"������");
    this->setCrossNum(getCrossNum()-start.infor3);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    PlayerEntity* dst = engine->getPlayerByID(start.dstID);
    dst->setCrossNum(dst->getCrossNum()+start.infor3, 4);
    coder.crossChangeNotice(start.dstID, dst->getCrossNum());
}

//��ʥ����
void ShenGuan::ShenShengLingYu(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID() || magic->infor1!=1505||this->getEnergy()==0)
        return;
    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("��ٶ����"+QString::number(magic->dstID)+"������ʥ����");
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    QList<CardEntity*> cards;
    int n = 0;
    if(magic->CardID!=-1)
    {
        cards << getCardByID(magic->CardID);
        n++;
    }
    if(magic->infor3!=-1)
    {
        cards << getCardByID(magic->infor3);
        n++;
    }
    if(n>0)
    {
        this->removeHandCards(cards,true);
        coder.discardNotice(this->getID(), n, "n", cards);
    }
    if(magic->infor2 == 1)
    {
        this->setCrossNum(getCrossNum()-1);
        coder.crossChangeNotice(this->getID(), getCrossNum());
        coder.notice("����Ƴ�1������");
        Harm harm;
        harm.harmPoint = 2;
        harm.type = MAGIC;
        this->engine->timeLine3(harm,this,dst,"��ʥ����");
    }
    else
    {
        coder.notice("�������2�����ƣ����"+QString::number(magic->dstID)+"����1������");
        this->setCrossNum(getCrossNum()+2);
        coder.crossChangeNotice(this->getID(), getCrossNum());
        dst->setCrossNum(dst->getCrossNum()+1);
        coder.crossChangeNotice(dst->getID(), dst->getCrossNum());
    }
}

void ShenGuan::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(specialFinishSIG(QList<void*>)),this,SLOT(ShenShengQiShi(QList<void*>)));
    connect(engine,SIGNAL(askForHeal(Harm,PlayerEntity*,PlayerEntity*,int*)),this,SLOT(ShengShiShouHu(Harm,PlayerEntity*,PlayerEntity*,int*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShenShengQiFu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShuiZhiShenLi(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(ShenShengQiYue(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShenShengLingYu(QList<void*>)));
}


/*
  ���� 13
  */
SiLing::SiLing(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 13;
    this->star = 3.5;
    //ʥ��
    this->crossMax = 5;
    this->makeConnection(engine);
}
//����
void SiLing::BuXiu(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(SiWangZhiChuUsed)
        return;
    coder.askForSkill(this->getID(), "����");
    if(messageBuffer::readInfor() == 0)
        return;

    this->setCrossNum(crossNum+1);
    coder.crossChangeNotice(this->getID(), crossNum);
    coder.notice("���鷢�������ࡿ������1����");
}

void SiLing::ShengDu(Harm harm, PlayerEntity *src, PlayerEntity *dst, int *crossAvailable)
{
    if(dst!=this)
        return;
    if(harm.type == ATTACK)
        *crossAvailable=0;
}

//����
void SiLing::WenYi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1302)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.notice("���鷨ʦ���������ߡ�");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(),1,"y",cards);
    PlayerEntity* dst = this->getNext();
    do{
        Harm harm;
        harm.harmPoint = 1;
        harm.type = MAGIC;
        this->engine->timeLine3(harm,this,dst,"����");
        if(engine->checkEnd())
            break;
        dst = dst->getNext();
    }while(dst!= this);
}
//����֮��
void SiLing::SiWangZhiChu(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1303)
        return;
    SiWangZhiChuUsed = true;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*> cards;
    for(int i=0;i<magic->infor3;i++)
        cards << getCardByID(cardNum[i].toInt());
    coder.notice("���鷨ʦ�����"+QString::number(magic->dstID)+"����������֮�������Ƴ�"+
                 QString::number(magic->infor2)+"�����ƣ���"+QString::number(magic->infor3)+"��ͬϵ��");
    this->setCrossNum(getCrossNum()-magic->infor2);
    coder.crossChangeNotice(magic->srcID, getCrossNum());
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), magic->infor3, "y", cards);
    Harm harm;
    harm.harmPoint = magic->infor2+magic->infor3-3;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"����֮��");
}

//Ĺ������
void SiLing::MuBeiYunLuo(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID()||magic->infor1 != 1304||getGem()==0)
        return;
    coder.notice("���鷨ʦ������Ĺ�����䡿");
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    PlayerEntity* dst = this->getNext();
    do{
        Harm harm;
        harm.harmPoint = 2;
        harm.type = MAGIC;
        this->engine->timeLine3(harm,this,dst,"Ĺ������");
        if(engine->checkEnd())
            break;
        dst = dst->getNext();
    }while(dst!= this);
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    coder.notice("���鷨ʦ������Ĺ�����䡿������1����");
}

void SiLing::skillReset(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    SiWangZhiChuUsed = false;
}

void SiLing::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(BuXiu(QList<void*>)));
    connect(engine,SIGNAL(askForHeal(Harm,PlayerEntity*,PlayerEntity*,int*)),this,SLOT(ShengDu(Harm,PlayerEntity*,PlayerEntity*,int*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(WenYi(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(SiWangZhiChu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MuBeiYunLuo(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}
XianZhe::XianZhe(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 17;
    this->star = 4;
    this->makeConnection(engine);
    energyMax=4;
}

void XianZhe::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(ZhiHuiFaDian(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(FaShuFanTan(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoDaoFaDian(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShengJieFaDian(QList<void*>)));
}
void XianZhe::ZhiHuiFaDian(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type != MAGICHARM || ((Harm*)args[2])->harmPoint<=3)
        return;
    setGem(gem+2);
    coder.energyNotice(id,gem,crystal);
}

void XianZhe::FaShuFanTan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type != MAGICHARM || ((Harm*)args[2])->harmPoint!=1)
        return;
    if(this->getHandCards().size()<2)
        return;
    coder.askForSkill(id,"��������");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    if(arg[3].toInt() != id || arg[0].toInt() != 1701)
        return;
    QList<CardEntity*>cards;
    for(int i=0;i<arg[4].toInt();i++)
        cards.append(getCardByID(arg[i+5].toInt()));
    coder.discardNotice(id,arg[4].toInt(),"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("���߶����"+QString::number(arg[2].toInt())+"����������������");
    PlayerEntity* dst = engine->getPlayerByID(arg[2].toInt());
    Harm harm;
    harm.harmPoint = (arg[4].toInt())-1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"��������");
    if(engine->checkEnd())
        return;
    Harm harm1;
    harm1.harmPoint = arg[4].toInt();
    harm1.type = MAGIC;
    this->engine->timeLine3(harm1,this,this,"��������");
}

void XianZhe::MoDaoFaDian(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1702)
        return;
    if(this->getGem()==0)
        return;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*>cards;
    for(int i=0;i<magic->infor2;i++)
        cards.append(getCardByID(cardNum[i].toInt()));
    coder.discardNotice(id,magic->infor2,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("���߶����"+QString::number(magic->dstID)+"������ħ�����䡿");
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = (magic->infor2)-1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"ħ������");
    if(engine->checkEnd())
        return;
    this->engine->timeLine3(harm,this,this,"ħ������");
}

void XianZhe::ShengJieFaDian(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1703)
        return;
    if(this->getGem()==0)
        return;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    QStringList cardNum = magic->inforstr.split(":");
    QStringList playerNum = magic->inforstrp.split(":");
    QList<CardEntity*>cards;
    QList<PlayerEntity*>players;
    int i=0;
    int j=0;
    for(i;i<magic->infor2;i++)
        cards.append(getCardByID(cardNum[i].toInt()));
    for(j;j<magic->infor3;j++)
        players.append(engine->getPlayerByID(playerNum[j].toInt()));
    coder.discardNotice(id,magic->infor2,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("���߷�����ʥ�෨�䡿");
    foreach(PlayerEntity*ptr,players)
    {
        ptr->setCrossNum(ptr->getCrossNum()+2);
        coder.crossChangeNotice(ptr->getID(),ptr->getCrossNum());
        coder.notice("����ʹ�á�ʥ�෨�䡿Ϊ���"+TOQSTR(ptr->getID())+"����2������");
    }
    Harm harm;
    harm.harmPoint = (magic->infor2)-1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,this,"ʥ�෨��");
}


GeDouJia::GeDouJia(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=20;
    this->star=4.5;
    tokenMax[0]=6;
    BaiShiTargetID=-1;
    BaiShiUsed=false;
    BaiShiFirst=false;
    StartUsed=false;
    XuLiUesd=false;
    CangYanUsed=false;
    this->makeConnection(engine);
}

void GeDouJia::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(NianQiLiChang(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(CangYanXuLi(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(XuLiYiJi2(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(XuLiYiJi3(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(NianDan(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(BaiShiHuanLingQuan1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(BaiShiHuanLingQuan2(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(BaiShiHuanLingQuan3(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(CangYanZhiHun2(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(DouShenTianQu(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(BaiShiTargetSet(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(BaiShiQuXiao1(QList<void*>)));
    connect(engine,SIGNAL(beforeMagicSIG(QList<void*>)),this,SLOT(BaiShiQuXiao2(QList<void*>)));
    connect(engine,SIGNAL(specialFinishSIG(QList<void*>)),this,SLOT(BaiShiQuXiao3(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

void GeDouJia::NianQiLiChang(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->harmPoint<5)
        return;
    Harm* harm= (Harm*)args[2];
    harm->harmPoint=4;
    coder.notice(tr("�񶷼ҷ���������������"));
}

void GeDouJia::CangYanXuLi(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(id,"��������");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt()==0)
        return;
    if(arg[2].toInt()==0){
    if(BaiShiUsed)
    {
        BaiShiUsed=false;
        setTap(0);
        coder.tapNotice(id,0,"����ͨ��̬��");
    }
    coder.notice(tr("�񶷼ҷ���������һ����"));
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
    XuLiUesd=true;
    }
    else
    {
        setToken(0,token[0]-1);
        coder.tokenNotice(id,0,token[0]);
        coder.notice(tr("�񶷼ҷ���������֮�꡿"));
        *(int*)args[5]=NOREPLY;
        CangYanUsed=true;
    }
}

void GeDouJia::XuLiYiJi2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!XuLiUesd)
        return;
    Harm* harm= (Harm*)args[2];
    harm->harmPoint++;
    XuLiUesd=false;
}

void GeDouJia::XuLiYiJi3(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!XuLiUesd)
        return;
    Harm harm;
    harm.type=MAGICHARM;
    harm.harmPoint=this->getToken(0);
    engine->timeLine3(harm,this,this,"����һ��");
    XuLiUesd=false;
}

void GeDouJia::NianDan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || this->getToken(0)==6)
        return;
    coder.askForSkill(id,"�");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt()==0)
        return;
    coder.notice("�񶷼Ҷ����"+QString::number(arg[2].toInt())+"���������");
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
    PlayerEntity* ptr=engine->getPlayerByID(arg[2].toInt());
    int cross=ptr->getCrossNum();
    Harm harm;
    harm.harmPoint=1;
    harm.type=MAGICHARM;
    engine->timeLine3(harm,this,ptr,"�");
    if(cross==0)
    {
        Harm harm2;
        harm2.harmPoint=this->getToken(0);
        harm2.type=MAGICHARM;
        engine->timeLine3(harm2,this,this,"�");
    }
}

void GeDouJia::BaiShiHuanLingQuan1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || this->getToken(0)<3 || BaiShiUsed || StartUsed)
        return;
    coder.askForSkill(id,"��ʽ����ȭ");
    if(messageBuffer::readInfor() == 0)
        return;
    setToken(0,token[0]-3);
    coder.tokenNotice(id,0,token[0]);
    coder.notice(tr("�񶷼ҷ�������ʽ����ȭ��"));
    setTap(1);
    coder.tapNotice(id,1,"����ʽ����ȭ��");
    BaiShiUsed=true;
    BaiShiFirst=true;
    StartUsed=true;
}

void GeDouJia::BaiShiHuanLingQuan2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    if(!*(bool*)args[4])
        return;
    coder.notice(tr("�񶷼ҷ�������ʽ����ȭ��"));
    Harm* harm=(Harm*)args[2];
    harm->harmPoint+=2;
}

void GeDouJia::BaiShiHuanLingQuan3(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    if(*(bool*)args[4])
        return;
    coder.notice(tr("�񶷼ҷ�������ʽ����ȭ��"));
    Harm* harm=(Harm*)args[2];
    harm->harmPoint++;
}


void GeDouJia::CangYanZhiHun2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !CangYanUsed)
        return;
    Harm harm;
    harm.type=MAGICHARM;
    harm.harmPoint=this->getToken(0);
    if(this->getToken(0)!=0)
        engine->timeLine3(harm,this,this,"����֮��");
    CangYanUsed=false;
}

void GeDouJia::DouShenTianQu(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||StartUsed||this->getEnergy()==0)
        return;
    coder.askForSkill(id,"��������");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    if(this->getCrystal()>0)
        crystal--;
    else
        gem--;
    setGem(gem);
    setCrystal(crystal);
    coder.energyNotice(id,gem,crystal);
    coder.notice(tr("�񶷼ҷ���������������"));
    QList<CardEntity*>cards;
    for(int i=0;i<arg[2].toInt();i++)
        cards.append(getCardByID(arg[i+3].toInt()));
    coder.discardNotice(id,arg[2].toInt(),"n",cards);
    this->removeHandCards(cards,false);
    this->setCrossNum(this->getCrossNum()+2);
    coder.crossChangeNotice(id,this->getCrossNum());
    coder.notice("�񶷼�ʹ�á�����������Ϊ�Լ�����2������");
    StartUsed=true;
}

void GeDouJia::BaiShiTargetSet(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed || !BaiShiFirst)
        return;
    PlayerEntity* dst=(PlayerEntity*)args[1];
    BaiShiTargetID=dst->getID();
    BaiShiFirst=false;
}

void GeDouJia::BaiShiQuXiao1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || BaiShiFirst || !BaiShiUsed)
        return;
    if(!*(bool*)args[4])
        return;
    PlayerEntity*dst=(PlayerEntity*)args[1];
    if(BaiShiTargetID==dst->getID())
        return;
    setTap(0);
    coder.tapNotice(id,0,"����ͨ��̬��");
    BaiShiUsed=false;
    BaiShiFirst=false;
    BaiShiTargetID=-1;
}

void GeDouJia::BaiShiQuXiao2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    setTap(0);
    coder.tapNotice(id,0,"����ͨ��̬��");
    BaiShiTargetID=-1;
    BaiShiUsed=false;
    BaiShiFirst=false;
}

void GeDouJia::BaiShiQuXiao3(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    setTap(0);
    coder.tapNotice(id,0,"����ͨ��̬��");
    BaiShiFirst=false;
    BaiShiUsed=false;
    BaiShiTargetID=-1;
}

void GeDouJia::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    StartUsed=false;
}


WuNv::WuNv(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 23;
    this->star = 5;
    this->makeConnection(engine);
    TongShengID = -1;
    tap = false;
}

void WuNv::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TongShengGongSi(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(XueZhiAiShang(QList<void*>)));
    connect(engine,SIGNAL(handCardsChange(PlayerEntity*)),this,SLOT(ToPuTongXingtai(PlayerEntity*)));
    connect(engine,SIGNAL(trueLoseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(ToLiuXueXingTai(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(LiuXue(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(NiLiu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XueZhiBeiMing(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XueZhiZuZhou(QList<void*>)));
}

void WuNv::TongShengGongSi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2301)
        return;
    coder.notice("��Ů�����"+QString::number(magic->dstID)+"������ͬ��������");
    engine->drawCards(2, 0, this);
    TongShengID = magic->dstID;
    coder.specialNotice(TongShengID,3,1);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    int change;
    if(tap)
        change = 1;
    else
        change = -2;
    if(TongShengID!=this->getID())
    {
        this->addHandCardsRange(change);
        coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
    }
    dst->addHandCardsRange(change);
    coder.handcardMaxNotice(dst->getID(),dst->getHandCardMax());
}

void WuNv::XueZhiAiShang(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||TongShengID==-1)
        return;
    coder.askForSkill(this->getID(),"Ѫ֮����");
    BatInfor start = messageBuffer::readBatInfor();
    if(start.infor2 == 0)
        return;

    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,this,"Ѫ֮����");

    PlayerEntity* oldDst = engine->getPlayerByID(TongShengID);
    int change;
    if(start.dstID == -1)
    {
        coder.notice("��Ůȡ����ͬ��������");
        coder.specialNotice(TongShengID,3,0);
        TongShengID=-1;
        if(tap)
            change = -1;
        else
            change = 2;
        if(TongShengID!=this->getID())
        {
        this->addHandCardsRange(change);
        coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
        }
        oldDst->addHandCardsRange(change);
        coder.handcardMaxNotice(oldDst->getID(),oldDst->getHandCardMax());
    }
    else
    {
        PlayerEntity* newDst = engine->getPlayerByID(start.dstID);
        coder.notice("��Ů����ͬ��������ת�������"+QString::number(start.dstID));
        coder.specialNotice(TongShengID,3,0);
        if(tap)
            change = -1;
        else
            change = 2;
        if(TongShengID!=this->getID())
        {
            oldDst->addHandCardsRange(change);
            coder.handcardMaxNotice(oldDst->getID(),oldDst->getHandCardMax());
        }
        TongShengID = start.dstID;
        coder.specialNotice(TongShengID,3,1);
        if(tap)
            change = 1;
        else
            change = -2;
        if(TongShengID!=this->getID())
        {
            newDst->addHandCardsRange(change);
            coder.handcardMaxNotice(newDst->getID(), newDst->getHandCardMax());
        }
    }
}

void WuNv::ToPuTongXingtai(PlayerEntity *dst)
{
    if(this != dst)
        return;
    if(this->getHandCardNum()>2||!tap)
        return;
    tap = false;
    coder.tapNotice(this->getID(),0,"����ͨ��̬��");
    if(TongShengID!=-1)
    {
        PlayerEntity* dst = engine->getPlayerByID(TongShengID);
        if(TongShengID!=this->getID())
        {
            this->addHandCardsRange(-3);
            coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
        }
        dst->addHandCardsRange(-3);
        coder.handcardMaxNotice(dst->getID(),dst->getHandCardMax());
    }
}

void WuNv::ToLiuXueXingTai(int harmed, int *howMany, PlayerEntity* dst)
{
    if(dst!=this||tap||harmed == 0||*howMany<=0)
        return;
    tap = true;
    coder.tapNotice(this->getID(),1,"����Ѫ��̬��");
    if(TongShengID!=-1)
    {
        PlayerEntity* dst = engine->getPlayerByID(TongShengID);
        if(TongShengID!=this->getID())
        {
            this->addHandCardsRange(3);
            coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
        }
        dst->addHandCardsRange(3);
        coder.handcardMaxNotice(dst->getID(),dst->getHandCardMax());
    }
    this->setCrossNum(this->getCrossNum()+1);
    coder.crossChangeNotice(this->getID(),this->getCrossNum());
}

void WuNv::LiuXue(QList<void *> args)
{
    if(this != (PlayerEntity*) args[0]||!tap)
        return;
    Harm harm;
    harm.harmPoint = 1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,this,"��Ѫ");
}

void WuNv::NiLiu(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2303)
        return;
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    coder.notice("��Ů����������������2���ƣ�����1����");
    this->removeHandCards(cards,false);
    coder.discardNotice(this->getID(), 2, "n", cards);
    this->setCrossNum(this->getCrossNum()+1);
    coder.crossChangeNotice(this->getID(),this->getCrossNum());
}

void WuNv::XueZhiBeiMing(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2304)
        return;
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.notice("��Ů�����"+QString::number(magic->dstID)+"������Ѫ֮������");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(),1,"y",cards);
    Harm harm;
    harm.harmPoint = magic->infor2;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,engine->getPlayerByID(magic->dstID),"Ѫ֮����");
    engine->timeLine3(harm,this,this,"Ѫ֮����");
}

void WuNv::XueZhiZuZhou(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2305)
        return;

    gem--;
    coder.energyNotice(id,gem,crystal);
    coder.notice("��Ů�����"+QString::number(magic->dstID)+"����Ѫ֮����");

    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,dst,"Ѫ֮����");

    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*> cards;
    if(magic->infor2>0)
    {
        for(int i=0;i<magic->infor2;i++)
            cards << getCardByID(cardNum[i].toInt());
        this->removeHandCards(cards,false);
        coder.discardNotice(this->getID(), magic->infor2, "n", cards);
    }
}

//��� ps.�ٲá������˳����ӷ����㲥 �񶷡����,������������С��
LingHun::LingHun(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=22;
    this->star=4.5;
    tokenMax[0]=6;
    tokenMax[1]=6;
    LianJieID=-1;
    LianJieUsed=false;
    StartUsed=false;
    LianJieChuFa=false;
    HeCheng=false;
    this->makeConnection(engine);
}

void LingHun::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(trueLoseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(LingHunTunShi(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(specialFinishSIG(QList<void*>)),this,SLOT(LingHunTunShi2(QList<void*>)));
    connect(engine,SIGNAL(loseMoraleHeChengSIG(int,int*,PlayerEntity*)),this,SLOT(LingHunTunShi3(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunZhaoHuan(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunJingXiang(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunZhenBao(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunCiYu(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(LingHunZhuanHuan(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(LingHunLianJie1(QList<void*>)));
    connect(engine,SIGNAL(timeLine6SIG(QList<void*>)),this,SLOT(LingHunLianJie2(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(LingHunLianJie3(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(LingHunZengFu(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

void LingHun::LingHunTunShi(int harmed, int *howMany, PlayerEntity *dst)
{
    if(dst->getColor()!=color || *howMany==0)
        return;
    setToken(0,token[0]+*howMany);
    coder.tokenNotice(id,0,token[0]);
    coder.notice(tr("�����ʿ������������ɡ�"));
}

void LingHun::LingHunTunShi2(QList<void *> args)
{
    PlayerEntity* ptr=(PlayerEntity*)args[0];
    if(ptr->getColor()==this->getColor())
        return;
    HeCheng=true;
}

void LingHun::LingHunTunShi3(int harmed, int *howMany, PlayerEntity *dst)
{
    if(!HeCheng)
        return;
    HeCheng=false;
    if(dst->getColor()==color)
        return;
    setToken(0,token[0]+*howMany);
    coder.tokenNotice(id,0,token[0]);
    coder.notice(tr("�����ʿ������������ɡ�"));
}

void LingHun::LingHunZhaoHuan(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2201)
        return;
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*>cards;
    for(int i=0;i<magic->infor2;i++)
        cards.append(getCardByID(cardNum[i].toInt()));
    coder.discardNotice(id,magic->infor2,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice(tr("�����ʿ����������ٻ���"));
    setToken(1,token[1]+1+magic->infor2);
    coder.tokenNotice(id,1,token[1]);
}

void LingHun::LingHunJingXiang(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2202)
        return;
    setToken(0,token[0]-2);
    coder.tokenNotice(id,0,token[0]);
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*>cards;
    coder.notice(tr("�����ʿ��������꾵��"));
    if(magic->infor2 != 0)
    {
        for(int i=0;i<magic->infor2;i++)
            cards.append(getCardByID(cardNum[i].toInt()));
        coder.discardNotice(id,magic->infor2,"y",cards);
        this->removeHandCards(cards,true);
    }
    else
    {
        coder.discardNotice(this->getID(),this->getHandCards().length(),"n",this->getHandCards());
        this->removeHandCards(this->getHandCards(),false,true);
    }
    PlayerEntity* dst=engine->getPlayerByID(magic->dstID);
    int drawNum=3;
    if(dst->getHandCardMax()-dst->getHandCardNum()>=3)
        engine->drawCards(drawNum,0,dst);
    else
    {
        drawNum=dst->getHandCardMax()-dst->getHandCardNum();
        engine->drawCards(drawNum,0,dst);
    }
}

void LingHun::LingHunZhenBao(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2203)
        return;
    setToken(0,token[0]-3);
    coder.tokenNotice(id,0,token[0]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("�����ʿ�����"+QString::number(magic->dstID)+"����������𱬡�");
    PlayerEntity* dst=engine->getPlayerByID(magic->dstID);
    Harm harm;
    if(dst->getHandCardNum()<3 && dst->getHandCardMax()>5)
        harm.harmPoint=5;
    else
        harm.harmPoint=3;
    harm.type=MAGICHARM;
    engine->timeLine3(harm,this,dst,"�����");
}

void LingHun::LingHunCiYu(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2204)
        return;
    setToken(1,token[1]-3);
    coder.tokenNotice(id,1,token[1]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("�����ʿ�����"+QString::number(magic->dstID)+"�����������衿");
    PlayerEntity* dst=engine->getPlayerByID(magic->dstID);
    dst->setGem(dst->getGem()+2);
    coder.energyNotice(magic->dstID,dst->getGem(),dst->getCrystal());
}

void LingHun::LingHunZhuanHuan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(this->getToken(0)==0 && this->getToken(1)==0)
        return;
    coder.askForSkill(id,"���ת��");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    coder.notice(tr("�����ʿ���������ת����"));
    if(arg[2].toInt() == 0)
    {
        setToken(0,token[0]-1);
        setToken(1,token[1]+1);
        coder.tokenNotice(id,0,token[0]);
        coder.tokenNotice(id,1,token[1]);
        coder.notice(tr("�����ʿת��1��ƻ�"));
    }
    else
    {
        setToken(0,token[0]+1);
        setToken(1,token[1]-1);
        coder.tokenNotice(id,0,token[0]);
        coder.tokenNotice(id,1,token[1]);
        coder.notice(tr("�����ʿת��1������"));
    }
}

void LingHun::LingHunLianJie1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(this->getToken(0)==0 || this->getToken(1)==0)
        return;
    if(LianJieUsed || StartUsed)
        return;
    coder.askForSkill(id,"�������");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    setToken(0,token[0]-1);
    setToken(1,token[1]-1);
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
    LianJieID=arg[2].toInt();
    coder.notice("�����ʿ�����"+QString::number(LianJieID)+"������������ӡ�");
    coder.specialNotice(LianJieID,2,1);
    LianJieUsed=true;
    StartUsed=true;
}

void LingHun::LingHunLianJie2(QList<void *> args)
{
    if(LianJieChuFa || !LianJieUsed)
        return;
    Harm* harm=(Harm*)args[2];
    PlayerEntity* dst=(PlayerEntity*)args[1];
    PlayerEntity* src=(PlayerEntity*)args[0];
    if(dst->getID()!=this->getID() && dst->getID()!=LianJieID)
        return;
    if(this->getToken(1)==0)
        return;
    if(harm->harmPoint==0)
        return;
    coder.askForSkill(id,"�������",QString::number(harm->harmPoint));
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0 || arg[2].toInt() == 0)
        return;
    int howMany=arg[2].toInt();
    coder.notice("�����ʿ������������ӡ���ת��"+QString::number(howMany)+"���˺�");
    LianJieChuFa=true;
    setToken(1,token[1]-howMany);
    coder.tokenNotice(id,1,token[1]);
    harm->harmPoint-=howMany;
    bool flag1,flag2;
    flag1=flag2=true;
    PlayerEntity* ptr = engine->getNext(src);
    do
    {
        if(flag1 && ptr->getID()==id)
        {
            flag1=false;
            Harm zhuanyi1;
            zhuanyi1.type=MAGICHARM;
            zhuanyi1.harmPoint=howMany;
            Harm zhuanyi2;
            zhuanyi2.type=harm->type;
            zhuanyi2.harmPoint=harm->harmPoint;
            if(dst->getID()==this->getID())
                engine->timeLine6(zhuanyi2,src,this);
            else
                engine->timeLine6(zhuanyi1,src,this);
            if(engine->checkEnd())
                break;
            LianJieChuFa=true;
        }
        else if(flag2 && ptr->getID()==LianJieID)
        {
            flag2=false;
            Harm zhuanyi1;
            zhuanyi1.type=MAGICHARM;
            zhuanyi1.harmPoint=howMany;
            Harm zhuanyi2;
            zhuanyi2.type=harm->type;
            zhuanyi2.harmPoint=harm->harmPoint;
            if(dst->getID()==this->getID())
                engine->timeLine6(zhuanyi2,src,engine->getPlayerByID(LianJieID));
            else
                engine->timeLine6(zhuanyi1,src,engine->getPlayerByID(LianJieID));
            if(engine->checkEnd())
                break;
            LianJieChuFa=true;
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
    harm->harmPoint=0;
    LianJieChuFa=false;
}

void LingHun::LingHunLianJie3(QList<void *> args)
{
    if(!LianJieChuFa)
        return;
    PlayerEntity* dst=(PlayerEntity*)args[1];
    if(dst->getID()!=this->getID() && dst->getID()!=LianJieID)
        return;
    LianJieChuFa=false;
}

void LingHun::LingHunZengFu(QList<void *> args)
{
    if(StartUsed)
        return;
    if(this->getGem()==0)
        return;
    if(this != (PlayerEntity*)args[0])
        return;
    coder.askForSkill(id,"�������");
    if(messageBuffer::readInfor() == 0)
        return;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice(tr("�����ʿ���������������"));
    StartUsed=true;
    setToken(0,token[0]+2);
    setToken(1,token[1]+2);
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
}

void LingHun::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    StartUsed=false;
}

//������ʿ 28
HongLian::HongLian(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 28;
    this->star = 4;
    //�ɺ�����
    crossMax = 4;
    tokenMax[0]=2;
    XingHongShengYueUsed = false;
    tap = false;
    this->makeConnection(engine);
}

void HongLian::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(XingHongShengYue(QList<void*>)));
    connect(engine,SIGNAL(askForHeal(Harm,PlayerEntity*,PlayerEntity*,int*)),this,SLOT(XingHongXinYang(Harm,PlayerEntity*,PlayerEntity*,int*)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(XueXingDaoYan(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(ShaLuShengYan(QList<void*>)));
    connect(engine,SIGNAL(trueLoseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(ToReXueFeiTeng(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(beforeLoseMoralSIG(int,int*,PlayerEntity*)),this,SLOT(ReXueFeiTeng(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(turnEndPhaseSIG(PlayerEntity*)),this,SLOT(OutReXueFeiTeng(PlayerEntity*)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(JieJiaoJieZao1(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(JieJiaoJieZao1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JieJiaoJieZao2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XingHongShiZi(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

void HongLian::XingHongShengYue(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    if(XingHongShengYueUsed)
        return;
    coder.askForSkill(this->getID(), "�ɺ�ʥԼ");
    if(messageBuffer::readInfor() == 0)
        return;

    XingHongShengYueUsed = true;
    this->setCrossNum(crossNum+1);
    coder.crossChangeNotice(this->getID(), crossNum);
    coder.notice("������ʿ�������ɺ�ʥԼ��������1����");
}

void HongLian::XingHongXinYang(Harm harm, PlayerEntity *src, PlayerEntity *dst, int *crossAvailable)
{
    if(dst!=this)
        return;
    if(src!=this)
        *crossAvailable = 0;
}

void HongLian::XueXingDaoYan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||this->getCrossNum()<=0)
        return;
    coder.askForSkill(this->getID(),"Ѫ�ȵ���");
    BatInfor start = messageBuffer::readBatInfor();
    if(start.infor2 == 0||(start.infor3+start.infor5)<=0)
        return;
    setCrossNum(crossNum-start.infor3-start.infor5);
    coder.crossChangeNotice(this->getID(),crossNum);

    Harm harm;
    harm.harmPoint = start.infor3+start.infor5;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,this,"Ѫ�ȵ���");

    PlayerEntity* dst = engine->getPlayerByID(start.dstID);
    dst->setCrossNum(dst->getCrossNum()+start.infor3);
    coder.crossChangeNotice(start.dstID,dst->getCrossNum());
    if(start.infor4!=-1)
    {
        dst = engine->getPlayerByID(start.infor4);
        dst->setCrossNum(dst->getCrossNum()+start.infor5);
        coder.crossChangeNotice(start.infor4,dst->getCrossNum());
    }
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);

}

void HongLian::ShaLuShengYan(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0])||token[0]<=0)
        return;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(this->getID(),"ɱ¾ʢ��");
    if(messageBuffer::readInfor() == 0)
        return;

    coder.notice("������ʿ������ɱ¾ʢ�硿");
    setToken(0,token[0]-1);
    coder.tokenNotice(this->getID(),0,token[0]);

    Harm selfHarm;
    selfHarm.harmPoint = 4;
    selfHarm.type = MAGIC;
    engine->timeLine3(selfHarm,this,this,"ɱ¾ʢ��");

    Harm *harm = (Harm*)args[2];
    harm->harmPoint += 2;
}

void HongLian::ToReXueFeiTeng(int harmed, int *howMany, PlayerEntity *dst)
{
    if(dst!=this||tap||harmed == 0||*howMany<=0)
        return;
    tap = true;
    coder.tapNotice(this->getID(),1,"����Ѫ����״̬��");
}

void HongLian::ReXueFeiTeng(int harmed, int *howMany, PlayerEntity *dst)
{
    if(dst!=this||!tap||harmed == 0||*howMany<=0)
        return;
    *howMany = 0;
    coder.notice("������ʿ���ڡ���Ѫ����״̬���������˺������ʿ���½�");
}

void HongLian::OutReXueFeiTeng(PlayerEntity * currunt)
{
    if(currunt!=this||!tap)
        return;
    tap = false;
    coder.tapNotice(this->getID(),0,"����ͨ��̬��");

    this->setCrossNum(crossNum+2);
    coder.crossChangeNotice(this->getID(), crossNum);
    coder.notice("������ʿ���롾��Ѫ����״̬��������2����");
}

void HongLian::JieJiaoJieZao1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0])||this->getEnergy() <= 0||!tap)
        return;
    coder.askForSkill(this->getID(),"�佾����");
    if(messageBuffer::readInfor() == 0)
        return;
    coder.notice("������ʿ�������佾���꡿");
    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),gem,crystal);

    tap = false;
    coder.tapNotice(this->getID(),0,"����ͨ��̬��");
    engine->addActionNum(ATTACKORMAGIC);
}

void HongLian::JieJiaoJieZao2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=2802)
        return;
    coder.notice("������ʿʹ�á��佾���꡿�Ĺ��������ж�");
}

void HongLian::XingHongShiZi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2803||getEnergy()<=0)
        return;

    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("������ʿ�����"+QString::number(magic->dstID)+"�������ɺ�ʮ�֡�");
    setToken(0,token[0]-1);
    coder.tokenNotice(this->getID(),0,token[0]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), 2, "y", cards);

    Harm selfHarm;
    selfHarm.harmPoint = 4;
    selfHarm.type = MAGIC;
    engine->timeLine3(selfHarm,this,this,"�ɺ�ʮ��");

    Harm harm;
    harm.harmPoint = 3;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,engine->getPlayerByID(magic->dstID),"�ɺ�ʮ��");
}

void HongLian::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    XingHongShengYueUsed = false;
}


LingFu::LingFu(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 18;
    this->star = 4;
    this->makeConnection(engine);

}

void LingFu::nianZhou(QList<void *> args)
{
    if(this->coverCards.count() >= 2)
        return;
    coder.askForSkill(this->getID(),"����");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;
    coder.notice("���ʦ���������䡿");

    CardEntity* yaoLi = getCardByID(ans.CardID);
    QList<CardEntity*> yaoLis;
    yaoLis << yaoLi;

    engine->moveCardFrom(yaoLi);
    engine->moveCardToCover(yaoLi,this->getID());
    coder.moveCardNotice(1,yaoLis,this->getID(),HAND,this->id,COVERED);
    coder.coverCardNotice(this->id,1,yaoLis,false,false);

    this->setToken(2,this->getCoverCards().count());
    coder.tokenNotice(this->getID(),2,this->getCoverCards().count());
}

void LingFu::leiMing(QList<void *> args)
{
    BatInfor* action = (BatInfor*) args[0];
    if(action->srcID != this->id)
        return;
    if(action->infor1 != 1802)
        return;
    coder.notice("���ʦ���������-������");

    CardEntity* card = getCardByID(action->CardID);
    QList<CardEntity*> cards;
    cards << card;
    this->removeHandCards(cards,true);
    coder.discardNotice(this->id,1,"y",cards);

    Harm harm;
    harm.type = MAGIC;
    harm.harmPoint = 1;
    this->lingLiBengJie(harm);

    PlayerEntity* target = this;
    for(int counter = 0;counter < 2;target = target->getNext())
    {
        if(target->getID() != action->infor2 && target->getID() != action->infor3)
            continue;
        engine->timeLine3(harm,this,target,"���-����");
        counter++;
    }

    this->nianZhou(args);
}

void LingFu::fengXing(QList<void *> args)
{
    BatInfor* action = (BatInfor*) args[0];
    if(action->srcID != this->id)
        return;
    if(action->infor1 != 1803)
        return;
    coder.notice("���ʦ���������-���С�");

    CardEntity* card = getCardByID(action->CardID);
    QList<CardEntity*> cards;
    cards << card;
    this->removeHandCards(cards,true);
    coder.discardNotice(this->id,1,"y",cards);

    Harm harm;
    harm.type = MAGIC;
    harm.harmPoint = 1;

    this->lingLiBengJie(harm);
    PlayerEntity* target = this;
    for(int counter = 0;counter < 2;target = target->getNext())
    {
        if(target->getID() != action->infor2 && target->getID() != action->infor3)
            continue;
        counter++;

        if(target->getHandCards().count() == 0)
            continue;
        coder.askForDiscard(target->getID(),1,"n");

        QList<CardEntity*> cardChosen;
        cardChosen = messageBuffer::readCardID(1);
        target->removeHandCards(cardChosen,false);
        coder.discardNotice(target->getID(),1,"n",cardChosen);

    }

    this->nianZhou(args);
}

void LingFu::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(leiMing(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(fengXing(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(baiGuiYeXing(QList<void*>)));
}

void LingFu::lingLiBengJie(Harm &hurt)
{
    if(this->getEnergy() <= 0)
        return;
    coder.askForSkill(this->getID(),"��������");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;

    coder.notice("���ʦ�������������⡿");
    if(ans.infor1 == 0)
        this->setCrystal(this->getCrystal()-1);
    else
        this->setGem(this->getGem()-1);
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());

    hurt.harmPoint++;
}

void LingFu::baiGuiYeXing(QList<void *> args)
{
    PlayerEntity *src = (PlayerEntity*)args[0];
    bool isActive = *(bool*)args[4];
    if(src->getID() != this->getID())
        return;
    if(!isActive)
        return;
    if(this->getCoverCards().count() == 0)
        return;

    coder.askForSkill(this->getID(),"�ٹ�ҹ��");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;

    coder.notice("���ʦ�������ٹ�ҹ�С�");

    CardEntity *yaoLi = getCardByID(ans.CardID);
    QList<CardEntity*> yaoLis;
    yaoLis << yaoLi;
    PlayerEntity *dst1,*dst2;
    BatInfor ans2;




    if(yaoLi->getElement() == "fire")
    {
        coder.askForSkillNumber(this->getID(),180501);
        ans2 = messageBuffer::readBatInfor();
        dst1 = engine->getPlayerByID(ans2.infor1);
        if(ans2.reply != 0)
        {
            engine->moveCardFromCoverToDiscard(yaoLi,true);
            this->setToken(2,this->getCoverCards().count());
            coder.tokenNotice(this->getID(),2,this->getCoverCards().count());
            coder.coverCardNotice(this->getID(),1,yaoLis,true,true);
            dst2 = engine->getPlayerByID(ans2.infor2);
            PlayerEntity *target = this;

            Harm hurt;
            hurt.type = MAGIC;
            hurt.harmPoint = 1;
            this->lingLiBengJie(hurt);

            for(int counter = 0;counter < engine->getPlayerNum();counter++,target = target->getNext())
            {
                if(target->getID() == dst1->getID() || target->getID() == dst2->getID())
                    continue;
                engine->timeLine3(hurt,this,target,"�ٹ�ҹ��");
            }
            return;
        }
    }
    else
    {
        coder.askForSkillNumber(this->getID(),180503);
        int dstID = messageBuffer::readInfor();
        dst1 = engine->getPlayerByID(dstID);
    }

    engine->moveCardFromCoverToDiscard(yaoLi,false);
    this->setToken(2,this->getCoverCards().count());
    coder.tokenNotice(this->getID(),2,this->getCoverCards().count());
    coder.coverCardNotice(this->getID(),1,yaoLis,true,false);

    Harm hurt;
    hurt.type = MAGIC;
    hurt.harmPoint = 1;
    this->lingLiBengJie(hurt);
    engine->timeLine3(hurt,this,dst1,"�ٹ�ҹ��");

}
