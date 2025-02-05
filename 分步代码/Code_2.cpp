// 每按左键增加一个身体
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include "EasyXPng.h"
#define screenWidth 400
#define screenHeight 800
#define numMax 300
int gao=40;//gao为砖块和身体图片的边长

int bodynum=0;//身体个数
class Block//定义砖块类
{
public:
	IMAGE im_block;//砖块图像
	int x,y;//砖块横纵坐标
	int v;//砖块速度

	void draw()//显示砖块	
	{
		putimagePng(x,y,&im_block);
	}
	void update()
	{
		x-=v;//砖块向左移动
	}
};

class Chickenhead//定义头部类
{
public:
	IMAGE im_chickenHead;//头部图像
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
		if(bodynum>0)//身体个数大于零时
		{
			for(int i=0;i<bodynum;i++)//显示相应个数的身体
			{
				putimagePng(a,b+i*gao,&im_chickenbody);
			}
		}
	}
};

IMAGE im_block,im_chickenHead,im_chickenbody;//定义图像对象	
Block block[numMax];//定义砖块对象
Chickenhead chickenhead;//定义头部对象
Chickenbody chickenbody;//定义身体对象

void startup()//初始化
{
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);
	loadimage(&im_block,_T("block.png"),gao,gao);
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);
	initgraph(screenWidth,screenHeight); // 新开一个画面

	//初始化数据
	for (int i=0;i<numMax;i++)
	{
		block[i].im_block=im_block;
		block[i].x=screenWidth;
		block[i].y=screenHeight/2-27;
		block[i].v=1;
	}	
	chickenhead.im_chickenHead=im_chickenHead;
	chickenhead.m=screenWidth/4;
	chickenhead.n=screenHeight*2/3;
	chickenbody.im_chickenbody=im_chickenbody;
	chickenbody.a=chickenhead.m+58;

	BeginBatchDraw();//开始批量绘制
}

void show()//绘制函数
{
	setbkcolor(RGB(189,247,255));
	cleardevice();//显示背景

	chickenhead.draw();
	chickenbody.draw();
	for (int i=0;i<numMax;i++)
	{
		block[i].draw();//显示砖块
	}
	FlushBatchDraw();//批量绘制
}


void updateWithoutInput()//和输入无关的更新
{
	for (int i=0;i<numMax;i++)
	{
		block[i].update();//更新砖块的位置、速度
	}
}

void updateWithInput()//和输入有关的更新
{
	MOUSEMSG m;//定义鼠标消息
	if(MouseHit())
	{
		m=GetMouseMsg();//获得鼠标消息
		if(m.uMsg==WM_LBUTTONDOWN)//如果按下鼠标左键
		{
			chickenbody.b=chickenhead.n+gao;//更新身体的纵坐标
			chickenhead.n=chickenhead.n-gao;//头部向上移动一个身体的距离
			bodynum++;
		}
	}
}

int main()
{
	startup();
	while(1)// 重复运行
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