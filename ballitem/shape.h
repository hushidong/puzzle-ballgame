#ifndef SHAPE_H
#define SHAPE_H

#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <shapelink.h>
#include <shapeball.h>
#include <iostream>
#include <cstdlib>
#include <qDebug>
#include <QMessageBox>


/*
球构成的下三角矩阵
*/
class grid
{
private:
    int gridid;

protected:
    int n_scale;//网格的大小，行列数均为n_scale
	int n_balls_ofshape;//链接形状中的球数
	int n_ofshapes;//划分成的形状数
    int n_total;
	int n_focus;//<当前网格中的焦点,用于在其位置及邻居位置判断占用情况,从1开始计数，为实际球序号
    std::vector<balltopo> vecgrid;//球列表
	std::vector<linkshape> veclinkshape;//形状列表

public:

grid()//构造函数,无参数构造函数可以用来声明数组
{}

grid(int n,int m)//重载构造函数
{
    setgrid(n,m);
}

void setgrid(int n,int m)
{
    n_scale=n;
    n_balls_ofshape=m;
    std::vector<balltopo> vecgrida;//每次改变都清空
    vecgrid=vecgrida;
    balltopo trigrid[n_scale][n_scale];
    //下三角网格，只是一个下三角矩阵即可
    int nij=0;
    //printf("construct:\n");
    for (int i=0;i<n_scale;i++){//存储结构是先行后列.
        for(int j=0;j<=i;j++){
            vecgrid.push_back(trigrid[i][j]);
            nij++;
            //printf("%d ",nij);
        }
    }
    //printf("\n");
    trigridinit();
}

void trigridinit(){//下三角网格信息初始化
    balltopo trigrid[n_scale][n_scale];
    int n=n_scale;
    int sumlinelessi=0;
    int sn=0;
    for(int i=1; i<=n; i++)//行
    {
        sumlinelessi=sumlinelessi+(i-1);//i行前的球数,每行的球数都等于该行的行号
        for(int j=1; j<=i; j++)//列
        {
            //序号
            sn=sumlinelessi+j; //sn_self=sum_{k=1}^{i-1}(i)+j,注意:序号从1开始
                                                //但数组是从0开始的
            //左侧邻居
            int left=0;
            if(j==1){
                left=-1;//j=1时,左侧无邻居,所以设置为-1
            }
            else{
                left=sn-1;
            }

            //右侧邻居
            int right=0;
            if(j==i){
                right=-1;//j=i时,右侧无邻居,所以设置为-1
            }
            else{
                right=sn+1;
            }

            //上方邻居
            int above=0;
            if(i==j){
                above=-1;//i=j时,上方无邻居,所以设置为-1
            }
            else{
                above=sn-i+1;
            }

            //下方邻居
            int below=0;
            if(i==n){
                below=-1;//i=n时,下方无邻居,所以设置为-1
            }
            else{
                below=sn+i;
            }

            //printf("sn:%d,left:%d,right:%d,above:%d,below:%d\n",sn,left,right,above,below);
            int toposet[5]={sn,left,right,above,below};
            trigrid[i-1][j-1].settopo(toposet);
        }
    }
    n_total=sn;
    int nij=0;
    for (int i=0;i<n_scale;i++)
    {
        for(int j=0;j<=i;j++)//[i][j]与矢量中序号是一一对应的。
        {
            trigrid[i][j].setoccp(0);
            trigrid[i][j].setshpd(0);
            vecgrid[nij]=trigrid[i][j];
            nij++;
        }
    }
}

//利用形状中的球数和特征值列表判断形状是否相同，//0表示不同，1表示相同
int checksameshape(linkshape a,linkshape b)
{
    if(a.getshapenballs() != b.getshapenballs()) {
        return 0;
    }else
    {
        for(int i=0;i<a.getshapenballs();i++)
        {
            if(fabs(a.getshapeeigen(i)-b.getshapeeigen(i))>0.000001) return 0;
        }
    }

    //printf("eigen is the same!\n");

    int flagsame=0;
    //printf("ball a rows=%d , cols=%d\n",a.getrows(),a.getcols());
    //printf("ball b rows=%d , cols=%d\n",b.getrows(),b.getcols());
    if(a.getrows() != a.getcols()) {//行列不相等时，只要做翻转就可以比较了
        if(a.getrows()==b.getrows() && a.getcols()==b.getcols() )//当行数列数完全相同
        {
            int sumrowsa=0;
            int sumcolsa=0;
            int sumrowsb=0;
            int sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)//正常比较坐标位置行和列加和后的总数
            {
                sumrowsa=sumrowsa+a.getptrow(i);
                sumcolsa=sumcolsa+a.getptcol(i);
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball a pos sum of rows=%d , of cols=%d\n",sumrowsa,sumcolsa);
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb) flagsame=1;

            b.mirror_lr();//左右翻转一下
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.mirror_ab();//接着上下翻转一下
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if (sumrowsa==sumrowsb && sumcolsa==sumcolsb) flagsame=1;

            b.mirror_lr();//接着再左右翻转一下，4中情况都覆盖到了
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;
        }
        else if(a.getrows()==b.getcols() && a.getcols()==b.getrows() )//当a行数=b列数，a列数=b行数
        {
            b.rotate_right();//转过来使得行，列数相同
            int sumrowsa=0;
            int sumcolsa=0;
            int sumrowsb=0;
            int sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)//正常比较坐标位置行和列加和后的总数
            {
                sumrowsa=sumrowsa+a.getptrow(i);
                sumcolsa=sumcolsa+a.getptcol(i);
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball a pos sum of rows=%d , of cols=%d\n",sumrowsa,sumcolsa);
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb) flagsame=1;

            b.mirror_lr();//左右翻转一下
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.mirror_ab();//接着上下翻转一下
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if (sumrowsa==sumrowsb && sumcolsa==sumcolsb) flagsame=1;

            b.mirror_lr();//接着再左右翻转一下，4中情况都覆盖到了
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;
        }
        else
        {
            return 0;
        }
    }
    else//行列相等时，需要做旋转和翻转
    {
        if(b.getrows() != b.getcols())
        {
            return 0;
        }else
        {
            int sumrowsa=0;
            int sumcolsa=0;
            int sumrowsb=0;
            int sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)//正常比较坐标位置行和列加和后的总数
            {
                sumrowsa=sumrowsa+a.getptrow(i);
                sumcolsa=sumcolsa+a.getptcol(i);
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //a.pos_show();
            //b.pos_show();
            //printf("ball a pos sum of rows=%d , of cols=%d\n",sumrowsa,sumcolsa);
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb) flagsame=1;

            b.rotate_right();//顺时针旋转90度
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //b.pos_show();
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.rotate_right();//顺时针旋转180度
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //b.pos_show();
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.rotate_right();//顺时针旋转270度
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //b.pos_show();
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.rotate_right();//顺时针旋转360度，还原
            b.mirror_ab();//接着上下翻转一下
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //b.pos_show();
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.rotate_right();//接着再顺时针旋转90度
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //b.pos_show();
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.rotate_right();//接着再顺时针旋转180度
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //b.pos_show();
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

            b.rotate_right();//接着再顺时针旋转270度
            sumrowsb=0;
            sumcolsb=0;
            for(int i=0;i<a.getshapenballs();i++)
            {
                sumrowsb=sumrowsb+b.getptrow(i);
                sumcolsb=sumcolsb+b.getptcol(i);
            }
            //b.pos_show();
            //printf("ball b pos sum of rows=%d , of cols=%d\n",sumrowsb,sumcolsb);
            if(sumrowsa==sumrowsb && sumcolsa==sumcolsb)  flagsame=1;

        }

    }

    if(flagsame==0) return 0;
    return 1;
}

void partition(int ntotal)//将球划分到形状中
{
	int i,j,k;
	int sn_now,sn_ball,posrow_now,poscol_now;
    linkshape shapevec[ntotal];//这个地方是要特别注意的，因为数组的声明需要的是常数，无法直接用类的属性，即便某些时候不出错，但也不是好的方式，因此增加一个输入ntotal相当于是给出了常数。
    linkshape shapevecrec[ntotal];
	float randtemp;
	srand(time(NULL));
	int shapenumber,shapenumberrec;
	int flagnoleft;//网格中存在球未划分的标记=0，表示还需要划分，>0否则不需要划分
	int flaginshape;//用于表示当前球是在形状中的标记
	int flagsameshape;
	
	
    posrow_now=0;
    poscol_now=0;
    shapenumberrec=ntotal+1;
	int ndopart;
    for(ndopart=0;ndopart<2000;ndopart++){
		//printf("-----------------------------------------\n");
		//printf("times of partitions:%d\n",ndopart);
        //qDebug()<<"times of partitions:"<<ndopart;
		
        for(i=0;i<ntotal;i++)//清除划分状态，便于再次划分
		{
			vecgrid[i].setshpd(0);
		}
		
		shapenumber=0;
		flagnoleft=0;
		int ndoshapes=0;//用于避免无法划分出不同形状完全覆盖网格时的死循环
		
        while (flagnoleft==0 && ndoshapes<200)
		{
			ndoshapes++;
			//printf("times of create different shape to cover all the grid :%d\n",ndoshapes);
            coord ptpos[n_balls_ofshape]={0,0};
			int ptnum[n_balls_ofshape]={0};


			//确定起点
//			k=0;
//			i=0;
//            while(i<ntotal)
//			{
//				if(vecgrid[i].getshpd()==0){
//					sn_now=i;
//					posrow_now=0;
//					poscol_now=0;
//					vecgrid[sn_now].setshpd(1);
//					ptnum[k]=sn_now;
//					ptpos[k].row=posrow_now;
//					ptpos[k].col=poscol_now;
//					//printf("i=%d",i);
//                    i=ntotal;
//				} else {
//					i++;
//					//printf("i=%d",i);
//                    if(i >= ntotal) flagnoleft=1;
//					//printf("flagnoleft=%d",flagnoleft);
//				}
//			}

            //确定起点机制换一种,采用随机选取的方式进行
            k=0;
            do
            {
                flagnoleft=1;//首先确定是否存在没有连接的球
                for(i=0;i<n_total;i++)
                {
                    if(vecgrid[i].getshpd()==0) flagnoleft=0;
                }
                sn_now=rand()%n_total;
            }
            while(vecgrid[sn_now].getshpd()==1 && flagnoleft==0);
            if(vecgrid[sn_now].getshpd()==0 && flagnoleft==0)
            {
                vecgrid[sn_now].setshpd(1);
                coord coorda=getrowcolformsn(sn_now+1);
                posrow_now=coorda.row;
                poscol_now=coorda.col;
                ptnum[k]=sn_now;
                ptpos[k].row=posrow_now;
                ptpos[k].col=poscol_now;
            }
			
			if(flagnoleft==0) {
			
				// printf("\ninitial ball numbers:");
				// for(j=0;j<=k;j++)
				// {
					// printf("%d ",ptnum[j]+1);//球真实序号比矢量中的序号大1
				// }
				// printf("\ninitial ball postions:");
				// for(j=0;j<=k;j++)
				// {
					// printf("(%d,%d) ",ptpos[j].row,ptpos[j].col);
				// }
				// printf("\n");
				
				
				int nstep=0;
				while(k<n_balls_ofshape-1 && nstep < 100){//当球数未达到要求，走动次数小于100次时做
					nstep++;
					randtemp=(rand()%100/100.0);
					//printf("steps of moving try to create a shape:%d. ",nstep);
					//printf("nstep=%d\n",nstep);
					//printf("randt=%f\n",randtemp);
					
					if(randtemp < 0.25)//右移
					{
						//printf("sn_now=%d\n",sn_now);
						//printf("right of sn_now=%d\n",vecgrid[sn_now].getright()-1);
						sn_ball=vecgrid[sn_now].getright();
						if(sn_ball>0) {
							if(vecgrid[sn_ball-1].getshpd() == 0){
								sn_now=sn_ball-1;//矢量中的序号比球真实序号小1
								poscol_now++;
								k++;
								ptnum[k]=sn_now;
								ptpos[k].row=posrow_now;
								ptpos[k].col=poscol_now;
								vecgrid[sn_now].setshpd(1);
							}else{//处理往回走的情况，但不能走到其它形状中
								flaginshape=0;
								for(j=0;j<=k;j++)
								{
									if(sn_ball-1==ptnum[j]) flaginshape=1;
								}
								if(flaginshape==1) {//当移动到的球在形状内则确定移动
									sn_now=sn_ball-1;
									poscol_now++;
								}
							}
						}
					}
					else if(randtemp < 0.5)//左移
					{
						//printf("sn_now=%d\n",sn_now);
						//printf("left of sn_now=%d\n",vecgrid[sn_now].getleft()-1);
						sn_ball=vecgrid[sn_now].getleft();
						if(sn_ball>0) {
							if(vecgrid[sn_ball-1].getshpd() == 0){
								sn_now=sn_ball-1;//矢量中的序号比球真实序号小1
								poscol_now--;
								k++;
								ptnum[k]=sn_now;
								ptpos[k].row=posrow_now;
								ptpos[k].col=poscol_now;
								vecgrid[sn_now].setshpd(1);
							}else{//处理往回走的情况，但不能走到其它形状中
								flaginshape=0;
								for(j=0;j<=k;j++)
								{
									if(sn_ball-1==ptnum[j]) flaginshape=1;
								}
								if(flaginshape==1) {
									sn_now=sn_ball-1;
									poscol_now--;
								}
							}
						}
					}
					else if(randtemp < 0.75)//上移
					{
						//printf("sn_now=%d\n",sn_now);
						//printf("above of sn_now=%d\n",vecgrid[sn_now].getabove()-1);
						sn_ball=vecgrid[sn_now].getabove();
						if(sn_ball>0) {
							if(vecgrid[sn_ball-1].getshpd() == 0){
								sn_now=sn_ball-1;//矢量中的序号比球真实序号小1
								posrow_now--;
								k++;
								ptnum[k]=sn_now;
								ptpos[k].row=posrow_now;
								ptpos[k].col=poscol_now;
								vecgrid[sn_now].setshpd(1);
							}else{//处理往回走的情况，但不能走到其它形状中
								flaginshape=0;
								for(j=0;j<=k;j++)
								{
									if(sn_ball-1==ptnum[j]) flaginshape=1;
								}
								if(flaginshape==1) {
									sn_now=sn_ball-1;
									posrow_now--;
								}
							}
						}
					}
					else//下移
					{
						//printf("sn_now=%d\n",sn_now);
						//printf("below of sn_now=%d\n",vecgrid[sn_now].getbelow()-1);
						sn_ball=vecgrid[sn_now].getbelow();
						if(sn_ball>0) {
							if(vecgrid[sn_ball-1].getshpd() == 0){
								sn_now=sn_ball-1;//矢量中的序号比球真实序号小1
								posrow_now++;
								k++;
								ptnum[k]=sn_now;
								ptpos[k].row=posrow_now;
								ptpos[k].col=poscol_now;
								vecgrid[sn_now].setshpd(1);
							}else{//处理往回走的情况，但不能走到其它形状中
								flaginshape=0;
								for(j=0;j<=k;j++)
								{
									if(sn_ball-1==ptnum[j]) flaginshape=1;
								}
								if(flaginshape==1) {
									sn_now=sn_ball-1;
									posrow_now++;
								}
							}
						}
					}
					//printf("sn_now=%d, real_sn_of_ball=%d\n",sn_now,sn_now+1);
					// int tempa;
					// std::cin>>tempa;
				}
				
				
				//显示一下当前的形状的球序号和相对坐标
				// trigriddisplay();
				// printf("\n");
				// printf("ball numbers:");
				// for(j=0;j<=k;j++)
				// {
					// printf("%d ",ptnum[j]+1);
				// }
				// printf("\n");
				// printf("ball postions:");
				// for(j=0;j<=k;j++)
				// {
					// printf("(%d,%d) ",ptpos[j].row,ptpos[j].col);
				// }
				// printf("\n");
				// printf("number of balls in this shape:%d\n",k+1);//从0开始的当前形状中的球数+1。
				//int tempa;
				//std::cin>>tempa;
				
				
				//将当前形状放到形状数组中
				coord vectmp[k+1];
				int vecnumber[k+1];
				for(j=0;j<=k;j++)
				{
					vectmp[j].row=ptpos[j].row;
					vectmp[j].col=ptpos[j].col;
					vecnumber[j]=ptnum[j]+1;//输出真实的球序号，而不是从0开始的序号。
				}
				linkshape shapea=linkshape(shapenumber+1,k+1,vectmp,vecnumber);
				//先判断形状是否与原先构成的相同
				if(shapenumber==0) {
					shapevec[shapenumber]=shapea;
					shapenumber++;
				}else{
					flagsameshape=0;
					for(j=0;j<shapenumber;j++)
					{
						if(checksameshape(shapea,shapevec[j]))
						{
							flagsameshape=1;
							//printf("flagsameshape=%d\n",flagsameshape);
						}
					}
					if(flagsameshape==0){//和之前的形状不相同
						shapevec[shapenumber]=shapea;
						shapenumber++;
					}else{
						for(j=0;j<=k;j++)
						{
							vecgrid[ptnum[j]].setshpd(0);
						}
					}
				}
				//int tempa;
				//std::cin>>tempa;
			}
		}

		//输出当前次，划分的形状
		// printf("\n");
		// printf("do the %d th time's partition\n",ndopart);
		// printf("number of shapes in grid is %d\n",shapenumber);
        //qDebug()<<"total number of shapes in grid is "<<shapenumberrec;
		// for(j=0;j<shapenumber;j++)
		// {
			// printf("j=%d ,shape sn: %d\n",j,shapevec[j].getshapesn());
			// printf("j=%d ,total number of balls in shape: %d\n",j,shapevec[j].getshapenballs());
			// printf("j=%d ,serial number of balls in shape:",j);
			// for(i=0;i<shapevec[j].getshapenballs();i++){
			// printf("%d ",shapevec[j].getballnumber(i));}
			// printf("\n");
		// }
		// if(flagnoleft==0) {
			// printf("partition is not completed,grid is not covered\n");
		// }
		
		//用形状数来判断是否是最佳划分，形状数最小的肯定是最佳的，
		//并记录到shapevecrec数组中
        if(shapenumber<shapenumberrec  and flagnoleft==1 ) {
			for(j=0;j<shapenumber;j++)
			{
				shapevecrec[j]=shapevec[j];
			}
			shapenumberrec=shapenumber;
            break;//为减小计算时间，不做最佳分配了，因为游戏也不需要最佳，不是做算法。
		}
		
		//int tempa;
		//std::cin>>tempa;
	}
	
		//输出并记录最佳划分，划分的形状
        if(shapenumberrec<n_total){//如果没有划分则不做输出
            //printf("\n");
            //trigriddisplay();
            //printf("\n");
            //printf("best partition for %d times' attempt:\n",ndopart);
            //printf("total number of shapes in grid is %d\n",shapenumberrec);
            //qDebug()<<"best partition for"<<ndopart <<" times' attempt:";
            qDebug()<<"total number of shapes in grid is "<<shapenumberrec;
            for(j=0;j<shapenumberrec;j++)
            {
                //printf("j=%d ,shape sn: %d\n",j,shapevecrec[j].getshapesn());
                //printf("j=%d ,total number of balls in shape: %d\n",j,shapevecrec[j].getshapenballs());
                //printf("j=%d ,serial number of balls in shape:",j);
                //for(i=0;i<shapevecrec[j].getshapenballs();i++){
                //printf("%d ",shapevecrec[j].getballnumber(i));}
                //printf("\n");
                //把链接形状放到veclinkshape形状中
                veclinkshape.push_back(shapevecrec[j]);
            }
        }
        n_ofshapes=shapenumberrec;//记录形状数，当形状数大于球数时表示形状划分失败，用于后面的判断

}  

void setfocus(const int i){//设置焦点框序号,序号为框的真实序号，从1开始计数
    n_focus=i;
}

int canbesetdown(int n_balls,int sn_focus_inshape,std::vector<coord> ballscoords)//根据焦点球序号以及形状中焦点和坐标确定形状能否放入网格中
//n_balls形状中球数，sn_focus_inshape为形状中焦点球序号从1开始计数，ballscoords形状中各点行列坐标
{
	int row=ballscoords[sn_focus_inshape-1].row;
	int col=ballscoords[sn_focus_inshape-1].col;
	int flagcanbe=1;
	for(int i=0;i<n_balls;i++)
	{
		int drow=ballscoords[i].row -row;
		int dcol=ballscoords[i].col -col;
		int sn_ball=getnumdrowdcola(drow,dcol);
        //qDebug()<<"drow:"<<drow;
        //qDebug()<<"dcol:"<<dcol;
        //qDebug()<<"sn_ball now:"<<sn_ball;
		if(sn_ball<0 || vecgrid[sn_ball-1].getoccp()==1){//当球不在范围内或者球所在框已被占据则无法放入
			flagcanbe=0;
		}
	}

	return flagcanbe;
}

int canbesetdown(int boxid,int ballid,int shapeid)//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
{
    setfocus(boxid);
    int n_balls=veclinkshape[shapeid-1].getshapenballs();
    int row=veclinkshape[shapeid-1].getptrow(ballid-1);
    int col=veclinkshape[shapeid-1].getptcol(ballid-1);
    int flagcanbe=1;
    for(int i=0;i<n_balls;i++)
    {
        int drow=veclinkshape[shapeid-1].getptrow(i)-row;
        int dcol=veclinkshape[shapeid-1].getptcol(i)-col;
        int sn_ball=getnumdrowdcola(drow,dcol);
        //qDebug()<<"drow:"<<drow;
        //qDebug()<<"dcol:"<<dcol;
        //qDebug()<<"sn_ball now:"<<sn_ball;
        if(sn_ball<0 || vecgrid[sn_ball-1].getoccp()==1)//当球不在范围内或者球所在框已被占据则无法放入
        {
            flagcanbe=0;
        }
    }
    return flagcanbe;
}

void clearshapeoccp(int boxid,int ballid,int shapeid)//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
{

    setfocus(boxid);
    int n_balls=veclinkshape[shapeid-1].getshapenballs();
    int row=veclinkshape[shapeid-1].getptrow(ballid-1);
    int col=veclinkshape[shapeid-1].getptcol(ballid-1);
    for(int i=0;i<n_balls;i++)
    {
        int drow=veclinkshape[shapeid-1].getptrow(i)-row;
        int dcol=veclinkshape[shapeid-1].getptcol(i)-col;
        int sn_ball=getnumdrowdcola(drow,dcol);
        vecgrid[sn_ball-1].setoccp(0);
        //qDebug()<<"sn_ball clear occp:"<<sn_ball;
    }
}

void setshapeoccp(int boxid,int ballid,int shapeid)//参数是框的序号，形状中当前球的序号，形状的序号，都从1开始计数
{

    setfocus(boxid);
    int n_balls=veclinkshape[shapeid-1].getshapenballs();
    int row=veclinkshape[shapeid-1].getptrow(ballid-1);
    int col=veclinkshape[shapeid-1].getptcol(ballid-1);
    for(int i=0;i<n_balls;i++)
    {
        int drow=veclinkshape[shapeid-1].getptrow(i)-row;
        int dcol=veclinkshape[shapeid-1].getptcol(i)-col;
        int sn_ball=getnumdrowdcola(drow,dcol);
        vecgrid[sn_ball-1].setoccp(1);
    }
}

int gridocupied() //判断网格是否全被占据
{
    int flaggridoccp=1;
    for(int i=0;i<n_total;i++)
    {
        if(vecgrid[i].getoccp()==0) flaggridoccp=0;
    }
    return flaggridoccp;
}

int getntotal()
{
    return n_total;
}

coord getrowcolformsn(int sn_ball){//输入是球的真实序号
	int nij=0;
    coord a={0,0};
	for(int i=0;i<n_scale;i++)
	{
		for(int j=0;j<=i;j++)
		{
			nij++;
			if(nij==sn_ball)
			{
                a.row=i;
                a.col=j;
                break;
			}
		}
	}
    return a;
}

int getsnfromrowcol(int row,int col){//输入的行和列是以0为起点的
	if(row<0) return -1;
	if(col<0) return -1;
	if(row>n_scale) return -1;
	if(col>row) return -1;
	
	int nij=0;
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<=i;j++)
		{
			nij++;
		}
	}
	return nij+col+1;//sn_ball
}

int getnumdrowdcol(int drow,int dcol){//根据焦点球序号以及与焦点球的行距和列距确定当前球序号，利用球的位置和序号关系

	 coord a=getrowcolformsn(n_focus);//获取焦点球的行列
	 int row=a.row+drow;
	 int col=a.col+dcol;
	 return getsnfromrowcol(row,col);
	 
}

int getnumdrowdcola(int drow,int dcol){//根据焦点球序号以及与焦点球的行距和列距确定当前球序号，利用球的拓扑关系
	 	
    //这种思路要注意的是因为列数总是小于等于当前行的行号，
    //当drow>0则所以先走行，再走列，不会出现先越界的情况
    //当drow<=0则先走列，再走行，则不会出现先越界的情况
	int flagoutrange=0;
	int sn_now=n_focus-1;//sn_now是在矢量中的序号，等于球真实序号-1
    if(drow >0) {
		for(int j=0;j<fabs(drow);j++)
		{
            if(sn_now>=0) {//注意行增加是往下走，即below
                sn_now=vecgrid[sn_now].getbelow()-1;//sn_now是在矢量中的序号，等于球真实序号-1
			}else
			{
				flagoutrange=1;
				j=fabs(drow);
			}
		}
    }
	if(dcol <=0) {
		for(int j=0;j<fabs(dcol);j++)
		{
			if(sn_now>=0) {
				sn_now=vecgrid[sn_now].getleft()-1;//sn_now是在矢量中的序号，等于球真实序号-1
			}else
			{
				flagoutrange=1;
				j=fabs(dcol);
			}
		}
	}else{
		for(int j=0;j<fabs(dcol);j++)
		{
			if(sn_now>=0) {
				sn_now=vecgrid[sn_now].getright()-1;//sn_now是在矢量中的序号，等于球真实序号-1
			}else
			{
				flagoutrange=1;
				j=fabs(dcol);
			}
		}
	}
    if(drow <=0) {
        for(int j=0;j<fabs(drow);j++)
        {
            if(sn_now>=0) {
                sn_now=vecgrid[sn_now].getabove()-1;//sn_now是在矢量中的序号，等于球真实序号-1
            }else
            {
                flagoutrange=1;
                j=fabs(drow);
            }
        }
    }
	if(flagoutrange==1){
		return -1;
	}else{
	return sn_now+1;
	}
	
}

void trigriddisplay(){//显示下三角网格信息函数
    balltopo trigrid[n_scale][n_scale];
	int nij=0;
    for (int i=0;i<n_scale;i++){
            for(int j=0;j<=i;j++)
			{
				//矩阵和矢量的关系是
                trigrid[i][j]=vecgrid[nij];
				nij++;
			}
        }

    for(int i=1; i<=n_scale; i++)//行
        {
            for(int j=1; j<=i; j++)//列
            {
                if(j==i){printf(" %5d\n",trigrid[i-1][j-1].getsn());}
                else{printf(" %5d",trigrid[i-1][j-1].getsn());}
            }
        }
		
	// for(int i=0;i<n_total;i++)
	// {
		// printf("%5d ",vecgrid[i].getsn());
	// }
	// printf("\n");
	
}

void trigridtest(){//查看当前球及其邻居序号测试
    balltopo trigrid[n_scale][n_scale];
	int nij=0;
    for (int i=0;i<n_scale;i++)
	{
        for(int j=0;j<=i;j++)
		{
            trigrid[i][j]=vecgrid[nij];
			//printf("%d sn=%d\n",nij,vecgrid[nij].getsn());
			nij++;
		}
    }
    int n=n_scale;

    for(int i=1; i<=n; i++)//行
        {
            for(int j=1; j<=i; j++)//列
            {
                trigriddisplay();
                int topoget[5]={0};
                trigrid[i-1][j-1].gettopo(topoget);
                printf("%5c%5d%5c\n",' ',topoget[3],' ');
                printf("%5d%5d%5d\n",topoget[1],topoget[0],topoget[2]);
                printf("%5c%5d%5c\n",' ',topoget[4],' ');

                //char ch;
                //printf("please put in a key to continue:");
                //ch = getchar();
            }
        }
}

};


 #endif // SHAPE_H
