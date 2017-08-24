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
    void setshapeid(int id);//设置当前球所属形状的id号
    void setballid(int id);//设置当前球在所属形状中的序号

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void keyPressEvent(QKeyEvent * event);

signals:
    void rotateormirror(int transid);//形状旋转或翻转信号，transid表示项左，右旋转，上下，左右翻转
    void ballpressed(int ballid,int shapeid);//当前球和形状被选中信号
    void ballpressedright(int ballid,int shapeid);//当前球所在形状移出网格的信号


private:
    int ballid; //标记当前在形状中的序号，是第几个球,从1开始计数
    int shapeid;//标记当前球属于哪个形状，从1开始计数
    //    QColor brushcolor;

};

#endif // BALLITEM_H
