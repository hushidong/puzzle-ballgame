#ifndef SHAPEBALL_H
#define SHAPEBALL_H

#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <qDebug>

/**球的拓扑类
主要是设置球二维拓扑中左右上下的邻居信息
存在邻居则设置为邻居的编号
否则设置为负
*/
class balltopo
{
private:
    int sn_self;//< topo[0]  //是真实序号，从1开始计数
    int sn_left;//< topo[1]  //存在邻居则设置邻居序号，不存在则设置为-1
    int sn_right;
    int sn_above;
    int sn_below;
	int occupied; //用于判断网格中的球的位置是否已经占用,1表示占用,0表示不占用
	int shaped;//用于表示已经划分到形状中

public:
    void settopo(int topo[5])//根据设置球二维拓扑中左右上下的邻居信息
    {
        sn_self=topo[0];
        sn_left=topo[1];
        sn_right=topo[2];
        sn_above=topo[3];
        sn_below=topo[4];
    }
	
	void setoccp(int occp)
    {
		occupied=occp;
    }
	
	void setshpd(int shpd)
    {
		shaped=shpd;
    }
	
	int getoccp()
    {
		return occupied;
    }
	
	int getshpd()
    {
		return shaped;
    }

    int getsn()//得到当前球的编号/序号
    {
        return sn_self;
    }

    void gettopo(int topo[5])//得到当前球在二维拓扑中左右上下的邻居信息
    {
        topo[0]=sn_self;
        topo[1]=sn_left;
        topo[2]=sn_right;
        topo[3]=sn_above;
        topo[4]=sn_below;
    }
	
	int getright()
    {
        return sn_right;
    }

	int getleft()
    {
        return sn_left;
    }
	
	int getabove()
    {
        return sn_above;
    }
	int getbelow()
    {
        return sn_below;
    }
	
};


typedef struct axiscood{//坐标结构体,注意xy表示在形状矩阵中的行号和列号.行号从上到下增加,列号从左到右增加
int row;//x表示row
int col;//y表示col
}coord;


 #endif // SHAPEBALL_H
