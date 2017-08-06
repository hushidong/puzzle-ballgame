#ifndef BOXITEM_H
#define BOXITEM_H

#include <QObject>
#include <QGraphicsRectItem>

class boxitem : public QObject, public QGraphicsRectItem
{

    Q_OBJECT

private:
    int boxid;//盒子(方框，空位)序号，从1开始计数

public:
    boxitem();
    void setboxid(int id);//用于设置盒子(框)序号

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

signals:
    void tojudgesitdown(int boxid);
};

#endif // BOXITEM_H
