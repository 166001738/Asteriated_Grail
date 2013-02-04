#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "PlayerEntity.h"
class BackgroundEntity;
//��սʿ
class Berserker :public PlayerEntity
{
    Q_OBJECT
public :
    Berserker(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //��
    void wild(QList<void*> args);
    //˺��
    void tear(QList<void*> args);
    //ѪӰ��
    void wildBlade(QList<void*> args);
    void wildBladeAsk(QList<void*> args);
    //Ѫ������
    void roar(QList<void*> args);
signals:
private:
    bool wildBladeUsed;
};

//��֮Ů��
class BowLady : public PlayerEntity
{
    Q_OBJECT
public :
    BowLady(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //�����
    void thunderArrow(QList<void*> args);
    //�ᴩ���
    void transfixtion(QList<void*> args);
    //��������
    void trap(QList<void*> args);
    //��׼���
    void accurateShoot(QList<void*> args);
    //�ѻ�
    void snipe(QList<void*> args);
    void snipe2(QList<void*> args);
signals:
private:

};

class MoDao : public PlayerEntity
{
    Q_OBJECT
public :
    MoDao(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void MoDanRongHe(QList<void*> args);
    void MoBaoChongJi(QList<void*> args);
    void HuiMeiFengBao(QList<void*> args);
signals:
private:
    \
};

class AnSha : public PlayerEntity
{
    Q_OBJECT
public :
    AnSha(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void ShuiYing(QList<void*> args);
    void QianXing(QList<void*> args);
    void FanShi(QList<void*> args);
    void AttackCheck(QList<void*> args);
signals:
private:
    \
};

class TianShi : public PlayerEntity
{
    Q_OBJECT
public :
    TianShi(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void FengZhiJieJin(QList<void*> args);
    void TianShiZhuFu(QList<void*> args);
    void TianShiZhiQiang(QList<void*> args);
    void TianShiZhiGe(QList<void*> args);
    void ShenZhiBiHu(int harmed,int*howMany,PlayerEntity*dst);
    void TianShiJiBan(int userID);
signals:
private:
    \
};
class Saintness : public PlayerEntity
{
    Q_OBJECT
public:
    Saintness(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    //��˪����
    void prayerOfFrost(QList<void*> args);
    //������
    void cure(QList<void*> args);
    //����֮��
    void healingLight(QList<void*> args);
    //����
    void mercy(QList<void*> args);
    //ʥ��
    void layOnHands(QList<void*> args);
    void layOnHands2(QList<void*> args);
private:
    bool layOnHandsUsed;
};

//��ʥ
class JianSheng :public PlayerEntity
{
    Q_OBJECT
public :
    JianSheng(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //�ҷ缼
    void LieFengJi1(QList<void*> args);
    void LieFengJi2(QList<void*> args);
    //���缼
    void JiFengJi1(QList<void*> args);
    void JiFengJi2(QList<void*> args);
    //������
    void LianXuJi1(QList<void*> args);
    void LianXuJi2(QList<void*> args);
    //��Ӱ
    void JianYing1(QList<void*> args);
    void JianYing2(QList<void*> args);
    //ʥ��
    void ShengJian(QList<void*> args);
    void skillReset(QList<void*> args);

private:
    int attackCount;
    bool LieFengJiUsed,LianXuJiUsed,JianYingUsed;
};

//��ӡ
class FengYin : public PlayerEntity
{
    Q_OBJECT
public :
    FengYin(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //��������
    void FaShuJiDang1(QList<void*> args);
    void FaShuJiDang2(QList<void*> args);
    //��ӡ����
    void FengYinFaShu1(QList<void*> args);
    void FengYinFaShu2(QList<CardEntity *> cards, PlayerEntity *user);
    //��ϵ����
    void WuXiShuFu1(QList<void*> args);
    void WuXiShuFu2(PlayerEntity*,bool*);
    //��ӡ����
    void FengYinPoSui(QList<void*> args);

private:
    int shuFuID;
};

//ħ��
class MoJian :public PlayerEntity
{
    Q_OBJECT
public :
    MoJian(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //�ڰ����
    void HeiAnZhenChan1(QList<void*> args);
    void HeiAnZhenChan2(QList<void*> args);
    void HeiAnZhenChan3(QList<void *> args);
    //������ն
    void XiuLuoLianZhan1(QList<void*> args);
    void XiuLuoLianZhan2(QList<void*> args);
    //��Ӱ����
    void AnYingLiuXing(QList<void*> args);
    //��Ӱ����
    void AnYingNingJu(QList<void*> args);
    //��Ӱ֮��
    void AnYingZhiLi(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool HeiAnZhenChanUsed,XiuLuoLianZhanUsed,isHeiAnZhenChan;
};
//ð��
class MaoXian : public PlayerEntity
{
    Q_OBJECT
public :
    MaoXian(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //��թ
    void QiZha(QList<void*> args);
    //͵�컻��
    void TouTianHuanRi(QList<void*> args);
    void TouTianHuanRi2(QList<void*> args);
    //����ӹ�
    void TeShuJiaGong(QList<void*> args);
    void TeShuJiaGong2(QList<void*> args);
    //ð��������
    void MaoXianZheTianTang(QList<void*> args);
signals:
private:
    \
};


//Ԫ��
class YuanSu : public PlayerEntity
{
    Q_OBJECT
public :
    YuanSu(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //Ԫ������
    void YuanSuXiShou(QList<void*> args);
    //Ԫ�ط���
    void YuanSuFaShu(QList<void*> args);
    void YuanSuFaShu2(QList<void*> args);
    //Ԫ�ص�ȼ
    void YuanSuDianRan(QList<void*> args);
    void YuanSuDianRan2(QList<void*> args);
    //�¹�
    void YueGuang(QList<void*> args);

signals:
private:
    bool ignite;
    \
};
//�ٲ�
class ZhongCai : public PlayerEntity
{
    Q_OBJECT
public:
    ZhongCai(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    //��ʽ�ж�
    void YiShiZhongDuan(QList<void*> args);
    //ĩ������
    void MoRiShenPan(QList<void*> args);
    //�����˳�
    void ShenPanLangChao(QList<void*> args);
    //�ٲ���ʽ
    void ZhongCaiYiShi1(QList<void*> args);
    void ZhongCaiYiShi2(QList<void *> args);
    //�о���ƽ
    void PanJueTianPing(QList<void*> args);
    //������������
    void skillReset(QList<void*> args);
signals:
private:
    bool startUsed;
};

//����
class YongZhe : public PlayerEntity
{
    Q_OBJECT
public:
    YongZhe(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    //ŭ��
    void NuHou(QList<void*> args);
    void NuHou1(QList<void*> args);
    void NuHou2(QList<void*> args);
    //����ֹˮ
    void MingJingZhiShui(QList<void*> args);
    //����֮��
    void JinDuanZhiLi1(QList<void*> args);
    void JinDuanZhiLi2(QList<void*> args);
    //����
    void SiDou(QList<void*> args);
    //����
    void TiaoXin1(QList<void*> args);
    void TiaoXin2(PlayerEntity*,int*, bool *canGiveUp);
    void TiaoXin3(PlayerEntity*);
    //void TiaoXin4(QList<void *> args);
    //��ƣ����
    void JingPiLiJie1(QList<void*> args);
    void JingPiLiJie2(QList<void *> args);
    void JingPiLiJie3(QList<void *> args);
private:
    int tiaoXinID,fireNum;
    bool nuHouUsed,tiaoXinUsed,tiaoXinChuFa;
    bool jinDuanUsed;
};

//ʥǹ
class ShengQiang: public PlayerEntity
{
    Q_OBJECT
public:
    ShengQiang(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void HuiYao(QList<void*> args);
    void ChengJie(QList<void*> args);
    void HuiYao2(QList<void*> args);
    void ChengJie2(QList<void*> args);
    void ShengJi(QList<void*> args);
    void TianQiang(QList<void*> args);
    void DiQiang(QList<void*> args);
    void ShengGuangQiYu(QList<void*> args);
    void ShengGuangQiYu2(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool ShengGuangQiYuUsed;
    bool TianQiangUsed;
};
//��ʦ
class QiDao : public PlayerEntity
{
    Q_OBJECT
public :
    QiDao(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //�����͸�
    void WeiLiCiFu1(QList<void*> args);
    void WeiLiCiFu2(QList<void*> args);
    //Ѹ�ݴ͸�
    void XunJieCiFu1(QList<void*> args);
    void XunJieCiFu2(QList<void*> args);
    void XunJieCiFu3(QList<void *> args);
    //�������
    void GuangHuiXinYang(QList<void*> args);
    //�������
    void QiHeiXinYang(QList<void*> args);
    //������ϫ
    void FaLiChaoXi1(QList<void*> args);
    void FaLiChaoXi2(QList<void*> args);
    //���� ��
    void Pray1(QList<void*> args);
    void Pray2(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool FaLiChaoXiUsed;
};

//���
class ShenGuan: public PlayerEntity
{
    Q_OBJECT
public:
    ShenGuan(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void ShenShengQiShi(QList<void*> args);
    void ShenShengQiFu(QList<void*> args);
    void ShuiZhiShenLi(QList<void*> args);
    void ShenShengQiYue(QList<void*> args);
    void ShenShengLingYu(QList<void*> args);
};

//����
class SiLing: public PlayerEntity
{
    Q_OBJECT
public:
    SiLing(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void BuXiu(QList<void*> args);
    void WenYi(QList<void*> args);
    void SiWangZhiChu(QList<void*> args);
    void MuBeiYunLuo(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool SiWangZhiChuUsed;
};
//����
class XianZhe : public PlayerEntity
{
    Q_OBJECT
public:
    XianZhe(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void ZhiHuiFaDian(QList<void*> args);
    void FaShuFanTan(QList<void*> args);
    void MoDaoFaDian(QList<void*> args);
    void ShengJieFaDian(QList<void*> args);
private:
};

//��Ů
class WuNv: public PlayerEntity
{
    Q_OBJECT
public:
    WuNv(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void TongShengGongSi(QList<void*> args);
    void XueZhiAiShang(QList<void*> args);
    void ToPuTongXingtai(QList<void*> args);
    void ToLiuXueXingTai(int harmed, int howMany, PlayerEntity* dst);
    void LiuXue(QList<void*> args);
    void NiLiu(QList<void*> args);
    void XueZhiBeiMing(QList<void*> args);
    void XueZhiZuZhou(QList<void*> args);
private:
    int TongShengID;
};

#endif // CHARACTERS_H
