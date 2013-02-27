#include "Animation.h"
#include "data/DataInterface.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QCoreApplication>
#include <QThread>
#include <QList>
#include <QMutex>
Animation *animation = NULL;

PictureContainer::PictureContainer():QGraphicsObject()
{
    width = 0;
    height = 0;
}

QRectF PictureContainer::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void PictureContainer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, pixmap);
}


Animation::Animation():QObject()
{   
    busy = false;
}

//���ϣ����������Ч����item����ʱ���ɣ�����֮�����Զ���ʧ�ģ�һ��Ҫ��new������ʱ����װ������б�������ڴ�й¶
//������������߳�������ȴ���һ��ʹ����ʱ����Ķ�����Ŀǰֻ��itemFlash�����֮��������һ�ε���ʱ����Ȼ�󷵻�������ʱ���������
QList<QGraphicsObject*>* Animation::getTempItems()
{
    while(busy)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
    }

    QGraphicsObject* ptr;
    foreach(ptr,tempItems)
    {
        delete ptr;
    }
    tempItems.clear();
    return &tempItems;
}

//itemFlash����֮�������������һ����˸Ч����ȡ��ʱ�����б�
void Animation::unlock()
{
    busy = false;
}

void Animation::setRoomScene(RoomScene *view)
{
    this->view = view;
}

//��QGraphicsObject���������˸Ч��
//QGraphicsObject����һ��Ҫװ��getTempItems�������ص�QList�У�
//x��y��ϣ����˸�Ķ�������ĵ�λ�ã�ע�ⲻ�����Ͻ�λ�ã�
QParallelAnimationGroup* Animation::itemFlash(QGraphicsObject*  item, int x, int y)
{

    int w = item->boundingRect().width();
    int h = item->boundingRect().height();
    this->view->addItem(item);

    QPropertyAnimation *posAni = new QPropertyAnimation(view);
    QPropertyAnimation *scaleAni = new QPropertyAnimation(view);
    QPropertyAnimation *opacityAni = new QPropertyAnimation(view);

    posAni->setTargetObject(item);
    posAni->setPropertyName("pos");
    posAni->setDuration(1000);
    posAni->setStartValue(QPoint(x - w/2,y-h/2));

    scaleAni->setTargetObject(item);
    scaleAni->setPropertyName("scale");
    scaleAni->setDuration(1000);
    scaleAni->setStartValue(1);

    opacityAni->setTargetObject(item);
    opacityAni->setPropertyName("opacity");
    opacityAni->setDuration(1000);
    opacityAni->setStartValue(0.7);

    w *= 3;
    h *= 3;
    posAni->setEndValue(QPoint(x - w/2,y-h/2));
    scaleAni->setEndValue(3);
    opacityAni->setEndValue(0);

    QParallelAnimationGroup *paraAniGroup = new QParallelAnimationGroup();
    paraAniGroup->addAnimation(posAni);
    paraAniGroup->addAnimation(scaleAni);
    paraAniGroup->addAnimation(opacityAni);
    connect(paraAniGroup,SIGNAL(finished()),this,SLOT(unlock()));
    busy = true;
    return paraAniGroup;

}
