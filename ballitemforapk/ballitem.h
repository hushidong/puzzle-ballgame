#ifndef BALLITEM_H
#define BALLITEM_H

#include <QObject>
#include <QGraphicsEllipseItem>
//#include <QPainter> //该类也需要加入，否则出现不完整qpainter的错误提示

class ballitem : public QObject, public QGraphicsEllipseItem
{

    Q_OBJECT

public:
    ballitem(QObject *parent =0);
    ~ballitem();
    //void setColor(const QColor &color);
    //QRectF boundingRect() const;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setshapeid(int id);
    void setballid(int id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void keyPressEvent(QKeyEvent * event);

signals:
    void rotateormirror(int transid);
    void ballpressed(int ballid,int shapeid);
    void ballpressedright(int ballid,int shapeid);


private:
    int ballid; //标记当前在形状中的序号，是第几个球,从1开始计数
    int shapeid;//标注当前球属于哪个形状，从1开始计数
    //    QColor brushcolor;

};

#endif // BALLITEM_H
