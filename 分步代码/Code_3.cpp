//砖块推走身体，上方头部和身体下落
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include"EasyXPng.h"
#include<time.h>
#include"Timer.h"
#define screenWidth 400
#define screenHeight 800
#define numMax 300


//全局变量
int bodynum=0;//身体数量
int blockNum;//砖块数量
int gao=40;

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
	IMAGE im_chickenHead;
	int m,n;//定义头部坐标

	void draw()//显示头部相关信息	
	{
		putimagePng(m,n,&im_chickenHead);
	}
};

class Chickenbody//定义身体类
{
public:
	IMAGE im_chickenbody;
	int a,b;

	void draw()//显示身体相关信息	
	{
		if(bodynum>0) 
		{
			putimagePng(a,b,&im_chickenbody);
		}
	}
};

IMAGE im_block,im_chickenHead,im_chickenbody;// 定义图像对象	
Block block[numMax];//定义砖块对象
Chickenhead chickenhead;//定义头部对象
Chickenbody chickenbody[numMax];//定义身体对象

void twoBroadwise()//两个横向砖块
{
	blockNum=2;
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth+gao*i;
		block[i].y=screenHeight/2-26;
		block[i].v=1;
	}
}

void startup()//初始化
{
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);
	loadimage(&im_block,_T("block.png"),gao,gao);
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);
	initgraph(screenWidth,screenHeight);//新开一个画面
	
	twoBroadwise();//两个横向砖块的数据
	chickenhead.im_chickenHead=im_chickenHead;//头部的数据
	chickenhead.m=screenWidth/4;
	chickenhead.n=screenHeight*2/3;
	for (int i=0;i<numMax;i++)
	{
		chickenbody[i].im_chickenbody=im_chickenbody;
		chickenbody[i].a=chickenhead.m+58;//调整数据，使身体与头部恰好对齐
		chickenbody[i].b=chickenhead.n+gao-gao*i;
	}
	BeginBatchDraw();//开始批量绘制
}

void show()//绘制函数
{
	setbkcolor(RGB(189,247,255));//设置背景底色
	cleardevice();//显示背景	
	
	for (int i=0;i<blockNum;i++)
	{
		block[i].draw();//显示砖块
		if(block[i].x==-gao)//当砖块从画面左侧消失后，在画面右侧重新生成
		{
			block[i].x=400;
		}
	}
	for (int i=0;i<bodynum;i++)
	{
		chickenbody[i].draw();//显示身体
	}
	chickenhead.draw();//显示头部

	FlushBatchDraw();//批量绘制
}

void updateWithoutInput()//和输入无关的更新
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

void updateWithInput()//和输入有关的更新
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
				chickenhead.n=chickenhead.n-gao;//头部上移
		}
	}
}
int main()
{
	startup();
	while(1)//重复运行
	{
		show();
		updateWithoutInput(); 
		updateWithInput();
		Sleep(1);
	}

	_getch();
	return 0;

	EndBatchDraw();
}