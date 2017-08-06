#include "ballshape.h"
#include <qDebug>
//#include <cstdlib>

ballshape::ballshape(QObject *parent)
{
    Q_UNUSED(parent);
}

ballshape::~ballshape()
{}

void ballshape::setballdiameter(int diameter)//设置球的直径
{
    balldiameter=diameter;
}

void ballshape::setballshape(int id,int n_ballsinshape,int ballpos[],QColor colora)//注意输入的是实际的位置信息
{
    shapeid=id;
    n_balls=n_ballsinshape;

    //QColor colora(qrand() % 256, qrand() % 256, qrand() % 256);

    for(int i=0;i<n_balls;i++)
    {
        ballitem *balla=new ballitem();
        balla->setshapeid(id);
        balla->setballid(i+1);
        balla->setRect(0, 0,balldiameter, balldiameter);
        balla->setBrush(colora);
        balla->setPos(ballpos[2*i],ballpos[2*i+1]);
        balls.append(balla);
        QObject::connect(balla,&ballitem::rotateormirror,this,&ballshape::transexec);
        QObject::connect(balla,&ballitem::ballpressed,this,&ballshape::ballselected);
        QObject::connect(balla,&ballitem::ballpressedright,this,&ballshape::ballstoresetpos);
    }
}

void ballshape::setballpos(int i,int posx,int posy)
{
    balls[i]->setPos(posx,posy);
}

void ballshape::recordpos()//
{
    for(int i=0;i<n_balls;i++)
    {
        ballposinit.append(balls[i]->pos());
    }
}

void ballshape::resetpos()//
{
    for(int i=0;i<n_balls;i++)
    {
       balls[i]->setPos(ballposinit[i]);
    }
}

QPointF ballshape::getballpos(int i) //i是球的真实序号
{
    return balls[i-1]->pos();
}


void ballshape::addtoscene(sceneinfotrans &scene)
{
    for(int i=0;i<n_balls;i++)
    {
        scene.addItem(balls[i]);//balls[0]本身是一个指针数组，所以直接用其值即可
    }
}

void ballshape::removeballs(sceneinfotrans &scene)
{
    for(int i=0;i<n_balls;i++)
    {
        scene.removeItem(balls[i]);//balls[0]本身是一个指针数组，所以直接用其值即可
    }
}

void ballshape::transexec(int transid)
{
     //qDebug()<<"transid="<<transid;
     //qDebug()<<"recieve transid from ball";
     emit shapetotrans(shapeid,transid);
}

void ballshape::ballselected(int ballid,int shapeida)//
{
    //qDebug()<<"ballid="<<ballid<<" shapeid"<<shapeida;
    emit shapeselected(ballid,shapeida);
    //qDebug()<<"shape emit info to grid";

}

void ballshape::ballstoresetpos(int ballid,int shapeida)//
{
    //qDebug()<<"ballid="<<ballid<<" shapeid"<<shapeida;
    emit shapetoreset(ballid,shapeida);
    //qDebug()<<"shape emit info to grid";
}




