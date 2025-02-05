//随机生成不同类型的砖块
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include <stdlib.h>
#include "EasyXPng.h"
#include <time.h>
#include"Timer.h"
#define screenWidth 450
#define screenHeight 900
#define numMax 300
#define gao 40

//全局变量
int blockNum;//每组砖块的砖块数量
int bodynum=0;
//枚举类型，列举砖块所有的可能状态
enum BlockAppearance{n_Lengthwise,n_Broadwise};
	
class Block //定义砖块类
{
public:
	IMAGE im_block;//砖块图像
	int x,y;//砖块横纵坐标
	int v;//砖块速度

	void draw()// 显示砖块	
	{
		putimagePng(x,y,&im_block);
	}
	void update()
	{
		x-=v;//砖块向左移动
	}
};

class Chickenhead //定义头部类
{
public:
	IMAGE im_chickenHead;//头部图像
	int m,n;//定义头部坐标

	void draw() // 显示头部相关信息	
	{
		putimagePng(m,n,&im_chickenHead);
	}
};

class Chickenbody //定义身体类
{
public:
	IMAGE im_chickenbody;//身体图像
	int a,b;//定义身体坐标

	void draw()//显示身体相关信息	
	{
		if(bodynum>0) 
		{
			putimagePng(a,b,&im_chickenbody);
		}
	}
};

IMAGE im_block,im_chickenHead,im_chickenbody;//定义图像对象	
Block block[numMax];//定义砖块对象
Chickenhead chickenhead;//定义头部对象
Chickenbody chickenbody[numMax];//定义身体对象


void nLengthwise()//多个纵向砖块
{
	blockNum=rand()%5+1;//随机生成每组砖块数量
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth;
		block[i].y=screenHeight/2-9+gao*i;
		block[i].v=1;
	}
}

void nBroadwise()//多个横向砖块
{
	blockNum=rand()%5+1;//随机生成每组砖块数量
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth+gao*i;
		block[i].y=screenHeight/2-9;
		block[i].v=1;
	}
}

void startup()//初始化
{
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);//导入图片
	loadimage(&im_block,_T("block.png"),gao,gao);
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);
	initgraph(screenWidth,screenHeight); // 新开一个画面

	//身体和头部基本数据
	chickenhead.im_chickenHead=im_chickenHead; 
	chickenhead.m=screenWidth/4;
	chickenhead.n=screenHeight*2/3;
	for (int i=0;i<numMax;i++)
	{
		chickenbody[i].im_chickenbody=im_chickenbody;
		chickenbody[i].a=chickenhead.m+58;
		chickenbody[i].b=chickenhead.n+gao-gao*i;
		
	}
	BeginBatchDraw();//开始批量绘制
}

void startupBlock()//砖块初始化，用于随机生成不同类型的砖块
{
	int k=rand();//随机生成数据
	int type=k%2;//随机生成0、1数据
	if(type==n_Lengthwise)//如果type=0
		nLengthwise();//生成多个纵向砖块
	else if(type==n_Broadwise)//如果type=1
		nBroadwise();//生成多个横向砖块
	
	BeginBatchDraw();// 开始批量绘制
}


void show()// 绘制函数
{
	setbkcolor(RGB(189,247,255));//设置背景底色
	cleardevice();//显示背景	

	for (int i=0;i<blockNum;i++)
	{
		block[i].draw();// 显示砖块
		if(block[i].x==-gao*6)
		{
			startupBlock();//重新生成随机类型的砖块
		}
	}

	for (int i=0;i<bodynum;i++)
	{
		chickenbody[i].draw();
	}
	chickenhead.draw();

	
	FlushBatchDraw();// 批量绘制

}

void updateWithoutInput() // 和输入无关的更新
{
	int find1=0;//判断头部是否下落的变量
	int pushNum=0;//被推走的身体数量

	//判断身体是否与砖块碰撞
	for (int i=0;i<blockNum;i++)//遍历所有砖块
	{
		for(int j=0;j<numMax;j++)//遍历所有身体,找出与砖块相碰的身体
		{
			if(chickenbody[j].a+gao==block[i].x)//如果身体右侧和砖块左侧的横坐标恰好相等（即判断身体与砖块相撞）
			{
				if(chickenbody[j].b>=block[i].y-20&&chickenbody[j].b<=block[i].y+20)//找出与每个砖块距离最近的身体
				{
					chickenbody[j].a=chickenbody[j].a-1;//该身体被砖块推走，向左移动，速度与砖块相同
					pushNum++;//被推走的身体数量加一
				}
			}
		}
	}

	//某身体被砖块推走后，其上方身体及头部的下落
	for (int i=0;i<blockNum;i++)//遍历所有砖块
	{
		for(int j=0;j<numMax;j++)//遍历所有身体
		{
				if(chickenbody[j].a==block[i].x+gao)//如果该砖块离开头身列（小鸡头部和未被推走的身体所在列）
				{
					if(chickenbody[j].b<block[i].y)//找出砖块上面的身体
					{
						if(block[blockNum-1].x<chickenbody[j].a)//如果该砖块为每组砖块最后一个
						{
							chickenbody[j].b=chickenbody[j].b+gao;//身体下落
							find1=1;//该变量赋值为1，表示头部应下落
						}
					}
				}
		}
	}
	if(find1==1)//如果头部应下落
	{
		chickenhead.n=chickenhead.n+gao*pushNum;//头部下降对应高度
	} 
	for (int i=0;i<numMax;i++)
	{
		block[i].update();//更新砖块的位置、速度
	}
}

void updateWithInput() // 和输入有关的更新
{
	MOUSEMSG m;//定义鼠标消息
	if(MouseHit())//如果鼠标有消息
	{
		m=GetMouseMsg();//获得鼠标消息
		if(m.uMsg==WM_LBUTTONDOWN)//如果按下鼠标左键
		{
			bodynum++;
			if(bodynum>numMax)//如果身体数量超过最大值
			{
				bodynum=numMax;
				chickenhead.n=chickenhead.n;//头部保持不动
			}
			else
				chickenhead.n=chickenhead.n-40;//头部上移
		}
	}
}

int main()
{
	srand(time(0));//初始化随机种子
	startup();//初始化
	startupBlock();//初始化砖块信息

	while(1)// 重复运行
	{
		show();//绘制图像
		updateWithoutInput();//和输入无关的更新
		updateWithInput();//和输入有关的更新
		Sleep(1);//暂停
	}

	_getch();
	return 0;

	EndBatchDraw();
}