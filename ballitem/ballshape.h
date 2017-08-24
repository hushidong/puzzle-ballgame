#ifndef BALLSHAPE_H
#define BALLSHAPE_H

#include <QObject>
#include "shape.h"
#include "ballitem.h"
#include <QGraphicsScene>
#include <QList>
#include "sceneinfotrans.h"


class ballshape : public QObject
{
    Q_OBJECT

public:
    ballshape(QObject *parent =0);
    ~ballshape();
    void setballshape(int id,int n_ballsinshape,int ballpos[],QColor colora);//构成当前形状
    void addtoscene(sceneinfotrans &scene);//将形状加入场景
    void removeballs(sceneinfotrans &scene);//将形状移出场景
    void setballpos(int i,int posx,int posy);//设置索引为i的球位置，i从0开始计数
    void recordpos();//记录一下游戏开始时球放在网格外的坐标位置
    void resetpos();//恢复一下游戏开始时球放在网格外的坐标位置
    QPointF getballpos(int sn);//得到球真实序号为sn的坐标位置,sn从1开始计数
    void setballdiameter(int diameter);//设置球的直径

private:
    int shapeid;//当前形状的id
    int n_balls;//当前形状内的球数
    QList<ballitem *> balls;//当前形状内球的列表
    QList<QPointF> ballposinit;//游戏开始前放在框(网格)外的坐标位置
    int balldiameter;//当前形状内球的直径

signals:
    void shapeselected(int ballid,int shapeida);//当前形状被选中，则传递给ballgird
    void shapetoreset(int ballid,int shapeida);//当前形状被选中,需要还原为原来在网格外的位置，则传递给ballgird
    void shapetotrans(int shapeida,int transid);//当前形状被选中，需要做旋转，翻转变换

public slots://链接的槽与信号的参数应是一致的。
    void transexec(int transid);//根据键盘信息旋转调整形状时的处理，主要是向ballgird传递消息
    void ballselected(int ballid,int shapeida);//当前形状和当前球被选中时的处理，主要是向ballgird传递消息
    void ballstoresetpos(int ballid,int shapeida);//当前形状被选中,需要还原为原来的位置的处理，主要是向ballgird传递消息

};

#endif // BALLSHAPE_H
