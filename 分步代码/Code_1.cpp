//砖块移动，小鸡出现
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include "EasyXPng.h"
#define screenWidth 400//定义画面宽度
#define screenHeight 800//定义画面高度
#define numMax 300//定义最大数量

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

class Chicken //定义玩家类
{
public:
	IMAGE im_chickenHead;//定义玩家图像
	int x,y;//定义玩家坐标

	void draw() // 显示玩家图像	
	{
		putimagePng(screenWidth/4,screenHeight*2/3,&im_chickenHead);
	}
};
IMAGE im_block,im_chickenHead;// 定义图像对象	
Block block[numMax];//定义砖块对象
Chicken chicken[numMax];

void startup()//初始化
{
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);//读取图片到img对象中
	loadimage(&im_block,_T("block.png"),40,40);//读取图片到img对象中
	initgraph(screenWidth,screenHeight); // 新开一个画面
	
	for (int i=0;i<numMax;i++)//设定砖块和玩家信息的初始值
	{
		block[i].im_block=im_block;
		block[i].x=screenWidth;
		block[i].y=screenHeight/2;
		block[i].v=2;
		chicken[i].im_chickenHead=im_chickenHead;
		chicken[i].x=screenWidth/4;
		chicken[i].y=screenHeight*2/3;
	}
	
	BeginBatchDraw();// 开始批量绘制
}
void show()// 绘制函数
{
	setbkcolor(RGB(189,247,255));//设置背景底色
	cleardevice();// 显示背景	

	for (int i=0;i<numMax;i++)
	{
		block[i].draw();//显示砖块
		chicken[i].draw();//显示玩家
	}

	FlushBatchDraw();// 批量绘制
	Sleep(10);//控制速度
}

void updateWithoutInput() // 和输入无关的更新
{
	for (int i=0;i<numMax;i++)
	{
		block[i].update(); //更新砖块的位置、速度
	}
}

int main()//主函数
{
	startup();
	while(1)// 重复运行
	{
		show();
		updateWithoutInput(); 
	}
	_getch();
	return 0;

	EndBatchDraw();
}