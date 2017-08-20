#ifndef BALLGRID_H
#define BALLGRID_H

#include "shape.h"
#include "ballshape.h"
#include <QList>
#include <QGraphicsScene>
#include "boxitem.h"
#include "sceneinfotrans.h"
#include <QObject>
#include <QTime>
#include <QTimer>
#include <QtXml>

class ballgrid : public QObject,  public grid
{

        Q_OBJECT

public:
    ballgrid();

    void setshapesdefault(int arraysize);//利用预存的默认划分形状，利用继承的n_scale，n_n_balls_ofshape信息选择不同情况下的默认划分
    void setshapes(int arraysize);//利用划分算法生成划分形状
    void addtoscenedefault();//将各划分形状加入到场景中
    void setshaperandposdefault();//将形状移动到框外面，以便开始游戏
    void removeshapes();//将各划分形状移出场景

    void setboxes();//生成空位(方框)网格
    void addboxestoscene();//将方框网格加入场景
    void removeboxes();//将方框网格移出场景
    int getboxidat(QPointF posa);//根据场景中的位置获取当前位置上的方框的索引，从0开始计数

    int getgridnum();//获取网格大小信息gridnum
    int getlinknum();//获取划分形状的预设球数信息linknum
    int getflag();//获取网格大小和划分形状的预设球数改变的标志信息flaggridchanged
    void setworkscene(sceneinfotrans &scene);//保存当前所在的场景信息
    void isinrank(int gridnumber,int linknumber,float gametime);//判断本次游戏成绩是否进入排行榜，进入则修改成绩记录xml

public slots:
    void judgesitdown(int boxid);//根据接收到盒子信息去判断能否落位
    void dealshapetosit(int ballid,int shapeida); //记录一下接收到的要落位的形状的信息
    void shapereset(int ballid,int shapeida); //重新恢复形状在网格外的原位置
    void shapetrans(int shapeida,int transid);//对当前形状进行旋转等操作
    void shapeturnright(bool a);//按钮控制向右旋转
    void shapeturnleft(bool a);//按钮控制向左旋转
    void shapemirrorab(bool a);//按钮控制上下翻转
    void shapemirrorlr(bool a);//按钮控制左右翻转
    void shaperesetpos(bool a);//按钮控制移出网格
    void gamestart(bool a);//游戏开始，将所有形状移动到网格外
    void gameexample(bool a);//游戏示例，将所有形状恢复填入到网格中
    void gameset(bool a);//当网格大小和划分形状的预设球数改变时，重设游戏
    void gamerepart(bool a);//对当前网格大小和划分形状的预设球数下利用算法重新进行划分
    void dealgridnumtext(QString text1);//根据界面输入信息记录网格大小
    void deallinknumtext(QString text1);//根据界面输入信息记录划分形状的预设球数
    void setballdiameter(QString text1);//根据界面输入信息记录球和方框的直径
    void recordtime();//记录时间,利用timer不断的显示流逝的游戏时间
    void showcredit();//显示成绩排行榜

signals:
    void shapecompleted(QString a);//当形状重新划分时，划分完成的信号
    void timerecorded(QString a);//记录一下游戏时间

private:
    QList<ballshape *> ballshapesingrid;//算法划分的形状图形的信息
    QList<ballshape *> ballshapesingriddefault;//默认保存的形状图形的信息
    QList<boxitem *> boxesingrid;//用于绘制网格中的框,注意只能用指针列表，而不能是实际列表，因为boxitem没有深复制
    //的确也没有必要拷贝，用指针可以指向它们

    int shapeidtosit,ballidtosit;//记录当前需要落位的形状序号及其中的焦点球(即选中的球)序号
    int gridnum,linknum;//记录输入的网格大小和链接形状的球数
    int flaggridchanged;//记录是否更改了网格大小和链接球数
    int balldiameter;//设置球的直径
    sceneinfotrans *sceneonwork;//当前网格所要加入的场景，注意用指针是避免传值
    int gamestarted;//记录游戏开始，=1表示已开始，才会做落位判断

    QTime timecount;
    QTimer timera;

    QDomDocument doc;
    float timegame;//用于记录本次游戏时间

};

#endif // BALLGRID_H
