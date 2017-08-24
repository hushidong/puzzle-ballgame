#include "boxitem.h"
#include <QCursor>
#include <QKeyEvent>
#include <qDebug>

boxitem::boxitem()
{
    //setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void boxitem::setboxid(int id)//用于设置盒子(框)序号
{
    boxid=id;
}

void boxitem::mousePressEvent(QGraphicsSceneMouseEvent * event)//鼠标点击事件处理
{
    Q_UNUSED(event);
    setFocus();
    setCursor(Qt::ClosedHandCursor);
    emit tojudgesitdown(boxid);//发送信号，用于落位判断
    //qDebug()<<"focus:"<<hasFocus();
    //qDebug()<<"focus:"<<focusItem();
    //qDebug()<<"emit:"<<"box:tojudgesitdown";
}

void boxitem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}
