#include "ballitem.h"
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <qDebug>

ballitem::ballitem(QObject *parent)
{
    //setFlag(QGraphicsItem::ItemIsMovable);
    Q_UNUSED(parent);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

ballitem::~ballitem()
{
}

void ballitem::setshapeid(int id)
{
     shapeid=id;
}

void ballitem::setballid(int id)
{
     ballid=id;
}


void ballitem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if(event->button()==Qt::LeftButton) { //event->button()返回当前点击的键
        setFocus();
        setCursor(Qt::ClosedHandCursor);
        emit ballpressed(ballid,shapeid);
        //qDebug()<<"focus:"<<hasFocus();
        //qDebug()<<"focus:"<<focusItem();
        //qDebug()<<"emit:"<<"ballpressed";
    }
    if(event->button()==Qt::RightButton) { //event->button()返回当前点击的键
        setCursor(Qt::ClosedHandCursor);
        emit ballpressedright(ballid,shapeid);
        //qDebug()<<"focus:"<<focusItem();
        //qDebug()<<"emit:"<<"ballpressedright";
    }


}

void ballitem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}

void ballitem::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
        case Qt::Key_Right:
            emit rotateormirror(1);
            break;

        case Qt::Key_Left:
            emit rotateormirror(2);
            break;

        case Qt::Key_Up:
            emit rotateormirror(3);
            break;

        case Qt::Key_Down:
            emit rotateormirror(4);
            break;

        default:
            break;
    }
}


//QRectF ballitem::boundingRect() const
//{
//    qreal penWidth=1;
//    return QRectF(0-penWidth/2,0-penWidth/2,50+penWidth,50+penWidth);
//}


//void ballitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    painter->setBrush(brushcolor);
//    painter->drawEllipse(0,0,50,50);
//}

//void ballitem::setColor(const QColor &color)
//{
//    brushcolor=color;
//}
