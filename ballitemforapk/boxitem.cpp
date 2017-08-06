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

void boxitem::setboxid(int id)
{
    boxid=id;
}

void boxitem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event);
    setFocus();
    setCursor(Qt::ClosedHandCursor);
    emit tojudgesitdown(boxid);
    //qDebug()<<"focus:"<<hasFocus();
    //qDebug()<<"focus:"<<focusItem();
    //qDebug()<<"emit:"<<"box:tojudgesitdown";
}

void boxitem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}
