#ifndef ANIMATION_H
#define ANIMATION_H
#include "RoomView.h"
#include "CardItem.h"
#include <QPixmap>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsObject>
#include <QObject>
#include <QMutex>

class PictureContainer:public QGraphicsObject
{
public:
    PictureContainer();
    void setPixmap(QString picSrc){pixmap.load(picSrc);width = pixmap.width();height = pixmap.height();}
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QPixmap pixmap;
    int width;
    int height;
};

class Animation:QObject
{
    Q_OBJECT
public:
    Animation();
    void setRoomScene(RoomScene *view);

    //���ϣ����������Ч����item����ʱ���ɣ�����֮�����Զ���ʧ�ģ�һ��Ҫ��new������ʱ����װ������б�������ڴ�й¶
    //������������߳�������ȴ���һ��ʹ����ʱ����Ķ�����Ŀǰֻ��itemFlash�����֮��������һ�ε���ʱ����Ȼ�󷵻�������ʱ���������
    QList<QGraphicsObject*>* getTempItems();

    //��QGraphicsObject���������˸Ч��
    //QGraphicsObject����һ��Ҫװ��getTempItems�������ص�QList�У�
    //x��y��ϣ����˸�Ķ�������ĵ�λ�ã�ע�ⲻ�����Ͻ�λ�ã�
    QParallelAnimationGroup* itemFlash(QGraphicsObject* item,int x,int y);   

public slots:
    //itemFlash����֮�������������һ����˸Ч����ȡ��ʱ�����б�
    void unlock();
private:
    RoomScene *view;
    QPropertyAnimation *posAni,*scaleAni,*opacityAni;
    QMutex lock;
    bool busy;
    QList<QGraphicsObject*> tempItems;
};

extern Animation *animation;
#endif // ANIMATION_H
