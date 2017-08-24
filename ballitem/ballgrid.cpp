#include "ballgrid.h"
#include <qDebug>
#include <QMessageBox>

#include <QInputDialog>
#include <QTableWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDir>

ballgrid::ballgrid()
{
    shapeidtosit=0; //
    ballidtosit=0;
    gridnum=10;
    linknum=5;
    flaggridchanged=0;
    flagdiamchanged=0;
    flaggametimeon=0;
    flagexampleon=0;
    balldiameter=40;
    gamestarted=0;
    timera.setInterval(1000);
    QObject::connect(&timera,&QTimer::timeout,this,&ballgrid::recordtime);//
    player=new QMediaPlayer;


    //读取或创建一个xml文件用于记录游戏成绩
    QString a=QCoreApplication::applicationDirPath();
    QString filepatha=a+"/info.xml";
    QFile file(filepatha);
    if(file.exists())
    {
        if (!file.open(QIODevice::ReadOnly))
            return;
        if (!doc.setContent(&file)) {
            file.close();
            return;
        }
    }else
    {
        if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)) return;
        QTextStream out(&file);
        out <<"<?xml version='1.0' encoding='UTF-8'?>\n";
        out <<"<scores>\n";
        out <<"<record user='zs' gridnum='10' linknum='5' id='1'>500</record>\n";
        out <<"</scores>\n";
        file.close();
        if (!file.open(QIODevice::ReadOnly))
            return;
        if (!doc.setContent(&file)) {
            file.close();
            return;
        }
    }
    file.close();
}

void ballgrid::setworkscene(sceneinfotrans &scene)
{
    sceneonwork=&scene;
}

int ballgrid::getgridnum()
{
   return  gridnum;
}
int ballgrid::getlinknum()
{
    return  linknum;
}
int ballgrid::getflag()
{
   return  flaggridchanged;
}

void ballgrid::setballdiameter(QString text1)
{
    balldiameter=text1.toInt();
    flagdiamchanged=1;
    gamestarted=0;//当球的直径改变后，因为涉及到球的重新绘制，因此将不能进一步游戏，只能重新设置或重新划分。所以这里设置gamestarted=0
}

void ballgrid::dealgridnumtext(QString text1)
{
    gridnum=text1.toInt();
    flaggridchanged=1;
    //qDebug()<<"gridnum:"<<gridnum;
}

void ballgrid::deallinknumtext(QString text1)
{
    linknum=text1.toInt();
    flaggridchanged=1;
    //qDebug()<<"linknum:"<<linknum;
}

void ballgrid::recordtime()
{
    int msa=timecount.elapsed();
    float sa=msa/1000.0;
    timegame=sa;
    emit timerecorded(QString("%1").arg(sa));//发送信号，显示到界面中的文本框中
}

void ballgrid::gamestart(bool a)
{
    Q_UNUSED(a);
    if(flaggridchanged==0 && flagdiamchanged==0) {
        setshaperandposdefault();
        player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
        player->play();
        cleargridoccp();
        gamestarted=1;
        if(flaggametimeon==0) {//计时器开始工作，只有当新游戏开始时才计时
            timecount.start();//开始时间记录
            timera.start();
            flaggametimeon=1;
        }
    }else{
        if(flaggridchanged==1)
        {
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText("Grid has been changed, need to partition, \nplease pull reset or reparition button");
            msgBox.exec();
        }
        if(flagdiamchanged==1)
        {
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText("Ball diameter has been changed, need to reset, \nplease pull reset or reparition button");
            msgBox.exec();
        }
    }
}

void ballgrid::gameexample(bool a)
{
    Q_UNUSED(a);
    if(flagdiamchanged==0) {//当球的直径未修改时，示例才能正确
        if(flaggametimeon==1){
            if(flagexampleon==0) {
                for(int i=0;i<n_ofshapes;i++)
                {
                    //int id=veclinkshape[i].getshapesn();
                    int n_ballsinshape=veclinkshape[i].getshapenballs();
                    //qDebug()<<"id:"<<id;
                    //qDebug()<<"n_ball_inshape:"<<n_ballsinshape;

                    for(int j=0;j<n_ballsinshape;j++)//直接根据记录的球的原始的行列信息重设位置
                    {
                        int row=veclinkshape[i].getptroworig(j);
                        int col=veclinkshape[i].getptcolorig(j);
                        ballshapesingriddefault[i]->setballpos(j,col*balldiameter,row*balldiameter);
                    }
                }
                player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
                player->play();
                gamestarted=0;
                flagexampleon=1;
            }else{
                setshaperandposdefault();
                player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
                player->play();
                cleargridoccp();
                gamestarted=1;
                flagexampleon=0;
            }
        }else{
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText("Game is not started, need not to show example,\nplease pull the start button!");
            msgBox.exec();
        }
    }else
    {
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("Ball diameter is changed, need to reset,\nplease pull the reset or repartition button!");
        msgBox.exec();
    }
}

void ballgrid::gameset(bool a)
{
    Q_UNUSED(a);
    player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
    player->play();
    if(flaggridchanged==1 || flagdiamchanged==1)//只有网格大小或链接球数改变了才做
    {
        qDebug()<<"do new partition";
        removeboxes();
        removeshapes();
        setgrid(gridnum,linknum);
        setboxes();
        addboxestoscene();
        setshapesdefault(n_total);
        addtoscenedefault();
    }else
    {
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("Grid not changed,need not to partition");
        msgBox.exec();
    }
    flaggridchanged=0;
    flagdiamchanged=0;
    gamestarted=0;
    if(flaggametimeon==1) {//计时器开始工作，只有当新游戏开始时才计时
        timera.stop();
        flaggametimeon=0;
    }
}

void ballgrid::gamerepart(bool a)
{
    Q_UNUSED(a);
    player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
    player->play();
    flaggridchanged=1;
    flagdiamchanged=1;
    qDebug()<<"redo new partition";
    removeboxes();
    removeshapes();
    setgrid(gridnum,linknum);
    setboxes();
    addboxestoscene();
    setshapesdefault(n_total);
    qDebug()<<"n_ofshapes="<<n_ofshapes;
    qDebug()<<"n_total="<<n_total;
    addtoscenedefault();
    flaggridchanged=0;
    flagdiamchanged=0;
    gamestarted=0;
    if(flaggametimeon==1) {//计时器开始工作，只有当新游戏开始时才计时
        timera.stop();
        flaggametimeon=0;
    }
}


void ballgrid::setboxes()//根据setgrid生成的框生成对应的boxitem，并绘制所有的框
{
    //boxitem boxb[n_total];
    boxesingrid.clear();
    for(int i=0;i<n_total;++i)
    {
        boxitem *boxa=new boxitem();
        boxa->setRect(0, 0,balldiameter,balldiameter);
        boxa->setBrush(Qt::lightGray);//cyan
        coord boxpos=getrowcolformsn(i+1);
        boxa->setPos(boxpos.col*balldiameter,boxpos.row*balldiameter);
        boxa->setboxid(i+1);
        boxesingrid.append(boxa);
        QObject::connect(boxa,&boxitem::tojudgesitdown,this,&ballgrid::judgesitdown);
    }
    //drawboxes();
}

void ballgrid::addboxestoscene()//将绘制的框显示到场景中
{
    int n_boxes=boxesingrid.size();
    for(int i=0;i<n_boxes;++i)
    {
        sceneonwork->addItem(boxesingrid[i]);
    }
}

int ballgrid::getboxidat(QPointF posa) //根据当前位置确定box的索引，从0开始计数
{
    int flagingrid=0;
    int i=0;
    for(i=0;i<n_total;++i)
    {
        QPointF posb=boxesingrid[i]->pos();
        if(QLineF(posa,posb).length() <0.00001)
        {
            flagingrid=1;
            break;
        }
    }
    if(flagingrid==0) //当不在网格内时返回网格中总的框数加1
    {
        return n_total+1;
    }else
    {
        return i;
    }
}

void ballgrid::removeboxes()//将绘制的框从场景中移除
{
    int n_boxes=boxesingrid.size();
    for(int i=0;i<n_boxes;++i)
    {
        sceneonwork->removeItem(boxesingrid[i]);
    }
}

void ballgrid::judgesitdown(int boxid)//点击选中box时的形状落位判断
{
    if(gamestarted==1) {
        qDebug()<<"recieved info from box:"<<boxid;
        //是否有shape要放的信息，有的话，根据shape信息调用
        int flagyestosit=0;
        int n_balls=0;
        if(shapeidtosit>0) {//判断是否存在shape需要落位
            QPointF posa=ballshapesingriddefault[shapeidtosit-1]->getballpos(ballidtosit);
            int boxida=getboxidat(posa)+1;//当前球所在的位置
            if(boxida <= n_total)//当在网格内时
            {
                clearshapeoccp(boxida,ballidtosit,shapeidtosit);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
            }
            qDebug()<<"ball in box id:"<<boxida;
            qDebug()<<"ball id:"<<ballidtosit;
            qDebug()<<"shape id:"<<shapeidtosit;

            setfocus(boxid);
            n_balls=veclinkshape[shapeidtosit-1].getshapenballs();
            std::vector<coord> ballscoords=veclinkshape[shapeidtosit-1].getvec_coord();
            flagyestosit=canbesetdown(n_balls,ballidtosit, ballscoords);//根据焦点球序号以及形状中焦点和坐标确定形状能否放入网格中
        }

        //如果能放入则发送信息给shape，接着shape调整位置，//即绘图
        if(flagyestosit>0) {
            int rowfocusinshape=veclinkshape[shapeidtosit-1].getptrow(ballidtosit-1);
            int colfocusinshape=veclinkshape[shapeidtosit-1].getptcol(ballidtosit-1);
            int rowfocusingrid=getrowcolformsn(boxid).row;
            int colfocusingrid=getrowcolformsn(boxid).col;
            setshapeoccp(boxid,ballidtosit,shapeidtosit);//设置一下占用情况
            for(int j=0;j<n_balls;j++)
            {
                int row=(veclinkshape[shapeidtosit-1].getptrow(j))-rowfocusinshape+rowfocusingrid;
                int col=(veclinkshape[shapeidtosit-1].getptcol(j))-colfocusinshape+colfocusingrid;

                ballshapesingriddefault[shapeidtosit-1]->setballpos(j,col*balldiameter,row*balldiameter);
            }
            player->setMedia(QUrl("qrc:/sound/wav/dropdown.wav"));//行
            player->play();
        }

        if(gridocupied())
        {
            player->setMedia(QUrl("qrc:/sound/wav/gamewon.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("OK");
            msgBox.setText("Goodjob! congratulations!! ");
            msgBox.exec();
            gamestarted=0;
            timera.stop();
            flaggametimeon=0;
            isinrank(gridnum,linknum,timegame);//时间成绩处理
        }
    }
}


void ballgrid::isinrank(int gridnumber,int linknumber,float gametime)//判断本次游戏成绩是否进入排行榜，进入则修改成绩记录xml
{
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    QList<QDomElement> elems;
    while(!n.isNull())
    {
        elems.append(n.toElement());
        n = n.nextSibling();
    }

    QList<int> elemlist;//记录此类元素的索引的列表
    int idrec=1;
    int elemlast=0;
    for(int i=0;i<elems.count();++i)
    {
        qDebug()<< qPrintable(elems[i].tagName())<<elems[i].text();
        qDebug()<< "gridnumber:"<<elems[i].attribute("gridnum").toInt();
        if(gridnumber == elems[i].attribute("gridnum").toInt() && linknumber == elems[i].attribute("linknum").toInt())
        {
            elemlast=i;//记录下此类最后一个排名的元素的索引
            idrec+=1;
            elemlist.append(i);
        }
    }

    bool flaginrank=false;
    if(!elemlist.empty())//存在此类成绩记录，则判断排行榜
    {
        int elemins=0;
        for(int j=0;j<elemlist.count();++j)
        {
            int i=elemlist[j];
            if(gametime < elems[i].text().toFloat())//当游戏时间小于存在得记录，则做处理
            {
                if(idrec > elems[i].attribute("id").toInt()) { //主要是设置id值
                    idrec=elems[i].attribute("id").toInt();
                    elemins=i;//记录要插入位置的元素的索引
                    elems[i].setAttribute("id",idrec+1);
                }else
                {
                    elems[i].setAttribute("id",elems[i].attribute("id").toInt()+1);
                }
            }
        }
        //当游戏时间都大于版中的时，插入位置是当前类的最后一个成绩后面
        if(idrec==elemlist.count()+1) elemins=elemlast+1;


        qDebug()<<"current score's rank id :idrect"<<idrec;
        QString gamer;
        if(idrec<=elemlist.count() || idrec<=3) {//排行榜内
            qDebug()<< "is in rank list";
            flaginrank=true;

            QDialog *rankdialog=new QDialog;//input dialog的parent需要是一个widget
            QInputDialog inputdialog;
            bool ok;
            gamer = inputdialog.getText(rankdialog, tr("QInputDialog::getText()"),
                                                    tr("gamer:"), QLineEdit::Normal,
                                                    tr("user"),&ok);

            if (gamer.isEmpty())
            {
                gamer="no name";
            }
            qDebug()<<"gamer:"<< gamer;


        }

        if(flaginrank) {
            QDomElement reca = doc.createElement("record");
            reca.setAttribute("gridnum",gridnumber);
            reca.setAttribute("linknum",linknumber);
            reca.setAttribute("user",gamer);
            reca.setAttribute("id",idrec);
            QDomText recat = doc.createTextNode(QString("%1").arg(gametime));
            reca.appendChild(recat);
            //docElem.appendChild(reca);
            qDebug()<<"elems:"<<elems.count()<<"insert pos:"<<elemins;
            if(elemins < elems.count()) {//当插入位置在elems内时插入
                docElem.insertBefore(reca,elems[elemins]);
            }else//当插入位置在最后时
            {
                docElem.appendChild(reca);
            }
            if(elemlist.count()>2)
            {//只有当一类排行数量大于3才会删除
                docElem.removeChild(elems[elemlast]);
            }
        }
    }
    else//不存在此类成绩记录，则创建排行榜
    {
        QString gamer;
        qDebug()<< "is in rank list";
        flaginrank=true;

        QDialog *rankdialog=new QDialog;
        QInputDialog inputdialog;
        bool ok;
        gamer = inputdialog.getText(rankdialog, tr("QInputDialog::getText()"),
                                                tr("gamer:"), QLineEdit::Normal,
                                                tr("user"),&ok);


        if (gamer.isEmpty())
        {
            gamer="no name";
        }
        qDebug()<< gamer;

        QDomElement reca = doc.createElement("record");
        reca.setAttribute("gridnum",gridnumber);
        reca.setAttribute("linknum",linknumber);
        reca.setAttribute("user",gamer);
        reca.setAttribute("id",1);
        QDomText recat = doc.createTextNode(QString("%1").arg(gametime));
        reca.appendChild(recat);
        docElem.appendChild(reca);

    }

    if(flaginrank){//在排行榜内才保存
        QString a=QCoreApplication::applicationDirPath();
        QString filepathb=a+"/info.xml";
        QFile fileb(filepathb);
        if(!fileb.open(QIODevice::WriteOnly|QIODevice::Truncate)) return;
        QTextStream out(&fileb);
        doc.save(out,4);
        fileb.close();
    }
}


void ballgrid::showcredit()
{
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    QList<QDomElement> elems;
    while(!n.isNull())
    {
        elems.append(n.toElement());
        n = n.nextSibling();
    }


    QDialog *rankdialog=new QDialog;
    QTableWidget *tableWidget;
    tableWidget = new QTableWidget(elems.count(),5, rankdialog);
    tableWidget->setGeometry(0,0,500,300);


    for(int row=0;row<elems.count();++row)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(elems[row].attribute("id"));
        tableWidget->setItem(row, 0, newItem);
        QTableWidgetItem *newItem1 = new QTableWidgetItem(elems[row].attribute("user"));
        tableWidget->setItem(row, 1, newItem1);
        QTableWidgetItem *newItem2 = new QTableWidgetItem(elems[row].text());
        tableWidget->setItem(row, 2, newItem2);
        QTableWidgetItem *newItem3 = new QTableWidgetItem(elems[row].attribute("gridnum"));
        tableWidget->setItem(row, 3, newItem3);
        QTableWidgetItem *newItem4 = new QTableWidgetItem(elems[row].attribute("linknum"));
        tableWidget->setItem(row, 4, newItem4);
    }

    QTableWidgetItem *valuesHeaderItem = new QTableWidgetItem(tr("排名"));
    tableWidget->setHorizontalHeaderItem(0, valuesHeaderItem);
    QTableWidgetItem *valuesHeaderItem1 = new QTableWidgetItem(tr("游戏者"));
    tableWidget->setHorizontalHeaderItem(1, valuesHeaderItem1);
    QTableWidgetItem *valuesHeaderItem2 = new QTableWidgetItem(tr("完成时间"));
    tableWidget->setHorizontalHeaderItem(2, valuesHeaderItem2);
    QTableWidgetItem *valuesHeaderItem3 = new QTableWidgetItem(tr("网格大小"));
    tableWidget->setHorizontalHeaderItem(3, valuesHeaderItem3);
    QTableWidgetItem *valuesHeaderItem4 = new QTableWidgetItem(tr("形状大小"));
    tableWidget->setHorizontalHeaderItem(4, valuesHeaderItem4);


    QStringList labels;
    for(int i=0;i<elems.count();++i)
    {
        labels<<" ";
    }
    tableWidget->setVerticalHeaderLabels(labels);


    tableWidget->show();

    rankdialog->exec();
}


void ballgrid::dealshapetosit(int ballid,int shapeida)
{
    shapeidtosit=shapeida;
    ballidtosit=ballid;
    qDebug()<<"shape's balls:"<<veclinkshape[shapeida-1].getshapenballs();
    for(int i=0;i<veclinkshape[shapeida-1].getshapenballs();++i)
    {
        qDebug()<<"coord:"<<veclinkshape[shapeida-1].getptrow(i)<<veclinkshape[shapeida-1].getptcol(i);
        if(gamestarted==1) {
        qDebug()<<" coord outer"<<veclinkshape[shapeida-1].getptrowouter(i)<<veclinkshape[shapeida-1].getptcolouter(i);
        }
    }
    player->setMedia(QUrl("qrc:/sound/wav/selected.wav"));
    player->play();
    qDebug()<<"recieved info from shape:"<<shapeida;
    qDebug()<<"shape:"<<shapeida<<" wants to be sit down!";
}

void ballgrid::shapeturnright(bool a)//按钮控制向右旋转
{
    Q_UNUSED(a);
    if(gamestarted==1) {
        if(shapeidtosit>0){//当有形状选中的时候做
            shapetrans(shapeidtosit,1);
            player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
            player->play();
        }else{
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText("None shape was selected! \npress choose a shape!");
            msgBox.exec();
        }
    }else{
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("please start the game! \npress start button!");
        msgBox.exec();
    }
}

void ballgrid::shapeturnleft(bool a)//按钮控制向左旋转
{
    Q_UNUSED(a);
    if(gamestarted==1) {
        if(shapeidtosit>0){//当有形状选中的时候做
            shapetrans(shapeidtosit,2);
            player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
            player->play();
        }else{
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText("None shape was selected! \npress choose a shape!");
            msgBox.exec();
        }
    }else{
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("please start the game! \npress start button!");
        msgBox.exec();
    }
}

void ballgrid::shapemirrorab(bool a)//按钮控制上下翻转
{
    Q_UNUSED(a);
    if(gamestarted==1) {
        if(shapeidtosit>0){//当有形状选中的时候做
            shapetrans(shapeidtosit,3);
            player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
            player->play();
        }else{
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText("None shape was selected! \npress choose a shape!");
            msgBox.exec();
        }
    }else{
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("please start the game! \npress start button!");
        msgBox.exec();
    }
}

void ballgrid::shapemirrorlr(bool a)//按钮控制左右翻转
{
    Q_UNUSED(a);
    if(gamestarted==1) {
        if(shapeidtosit>0){//当有形状选中的时候做
            shapetrans(shapeidtosit,4);
            player->setMedia(QUrl("qrc:/sound/wav/trans.wav"));//行
            player->play();
        }else{
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText("None shape was selected! \npress choose a shape!");
            msgBox.exec();
        }
    }else{
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("please start the game! \npress start button!");
        msgBox.exec();
    }
}

void ballgrid::shaperesetpos(bool a)//按钮控制移出网格
{
    Q_UNUSED(a);
    if(shapeidtosit>0){//当有形状选中的时候做
        shapereset(ballidtosit,shapeidtosit);
        player->setMedia(QUrl("qrc:/sound/wav/moveout.wav"));//行
        player->play();
    }else{
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("None shape was selected! \npress choose a shape!");
        msgBox.exec();
    }
}


void ballgrid::shapereset(int ballid,int shapeida)
{
    if(gamestarted==1) {
        //qDebug()<<"recieved info from shape:"<<shapeida;
        //qDebug()<<"shape:"<<shapeida<<" wants to be sit down!";
        shapeidtosit=shapeida;
        ballidtosit=ballid;

        QPointF posa=ballshapesingriddefault[shapeida-1]->getballpos(ballid);
        int boxid=getboxidat(posa)+1;//当前球所在的位置
        if(boxid < n_total)
        {
            clearshapeoccp(boxid,ballid,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
        }
        //qDebug()<<"boxid:"<<boxid;

        ballshapesingriddefault[shapeida-1]->resetpos();
        veclinkshape[shapeida-1].resetoutercoord();
        player->setMedia(QUrl("qrc:/sound/wav/moveout.wav"));//行
        player->play();

    }else{
        player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
        player->play();
        QMessageBox msgBox;
        msgBox.setWindowTitle("WARNING");
        msgBox.setText("please start the game! \npress start button!");
        msgBox.exec();
    }
}


void ballgrid::setshapesdefault(int arraysize)//利用原先存有的默认的划分，利用继承的n_scale，n_n_balls_ofshape信息选择不同情况下的默认划分
{
    QColor colora[256];
    for(int i=0;i<256;i++)
    {
        colora[i]=QColor(qrand()%256,qrand()%256,i);
    }

    ballshapesingriddefault.clear();//初始化一下
    std::vector<linkshape> veclinka;
    veclinkshape=veclinka;

    qDebug()<<"gridnum:"<<gridnum;
    if(flaggridchanged==0 && gridnum ==10)
    {

        n_ofshapes=12;
        coord vectmp1[5]={{6,0},{7,0},{7,1},{7,2},{7,3}};
        int vecnumber1[5]={22, 29, 30, 31, 32};
        linkshape shapea=linkshape(1,5,vectmp1,vecnumber1);
        veclinkshape.push_back(shapea);

        coord vectmp2[5]={{5,5},{5,4},{5,3},{4,3},{4,4}};
        int vecnumber2[5]={21, 20, 19, 14, 15};
        shapea=linkshape(2,5,vectmp2,vecnumber2);
        veclinkshape.push_back(shapea);

        coord vectmp3[5]={{3,3},{3,2},{4,2},{5,2},{3,1}};
        int vecnumber3[5]={10, 9, 13, 18, 8};
        shapea=linkshape(3,5,vectmp3,vecnumber3);
        veclinkshape.push_back(shapea);

        coord vectmp4[5]={{8,7},{7,7},{9,7},{9,6},{9,5}};
        int vecnumber4[5]={44, 36, 53, 52, 51};
        shapea=linkshape(4,5,vectmp4,vecnumber4);
        veclinkshape.push_back(shapea);

        coord vectmp5[5]={{9,4},{9,3},{9,2},{9,1},{9,0}};
        int vecnumber5[5]={50, 49, 48, 47, 46};
        shapea=linkshape(5,5,vectmp5,vecnumber5);
        veclinkshape.push_back(shapea);

        coord vectmp6[5]={{8,5},{8,6},{7,6},{6,6},{6,5}};
        int vecnumber6[5]={42, 43, 35, 28, 27};
        shapea=linkshape(6,5,vectmp6,vecnumber6);
        veclinkshape.push_back(shapea);


        coord vectmp7[3]={{9,9},{9,8},{8,8}};
        int vecnumber7[3]={55, 54, 45};
        shapea=linkshape(7,3,vectmp7,vecnumber7);
        veclinkshape.push_back(shapea);

        coord vectmp8[5]={{6,3},{6,2},{6,1},{5,1},{5,0}};
        int vecnumber8[5]={25, 24, 23, 17, 16};
        shapea=linkshape(8,5,vectmp8,vecnumber8);
        veclinkshape.push_back(shapea);

        coord vectmp9[5]={{8,3},{8,4},{7,4},{7,5},{6,4}};
        int vecnumber9[5]={40, 41, 33, 34, 26};
        shapea=linkshape(9,5,vectmp9,vecnumber9);
        veclinkshape.push_back(shapea);

        coord vectmp10[5]={{2,1},{2,2},{1,1},{1,0},{0,0}};
        int vecnumber10[5]={5, 6, 3, 2, 1};
        shapea=linkshape(10,5,vectmp10,vecnumber10);
        veclinkshape.push_back(shapea);

        coord vectmp11[4]={{4,0},{4,1},{3,0},{2,0}};
        int vecnumber11[4]={11, 12, 7, 4};
        shapea=linkshape(11,4,vectmp11,vecnumber11);
        veclinkshape.push_back(shapea);

        coord vectmp12[3]={{8,1},{8,2},{8,0}};
        int vecnumber12[3]={38, 39, 37};
        shapea=linkshape(12,3,vectmp12,vecnumber12);
        veclinkshape.push_back(shapea);

        int ballpos[arraysize];

        for(int i=0;i<n_ofshapes;i++)
        {
            int id=veclinkshape[i].getshapesn();
            int n_ballsinshape=veclinkshape[i].getshapenballs();
            //qDebug()<<"id:"<<id;
            //qDebug()<<"n_ball_inshape:"<<n_ballsinshape;

            for(int j=0;j<n_ballsinshape;j++)
            {
                int row=(veclinkshape[i].getptrow(j)+veclinkshape[i].getrowofzero());
                int col=(veclinkshape[i].getptcol(j)+veclinkshape[i].getcolofzero());
                //qDebug()<<"ball sn="<<veclinkshape[i].getballsn(j);
                //qDebug()<<"row="<<row<<" col="<<col;
                ballpos[2*j]=(col*balldiameter);
                ballpos[2*j+1]=(row*balldiameter);
            }

            ballshape *shapea=new ballshape();
            shapea->setballdiameter(balldiameter);
            shapea->setballshape(id,n_ballsinshape,ballpos,colora[qrand()%256]);
            ballshapesingriddefault.append(shapea);
            QObject::connect(shapea,&ballshape::shapeselected,this,&ballgrid::dealshapetosit);
            QObject::connect(shapea,&ballshape::shapetoreset,this,&ballgrid::shapereset);
            QObject::connect(shapea,&ballshape::shapetotrans,this,&ballgrid::shapetrans);
        }
    } else
    {
        partition(n_total);

        qDebug()<<"n_ofshapes="<<n_ofshapes;
        qDebug()<<"n_total="<<n_total;
        if(n_ofshapes<n_total) {
            setshapes(n_total);
            ballshapesingriddefault=ballshapesingrid;
            QString stra=QString("%1").arg(n_ofshapes);
            emit shapecompleted(stra);
        }else
        {
            player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
            player->play();
            QMessageBox msgBox;
            msgBox.setWindowTitle("WARNING");
            msgBox.setText(" partition is not successful! \n please reset the grid scale and link ball number, \n and redo partition!");
            msgBox.exec();
            qDebug()<<"partition is not successful! please redo!"<<n_ofshapes;
        }
    }
}

void ballgrid::addtoscenedefault()//将默认的划分的形状加入到场景中
{
    if(n_ofshapes<n_total) {//只有当形状划分正确的时候做，而划分不正确时n_ofshapes设为1+n_total
    for(int i=0;i<n_ofshapes;i++)
    {
        ballshapesingriddefault[i]->addtoscene(*sceneonwork);
    }
    }
}

void ballgrid::removeshapes()//将划分形状从场景移除
{
    if(n_ofshapes<n_total) {//只有当形状划分正确的时候做，而划分不正确时n_ofshapes设为1+n_total
    for(int i=0;i<n_ofshapes;i++)
    {
        ballshapesingriddefault[i]->removeballs(*sceneonwork);
    }
    }
}


void ballgrid::setshaperandposdefault()//将划分形状放到网格外
{
    int colnow=0;
    int rownow=-5;
    int colbound=(40*4+200)/balldiameter+2;
    for(int i=0;i<n_ofshapes;i++)
    {
        if(colnow < colbound ) {//一排排完前竖着放，即形状的起点位置小于(colbound)*balldiameter，即用列表示的话，不超过colbound列时竖着排
            int rows=veclinkshape[i].getrows();
            int cols=veclinkshape[i].getcols();
            if(cols > rows )
            {
                veclinkshape[i].rotate_right();
                rows=veclinkshape[i].getrows();
                cols=veclinkshape[i].getcols();
            }

            for(int j=0;j<veclinkshape[i].getshapenballs();j++)
            {
                int row=(veclinkshape[i].getptrow(j))-rows-1;
                int col=colnow+(veclinkshape[i].getptcol(j));

               ballshapesingriddefault[i]->setballpos(j,col*balldiameter,row*balldiameter);
            }
            colnow=colnow+cols+1;
        }
        else//剩下的横着放
        {
            int rows=veclinkshape[i].getrows();
            int cols=veclinkshape[i].getcols();
            if(cols < rows )
            {
                veclinkshape[i].rotate_right();
                rows=veclinkshape[i].getrows();
                cols=veclinkshape[i].getcols();
            }
            for(int j=0;j<veclinkshape[i].getshapenballs();j++)
            {
                int row=rownow+(veclinkshape[i].getptrow(j));
                int col=colnow+(veclinkshape[i].getptcol(j));

               ballshapesingriddefault[i]->setballpos(j,col*balldiameter,row*balldiameter);
            }
            rownow=rownow+rows+1;
        }
        ballshapesingriddefault[i]->recordpos();
        veclinkshape[i].recordoutercoord();
    }
}


void ballgrid::shapetrans(int shapeida,int transid)//形状变换:旋转和翻转
{
    if(shapeidtosit!=shapeida){
        qDebug()<<"error:"<<shapeidtosit<<shapeida;
    }

    QPointF posa=ballshapesingriddefault[shapeida-1]->getballpos(ballidtosit);
    int boxid=getboxidat(posa)+1;//当前球所在的位置
    qDebug()<<"pos :"<<posa.x()<<posa.y();

    switch(transid)
    {
        case 1://Qt::Key_Right:
            veclinkshape[shapeida-1].saveoldcoord();//保存原始坐标信息
            if(boxid <= n_total)//当在网格内时
            {
                clearshapeoccp(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                veclinkshape[shapeida-1].rotate_right();
                int flagcanbe=canbesetdown(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                if(flagcanbe==1)
                {
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }else
                {
                    player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
                    player->play();
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("WARNING");
                    msgBox.setText("can not rotate right");
                    msgBox.exec();
                    veclinkshape[shapeida-1].resetoldcoord();//恢复原始坐标信息
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }
            }
            else
            {
                veclinkshape[shapeida-1].rotate_right();
            }
            break;

        case 2://Qt::Key_Left:
            veclinkshape[shapeida-1].saveoldcoord();//保存原始坐标信息
            if(boxid <= n_total)//当在网格内时
            {
                clearshapeoccp(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                veclinkshape[shapeida-1].rotate_left();
                int flagcanbe=canbesetdown(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                if(flagcanbe==1)
                {
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }else
                {
                    player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
                    player->play();
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("WARNING");
                    msgBox.setText("can not rotate left");
                    msgBox.exec();
                    veclinkshape[shapeida-1].resetoldcoord();//恢复原始坐标信息
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }
            }
            else
            {
                veclinkshape[shapeida-1].rotate_left();
            }

            break;

        case 3://Qt::Key_Up:
            //qDebug()<<"mirror up and down";
            veclinkshape[shapeida-1].saveoldcoord();//保存原始坐标信息
            if(boxid <= n_total)//当在网格内时
            {
                clearshapeoccp(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                veclinkshape[shapeida-1].mirror_ab();
                int flagcanbe=canbesetdown(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                if(flagcanbe==1)
                {
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }else
                {
                    player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
                    player->play();
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("WARNING");
                    msgBox.setText("can not mirror up and down");
                    msgBox.exec();
                    //qDebug()<<"can not mirror up and down";
                    veclinkshape[shapeida-1].resetoldcoord();//恢复原始坐标信息
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }
            }
            else
            {
                veclinkshape[shapeida-1].mirror_ab();
            }
            break;

        case 4://Qt::Key_Down:
            //qDebug()<<"mirror up and down";
            veclinkshape[shapeida-1].saveoldcoord();//保存原始坐标信息
            if(boxid <= n_total)//当在网格内时
            {
                clearshapeoccp(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                veclinkshape[shapeida-1].mirror_lr();
                int flagcanbe=canbesetdown(boxid,ballidtosit,shapeida);//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
                if(flagcanbe==1)
                {
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }else
                {
                    player->setMedia(QUrl("qrc:/sound/wav/warning.wav"));//行
                    player->play();
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("WARNING");
                    msgBox.setText("can not mirror up and down");
                    msgBox.exec();
                    //qDebug()<<"can not mirror up and down";
                    veclinkshape[shapeida-1].resetoldcoord();//恢复原始坐标信息
                    setshapeoccp(boxid,ballidtosit,shapeida);
                }
            }
            else
            {
                veclinkshape[shapeida-1].mirror_lr();
            }
            break;

        default:
            break;
    }

    int n_balls=veclinkshape[shapeida-1].getshapenballs();
    int rowfocusinshape=veclinkshape[shapeidtosit-1].getptrow(ballidtosit-1);
    int colfocusinshape=veclinkshape[shapeidtosit-1].getptcol(ballidtosit-1);

    for(int j=0;j<n_balls;j++)
        {
            int row=(veclinkshape[shapeidtosit-1].getptrow(j))-rowfocusinshape;
            int col=(veclinkshape[shapeidtosit-1].getptcol(j))-colfocusinshape;

            ballshapesingriddefault[shapeidtosit-1]->setballpos(j,col*balldiameter+posa.x(),row*balldiameter+posa.y());
        }
}

void ballgrid::setshapes(int arraysize)//根据grid函数partion后得到的std::vector<linkshape> veclinkshape;
{
    int ballpos[arraysize];//形状中球的位置的临时数组设置的大一些，因为各个形状中球数是不同的
    //int ballpos[arraysize];

    QColor colora[256];
    for(int i=0;i<256;i++)
    {
        colora[i]=QColor(qrand()%256,qrand()%256,i);
    }

    ballshapesingrid.clear();
    for(int i=0;i<n_ofshapes;i++)
    {
        int id=veclinkshape[i].getshapesn();
        int n_ballsinshape=veclinkshape[i].getshapenballs();
        //qDebug()<<"id:"<<id;
        //qDebug()<<"n_ball_inshape:"<<n_ballsinshape;

        for(int j=0;j<n_ballsinshape;j++)
        {
            int row=(veclinkshape[i].getptrow(j)+veclinkshape[i].getrowofzero());
            int col=(veclinkshape[i].getptcol(j)+veclinkshape[i].getcolofzero());
            //qDebug()<<"ball sn="<<veclinkshape[i].getballsn(j);
            //qDebug()<<"row="<<row<<" col="<<col;
            ballpos[2*j]=(col*balldiameter);
            ballpos[2*j+1]=(row*balldiameter);
        }

        ballshape *shapea=new ballshape();
        shapea->setballdiameter(balldiameter);
        shapea->setballshape(id,n_ballsinshape,ballpos,colora[qrand()%256]);
        ballshapesingrid.append(shapea);//形状图形列表ballshapesingrid，对应的生成的形状veclinkshape

        QObject::connect(shapea,&ballshape::shapeselected,this,&ballgrid::dealshapetosit);
        QObject::connect(shapea,&ballshape::shapetoreset,this,&ballgrid::shapereset);
        QObject::connect(shapea,&ballshape::shapetotrans,this,&ballgrid::shapetrans);
    }
}
