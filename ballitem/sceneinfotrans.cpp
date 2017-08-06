#include "sceneinfotrans.h"
#include <qDebug>
#include "ballgrid.h"
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsGridLayout>


sceneinfotrans::sceneinfotrans()
{

//    QPushButton *buttonstart=new QPushButton(QObject::tr("开始"));
//    QPushButton *buttonexample=new QPushButton(QObject::tr("示例"));
//    QPushButton *buttonset=new QPushButton(QObject::tr("设置"));
//    QLabel * labelgridnum=new QLabel(QObject::tr("网格大小"));
//    QLabel * labellinknum=new QLabel(QObject::tr("链接球数"));
//    QLineEdit * textgridnum=new QLineEdit("10");
//    QLineEdit * textlinknum=new QLineEdit("5");

//    QGraphicsWidget *button1=this->addWidget(buttonstart);
//    QGraphicsWidget *button2=this->addWidget(buttonexample);
//    QGraphicsWidget *button3=this->addWidget(buttonset);
//    QGraphicsWidget *label1=this->addWidget(labelgridnum);
//    QGraphicsWidget *label2=this->addWidget(labellinknum);
//    QGraphicsWidget *text1=this->addWidget(textgridnum);
//    QGraphicsWidget *text2=this->addWidget(textlinknum);

//    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
//    layout->addItem(button1, 0, 0);
//    layout->addItem(button2, 0, 1);
//    layout->addItem(label1, 1, 0);
//    layout->addItem(label2, 1, 1);
//    layout->addItem(text1, 2, 0);
//    layout->addItem(text2, 2, 1);
//    layout->addItem(button3, 3, 1);

//    QGraphicsWidget *form = new QGraphicsWidget;
//    form->setLayout(layout);
//    this->addItem(form);
//    form->setPos(200,50);

//    qDebug()<<buttonstart->text();
//    //qDebug()<<button1->text();
//    //QPushButton *buttona = this->findChild<QPushButton *>("button1");
//    qDebug()<<this->findChild<QGraphicsWidget *>();

}

void sceneinfotrans::scenetobeset(bool a)
{
    Q_UNUSED(a);
    qDebug()<<"clicked";
   //emit toshowgrid(*this);

//   ballgrid gridballs;

//   gridballs.setgrid(10,5);
//   gridballs.setboxes();
//   //gridballs.addboxestoscene(scene);
//   //使用默认的划分方案
//   gridballs.setshapesdefault(gridballs.getntotal());
//   gridballs.setshaperandposdefault();

   //gridballs.gamestart(*this);
}
