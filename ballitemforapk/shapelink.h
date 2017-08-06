#ifndef SHAPELINK_H
#define SHAPELINK_H

#include <shapeball.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <iostream>
#include <cstdlib>
#include <qDebug>


class linkshape //链接形状是由几个球连接起来构成的形状
{
private:
	int sn_shape; //<当前链接形状序号,从1开始计数，这是真实的数字
	int n_balls_inshape; //<形状的链接的球数

    std::vector<int> vec_balls_number;//球的序号
	int rows,cols; //<链接形状当前的总行数和总列数
    int rowofzero,colofzero; //因为要做规范化话，则把规范化后(0,0)的对应行号记录下来
	std::vector<coord> vec_coord_shape;//位置坐标矢量与形状矩阵相关
	std::vector<int> vec_matrix_adjacent;//邻接矩阵矢量
	std::vector<float> vec_eigen_matrix;//邻接矩阵特征值列表

    std::vector<coord> vec_coord_origin;//位置坐标矢量-在网格中的初始坐标记录下来
    std::vector<coord> vec_coord_outer;//位置坐标矢量-在网格外的初始坐标记录下来
    std::vector<coord> vec_coord_save;//位置坐标矢量-把坐标变换前的信息记录下来便于恢复
	
public:
	
	//默认的构造函数，如果要形成一个数组，是必须的。
	linkshape()
    {}
	
    //重载的构造函数，//sn当前形状序号，n_balls 为当前形状中的球数，coordmatrix[] 坐标矩阵
	linkshape(int sn, int n_balls ,coord coordmatrix[])//输入当前形状序号,链接的球数,位置坐标矢量
	{
		printf("\n");
		printf("construct a new shape \n");
		sn_shape=sn;
		n_balls_inshape=n_balls;
		int i=0;
		int j=0;

		//对位置坐标矢量做从0开始的规范化
		int rowmin=100;
		int rowmax=-100;
		int colmin=100;
		int colmax=-100;
		for(i=0;i<n_balls;i++){
			if(coordmatrix[i].row < rowmin) rowmin=coordmatrix[i].row;
			if(coordmatrix[i].row > rowmax) rowmax=coordmatrix[i].row;
			if(coordmatrix[i].col < colmin) colmin=coordmatrix[i].col;
			if(coordmatrix[i].col > colmax) colmax=coordmatrix[i].col;
		}
		const int jm=colmax-colmin;//使用了const后才使得matrix_shape定义正确
		const int im=rowmax-rowmin;//表示从0开始的最后一行行序号，总的行数等于im+1
		rows=im;//为方便起见总行数也从0开始计数
		cols=jm;
        rowofzero=rowmin;
        colofzero=colmin;
		for(i=0;i<n_balls;i++){
			coordmatrix[i].row=coordmatrix[i].row-rowmin;
			coordmatrix[i].col=coordmatrix[i].col-colmin;
		}
		printf("postition of balls: \n");
		for(i=0;i<n_balls;i++){
			vec_coord_shape.push_back(coordmatrix[i]);
			printf("row=%5d%5c col=%5d\n",coordmatrix[i].row,' ',coordmatrix[i].col);//row表示行号,col表示列号
		}
		printf("\n");
			
		
		//由位置坐标矢量构建邻接矩阵
		int matrix_adjacent[n_balls][n_balls];
		for(i=0;i<n_balls;i++){
			for(j=0;j<n_balls;j++){
				if(i==j){
					matrix_adjacent[i][j]=0;
				}else{//当两球的某一个坐标相同另一个坐标相差1时表示两个球相邻
					if((coordmatrix[i].row==coordmatrix[j].row && abs(coordmatrix[i].col-coordmatrix[j].col)==1) || (coordmatrix[i].col==coordmatrix[j].col && abs(coordmatrix[i].row-coordmatrix[j].row)==1) ) {
						matrix_adjacent[i][j]=1;
					}else{
						matrix_adjacent[i][j]=0;
					}
				}
			}
		}
		
		printf("adjacent matrix of shape: \n");
		for(i=0;i<n_balls;i++){
			for(j=0;j<n_balls;j++){
				printf("%5d%5c",matrix_adjacent[i][j],' ');
			}
			printf("\n");
		}
		
		//由邻接矩阵得到其特征值列表
		double eigen_matrix[n_balls];
		double eps=0.000001;
		double mtemp[n_balls][n_balls];
		double vtemp[n_balls][n_balls];
		for(i=0;i<n_balls;i++){
		for(j=0;j<n_balls;j++){
			mtemp[i][j]=matrix_adjacent[i][j];
			}}
		double *vpt=&vtemp[0][0];
		double *mpt=&mtemp[0][0];
		
		int flag=cjcbi(mpt,n_balls,vpt,eps,100);//如果用多维数组则必须要用指向数组的指针来传递，指针即地址
		if(flag>0){
			printf("eigen elements=");
		for (i=0; i<n_balls; i++) {
			eigen_matrix[i]=mtemp[i][i];
			printf("%13.7e  ",mtemp[i][i]);
		}
		}
		
		printf("\nin order=");
		maopao(eigen_matrix, n_balls);
		for (i=0; i<n_balls; i++) {
			printf("%13.7e  ",eigen_matrix[i]);
			vec_eigen_matrix.push_back(eigen_matrix[i]);
		}
		printf("\n");

	}
	
	
    //重载的构造函数，//sn当前形状序号，n_balls 为当前形状中的球数，coordmatrix[] 坐标矩阵，ballnumbers球序号
	linkshape(int sn, int n_balls ,coord coordmatrix[],int ballnumbers[])//输入当前形状序号,链接的球数,位置坐标矢量,球的序号
	{
		//printf("\n");
		//printf("construct a new shape \n");
		sn_shape=sn;
		n_balls_inshape=n_balls;
		
		int i=0;
		int j=0;

		for(i=0;i<n_balls;i++){
			vec_balls_number.push_back(ballnumbers[i]);
            vec_coord_origin.push_back(coordmatrix[i]);
		}
				
		//对位置坐标矢量做从0开始的规范化
		int rowmin=100;
		int rowmax=-100;
		int colmin=100;
		int colmax=-100;
		for(i=0;i<n_balls;i++){
			if(coordmatrix[i].row < rowmin) rowmin=coordmatrix[i].row;
			if(coordmatrix[i].row > rowmax) rowmax=coordmatrix[i].row;
			if(coordmatrix[i].col < colmin) colmin=coordmatrix[i].col;
			if(coordmatrix[i].col > colmax) colmax=coordmatrix[i].col;
		}
		const int jm=colmax-colmin;//使用了const后才使得matrix_shape定义正确
		const int im=rowmax-rowmin;//表示从0开始的最后一行行序号，总的行数等于im+1
		rows=im;//为方便起见总行数也从0开始计数
		cols=jm;
        rowofzero=rowmin;
        colofzero=colmin;
		for(i=0;i<n_balls;i++){
			coordmatrix[i].row=coordmatrix[i].row-rowmin;
			coordmatrix[i].col=coordmatrix[i].col-colmin;
		}
		//printf("postition of balls: \n");
		for(i=0;i<n_balls;i++){
			vec_coord_shape.push_back(coordmatrix[i]);
			//printf("row=%5d%5c col=%5d\n",coordmatrix[i].row,' ',coordmatrix[i].col);//row表示行号,col表示列号
		}
		//printf("\n");
			
		
		//由位置坐标矢量构建邻接矩阵
		int matrix_adjacent[n_balls][n_balls];
		for(i=0;i<n_balls;i++){
			for(j=0;j<n_balls;j++){
				if(i==j){
					matrix_adjacent[i][j]=0;
				}else{//当两球的某一个坐标相同另一个坐标相差1时表示两个球相邻
					if((coordmatrix[i].row==coordmatrix[j].row && abs(coordmatrix[i].col-coordmatrix[j].col)==1) || (coordmatrix[i].col==coordmatrix[j].col && abs(coordmatrix[i].row-coordmatrix[j].row)==1) ) {
						matrix_adjacent[i][j]=1;
					}else{
						matrix_adjacent[i][j]=0;
					}
				}
			}
		}
		
		// printf("adjacent matrix of shape: \n");
		// for(i=0;i<n_balls;i++){
			// for(j=0;j<n_balls;j++){
				// printf("%5d%5c",matrix_adjacent[i][j],' ');
			// }
			// printf("\n");
		// }
		
		//由邻接矩阵得到其特征值列表
		double eigen_matrix[n_balls];
		double eps=0.000001;
		double mtemp[n_balls][n_balls];
		double vtemp[n_balls][n_balls];
		for(i=0;i<n_balls;i++){
		for(j=0;j<n_balls;j++){
			mtemp[i][j]=matrix_adjacent[i][j];
			}}
		double *vpt=&vtemp[0][0];
		double *mpt=&mtemp[0][0];
		
		int flag=cjcbi(mpt,n_balls,vpt,eps,100);//如果用多维数组则必须要用指向数组的指针来传递，指针即地址
		if(flag>0){
			//printf("eigen elements=");
			for (i=0; i<n_balls; i++) {
				eigen_matrix[i]=mtemp[i][i];
				//printf("%13.7e  ",mtemp[i][i]);
			}
		}
		
		//printf("\neigen in order=");
		maopao(eigen_matrix, n_balls);
		for (i=0; i<n_balls; i++) {
			//printf("%13.7e  ",eigen_matrix[i]);
			vec_eigen_matrix.push_back(eigen_matrix[i]);
		}
		//printf("\n");

	}
	
    void saveoldcoord()//记录放置在网格外时的坐标
    {
        vec_coord_save=vec_coord_shape;
    }

    void resetoldcoord()//记录放置在网格外时的坐标
    {
        vec_coord_shape=vec_coord_save;
    }

    void recordoutercoord()//记录放置在网格外时的坐标
    {
        vec_coord_outer=vec_coord_shape;
    }

    void resetoutercoord()//恢复放置在网格外时的坐标
    {
        vec_coord_shape=vec_coord_outer;
    }

    std::vector<coord> getvec_coord()
    {
        return vec_coord_shape;
    }

	int getballnumber(const int i)//返回形状中球的序号
	{
		return vec_balls_number[i];
	}
	
	int getshapesn()
	{
		return sn_shape;
	}

    int getshapenballs()
    {
        return n_balls_inshape;
    }
	
	int getrows()
	{
		return rows;
	}

    int getcols()
    {
        return cols;
    }
	
    int getrowofzero()
	{
        return rowofzero;
	}

    int getcolofzero()
    {
        return colofzero;
    }

    int getballsn(const int i)
    {
        return vec_balls_number[i];
    }
	
	float getshapeeigen(const int i)
	{
		return vec_eigen_matrix[i];
	}
	
    int getptrow(const int i)//给出球在形状中的行坐标,i从0开始计数
	{
		return vec_coord_shape[i].row;
	}

    int getptcol(const int i)//给出球在形状中的列坐标
    {
        return vec_coord_shape[i].col;
    }

    int getptroworig(const int i)//给出球在形状中的行坐标,i从0开始计数
    {
        return vec_coord_origin[i].row;
    }

    int getptcolorig(const int i)//给出球在形状中的列坐标
    {
        return vec_coord_origin[i].col;
    }
	
    int getptcolouter(const int i)//给出球在形状中的列坐标
	{
        return vec_coord_outer[i].col;
	}

    int getptrowouter(const int i)//给出球在形状中的行坐标
    {
        return vec_coord_outer[i].row;
    }

    coord getptcoord(const int i)//给出球在形状中的坐标
    {
        return vec_coord_shape[i];
    }

    coord getptcoordouter(const int i)//给出球在形状中的坐标
    {
        return vec_coord_outer[i];
    }
	
	void pos_show()//输出形状矩阵
	{
		//各个点的坐标
		int i,j,k;
		printf("\nshape balls' coords:{");
		for(i=0;i<n_balls_inshape-1;i++){//vector的下标与数组的下标是一样的也是从0开始。
			printf("(%2d,%2d),",vec_coord_shape[i].row,vec_coord_shape[i].col);
		}
		printf("(%2d,%2d)",vec_coord_shape[n_balls_inshape-1].row,vec_coord_shape[n_balls_inshape-1].col);
		printf("}\n");
		
		
		//形状矩阵
		int matrix_shape[rows+1][cols+1]={0};
		std::cout<<"max rows from row=0 is "<<rows<<", max cols from col=0 is "<<cols<<std::endl;
		printf("\n");
		for(i=0;i<rows+1;i++)//初始化一下
			for(j=0;j<cols+1;j++) matrix_shape[i][j]=0;
		for(k=0;k<n_balls_inshape;k++){
			i=vec_coord_shape[k].row;
			j=vec_coord_shape[k].col;
			matrix_shape[i][j]=k+1;//形状矩阵位置上用球的序号表示
		}
		for(i=0;i<=rows;i++){
			for(j=0;j<=cols;j++){
				printf("%5d%5c",matrix_shape[i][j],' ');
			}
			printf("\n");
		}
		printf("\n");
	}
	
	void rotate_right()//向右旋转90度以后的坐标位置矢量
	{
		for(int i=0;i<n_balls_inshape;i++)
		{
			int row=vec_coord_shape[i].row;
			int col=vec_coord_shape[i].col;
			vec_coord_shape[i].row=col;
			vec_coord_shape[i].col=rows-row;//+1,起点从0开始不需要加1
		}
		int temp=rows;
		rows=cols;
		cols=temp;
	}
	
	void rotate_left()//向左旋转90度以后的坐标位置矢量
	{
		for(int i=0;i<n_balls_inshape;i++)
		{
			int row=vec_coord_shape[i].row;
			int col=vec_coord_shape[i].col;
			vec_coord_shape[i].row=cols-col;
			vec_coord_shape[i].col=row;
		}
		int temp=rows;
		rows=cols;
		cols=temp;
	}
	
	void mirror_lr()//左右翻转以后的坐标位置矢量
	{
		for(int i=0;i<n_balls_inshape;i++)
		{
			int row=vec_coord_shape[i].row;
			int col=vec_coord_shape[i].col;
			vec_coord_shape[i].row=row;
			vec_coord_shape[i].col=cols-col;
		}
	}
	
	void mirror_ab()//上下翻转以后的坐标位置矢量
	{
		for(int i=0;i<n_balls_inshape;i++)
		{
			int row=vec_coord_shape[i].row;
			int col=vec_coord_shape[i].col;
			vec_coord_shape[i].row=rows-row;
			vec_coord_shape[i].col=col;
		}
	}
	
	/*实对称矩阵的特征值和特征向量求解，参考徐士良-Fortran常用算法程序集－第二版.pdf
	输入数组a
	数组的秩n
	数组的特征向量v
	判断残差eps
	迭代次数jt
	输出:特征值在a的对角线上，特征向量在v中
	*/
int cjcbi(double *a,int n,double* v,double eps,int jt)
  { int i,j,p,q,u,w,t,s,l;
    p=0;
    q=0;
    u=0;
    double fm,cn,sn,omega,x,y,d;
    l=1;
    for (i=0; i<=n-1; i++)
      { v[i*n+i]=1.0;
        for (j=0; j<=n-1; j++)
          if (i!=j) v[i*n+j]=0.0;
      }
    while (1==1)
      { fm=0.0;
        for (i=1; i<=n-1; i++)
        for (j=0; j<=i-1; j++)
          { d=fabs(a[i*n+j]);
            if ((i!=j)&&(d>fm))
              { fm=d; p=i; q=j;}
          }
        if (fm<eps)  return(1);
        if (l>jt)  return(-1);
        l=l+1;
        u=p*n+q; w=p*n+p; t=q*n+p; s=q*n+q;
        x=-a[u]; y=(a[s]-a[w])/2.0;
        omega=x/sqrt(x*x+y*y);
        if (y<0.0) omega=-omega;
        sn=1.0+sqrt(1.0-omega*omega);
        sn=omega/sqrt(2.0*sn);
        cn=sqrt(1.0-sn*sn);
        fm=a[w];
        a[w]=fm*cn*cn+a[s]*sn*sn+a[u]*omega;
        a[s]=fm*sn*sn+a[s]*cn*cn-a[u]*omega;
        a[u]=0.0; a[t]=0.0;
        for (j=0; j<=n-1; j++)
        if ((j!=p)&&(j!=q))
          { u=p*n+j; w=q*n+j;
            fm=a[u];
            a[u]=fm*cn+a[w]*sn;
            a[w]=-fm*sn+a[w]*cn;
          }
        for (i=0; i<=n-1; i++)
          if ((i!=p)&&(i!=q))
            { u=i*n+p; w=i*n+q;
              fm=a[u];
              a[u]=fm*cn+a[w]*sn;
              a[w]=-fm*sn+a[w]*cn;
            }
        for (i=0; i<=n-1; i++)
          { u=i*n+p; w=i*n+q;
            fm=v[u];
            v[u]=fm*cn+v[w]*sn;
            v[w]=-fm*sn+v[w]*cn;
          }
      }
    return(1);
  }
  
 	/*冒泡算法，对数组进行排序，从大到小排序
	输入:数组stu
	数组长度n
	*/
void maopao(double stu[], int n)
{
	int i,j,k,s;
	double temp1,temp2;
	for(j=1;j<=n-1;j++){//遍历次数是n-1次
		if(fmod(j,2) != 0) {//j为奇数次，从前向后遍历
			k=(j+1)/2;
			for(i=k+1;i<=n+1-k;i++)//通过遍历终止值，减少大小比较次数,因为一次遍历能得出一个方向的极值
			{
				s=i-1;
				if( stu[s] > stu[s-1]){//从大到小排序，若要从小到大则应改为<号
				temp1=stu[s];
				stu[s]=stu[s-1];
				stu[s-1]=temp1;
				}
			}}
		else
			{//j为偶数次，从后向前遍历
			k=j/2;
			for(i=n-k;i>=k+1;i--)//注意这里i>=k+1，因为i是递减的
			{
				s=i-1;
				if(stu[s] > stu[s-1]){//从大到小排序，若要从小到大则应改为<号
				temp2=stu[s];
				stu[s]=stu[s-1];
				stu[s-1]=temp2;
				}
			}}
		//printf("\n");
		//for(i=0;i<n;i++) printf("%13.7e ",stu[i]);
	}
}
};



 #endif // SHAPELINK_H
