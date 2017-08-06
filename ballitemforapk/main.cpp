//#include "mainwindow.h"
#include "ballitem.h"
#include "ballshape.h"
#include "ballgrid.h"
#include "sceneinfotrans.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <qDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    sceneinfotrans scene;

    QPushButton *buttonstart=new QPushButton(QObject::tr("开始"));
    QPushButton *buttonexample=new QPushButton(QObject::tr("示例"));
    QPushButton *buttonset=new QPushButton(QObject::tr("设置"));
    QPushButton *buttonpart=new QPushButton(QObject::tr("重新划分"));
    QLabel * labelgridnum=new QLabel(QObject::tr("网格大小"));
    QLabel * labellinknum=new QLabel(QObject::tr("链接球数"));
    QLineEdit * textgridnum=new QLineEdit("10");
    QLineEdit * textlinknum=new QLineEdit("5");

    labelgridnum->setMaximumHeight(50);
    labellinknum->setMaximumHeight(50);
    textgridnum->setMaximumWidth(100);//设置最大尺度长度为50像素
    textlinknum->setMaximumWidth(100);

    buttonstart->setMaximumHeight(50);
    buttonexample->setMaximumHeight(50);
    buttonset->setMaximumHeight(50);
    buttonpart->setMaximumHeight(50);


    QLabel * label_nshapes=new QLabel(QObject::tr("形状数量"));
    QLabel * labeldiameter=new QLabel(QObject::tr("球的尺寸"));
    QLineEdit * text_nshapes=new QLineEdit("0");
    QLineEdit * textdiameter=new QLineEdit("70");
    label_nshapes->setMaximumHeight(50);
    labeldiameter->setMaximumHeight(50);
    text_nshapes->setMaximumWidth(100);//设置最大尺度长度为50像素
    textdiameter->setMaximumWidth(100);

    label_nshapes->setMaximumHeight(50);
    labeldiameter->setMaximumHeight(50);
    text_nshapes->setMaximumHeight(50);
    textdiameter->setMaximumHeight(50);

    QPushButton *turnright=new QPushButton(QObject::tr("向右旋转"));
    QPushButton *turnleft=new QPushButton(QObject::tr("向左旋转"));
    QPushButton *mirrorab=new QPushButton(QObject::tr("上下翻转"));
    QPushButton *resetpos=new QPushButton(QObject::tr("移出网格"));

    turnright->setMaximumHeight(50);
    mirrorab->setMaximumHeight(50);
    turnleft->setMaximumHeight(50);
    resetpos->setMaximumHeight(50);

    QGraphicsWidget *button1=scene.addWidget(buttonstart);//用代理的方式将部件加入场景中
    QGraphicsWidget *button2=scene.addWidget(buttonexample);
    QGraphicsWidget *button3=scene.addWidget(buttonset);
    QGraphicsWidget *button4=scene.addWidget(buttonpart);

    QGraphicsWidget *button5=scene.addWidget(turnright);//用代理的方式将部件加入场景中
    QGraphicsWidget *button6=scene.addWidget(turnleft);
    QGraphicsWidget *button7=scene.addWidget(mirrorab);
    QGraphicsWidget *button8=scene.addWidget(resetpos);

    QGraphicsWidget *label1=scene.addWidget(labelgridnum);
    QGraphicsWidget *label2=scene.addWidget(labellinknum);
    QGraphicsWidget *text1=scene.addWidget(textgridnum);
    QGraphicsWidget *text2=scene.addWidget(textlinknum);

    QGraphicsWidget *label3=scene.addWidget(label_nshapes);
    QGraphicsWidget *label4=scene.addWidget(labeldiameter);
    QGraphicsWidget *text3=scene.addWidget(text_nshapes);
    QGraphicsWidget *text4=scene.addWidget(textdiameter);

    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    layout->addItem(button1, 0, 0);
    layout->addItem(button2, 0, 1);
    QGraphicsLinearLayout *left=new QGraphicsLinearLayout;
    left->addItem(label1);
    left->addItem(text1);

    QGraphicsLinearLayout *right=new QGraphicsLinearLayout;
    right->addItem(label2);
    right->addItem(text2);

    QGraphicsLinearLayout *left1=new QGraphicsLinearLayout;
    left1->addItem(label3);
    left1->addItem(text3);

    QGraphicsLinearLayout *right1=new QGraphicsLinearLayout;
    right1->addItem(label4);
    right1->addItem(text4);

    layout->addItem(left, 1, 0);
    layout->addItem(right, 1, 1);
    layout->addItem(button3, 2, 1);
    layout->addItem(button4, 2, 0);
    layout->addItem(left1, 3, 0);
    layout->addItem(right1, 3, 1);
    layout->addItem(button5, 4, 1);
    layout->addItem(button6, 5, 1);
    layout->addItem(button7, 6, 1);
    layout->addItem(button8, 7, 1);
    //layout->setMaximumHeight(400);
    //layout->setMaximumWidth(200);

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    scene.addItem(form);
    form->setPos(textdiameter->text().toInt()*4,textdiameter->text().toInt()/2);
    form->resize(200,300);
    QFont fonta;
    fonta.setPixelSize(30);
    form->setFont(fonta);

    ballgrid gridballs;

    QObject::connect(&gridballs,&ballgrid::shapecompleted,text_nshapes,&QLineEdit::setText);
    QObject::connect(buttonstart,&QPushButton::clicked,&gridballs,&ballgrid::gamestart);
    QObject::connect(buttonexample,&QPushButton::clicked,&gridballs,&ballgrid::gameexample);
    QObject::connect(textgridnum,&QLineEdit::textChanged,&gridballs,&ballgrid::dealgridnumtext);
    QObject::connect(textlinknum,&QLineEdit::textChanged,&gridballs,&ballgrid::deallinknumtext);
    QObject::connect(textdiameter,&QLineEdit::textChanged,&gridballs,&ballgrid::setballdiameter);
    QObject::connect(buttonset,&QPushButton::clicked,&gridballs,&ballgrid::gameset);
    QObject::connect(buttonpart,&QPushButton::clicked,&gridballs,&ballgrid::gamerepart);

    QObject::connect(turnright,&QPushButton::clicked,&gridballs,&ballgrid::shapeturnright);
    QObject::connect(turnleft,&QPushButton::clicked,&gridballs,&ballgrid::shapeturnleft);
    QObject::connect(mirrorab,&QPushButton::clicked,&gridballs,&ballgrid::shapemirrorab);
    QObject::connect(resetpos,&QPushButton::clicked,&gridballs,&ballgrid::shaperesetpos);

    if(gridballs.getflag()==0 && gridballs.getgridnum()==10)
    {
        gridballs.setgrid(gridballs.getgridnum(),gridballs.getlinknum());
        gridballs.setboxes();
        gridballs.setshapesdefault(gridballs.getntotal());
    }

    gridballs.setworkscene(scene);
    gridballs.addboxestoscene();
    gridballs.addtoscenedefault();

    QGraphicsView view(&scene);
    view.resize(750,650);
    view.show();

    return a.exec();
}
