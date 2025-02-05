//完善游戏界面
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include <stdlib.h>
#include "EasyXPng.h"//用于显示带透明通道的png图片
#include <time.h>
#include"Timer.h"//精准延时
#pragma comment(lib,"Winmm.lib")//引用 Windows Multimedia API
#define screenWidth 400//画面宽度
#define screenHeight 800//画面高度
#define numMax 300//最大数量
#define gao 40//砖块和身体图片的边长

//定义图像对象
IMAGE im_block,im_block1,im_block2,im_block3;//砖块图像	
IMAGE im_chickenHead,im_chickenbody;//头部及身体图像
IMAGE im_background1,im_background2,im_background3,im_cloud;//背景及云朵图像
IMAGE im_victory,im_gameover,im_cover,im_startKey,im_statementKey,im_statement,im_startKeyGlow,im_statementKeyGlow;//游戏胜利、游戏失败、开始界面、游戏说明、按键图像

//全局变量
float p=0;//游戏时间
int bodynum=0;//身体数量
int finddd=0;//砖块类型
int crash=0;//赋值为1表示碰撞，赋值为2表示游戏胜利
int display=0;//赋值为1表示开始游戏，赋值为2表示查看游戏说明
int clicknum=0;//鼠标点击次数
int glow=0;//显示按键发光效果
int k=rand();//随机数
static int nowSecond = 0;//记录当前程序运行了多少秒
//枚举类型，砖块所有可能的状态
enum blockType{n_Lengthwise,three_Next,n_Broadwise,n_32Block,n_6Block,n32block,n6block};

void PlayMusicOnce(TCHAR fileName[80])//播放一次音乐函数
{
	TCHAR cmdString1[50];
	_stprintf(cmdString1,_T("open %s alias tmpmusic"),fileName);//生成命令字符串
	mciSendString(_T("close tmpmusic"),NULL,0,NULL);//先把前面一次的音乐关闭
	mciSendString(cmdString1,NULL,0,NULL);//打开音乐
	mciSendString(_T("play tmpmusic"),NULL,0,NULL);//仅播放一次	
}

class Cloud//定义云朵类
{
public:
	IMAGE im_cloud;//云朵图像
	int cloud_x,cloud_y;//云朵横纵坐标
	int cloud_v;//云朵速度
	void draw()//显示云朵
	{
		putimagePng(cloud_x,cloud_y,&im_cloud);
	}
	void update()//更新云朵
	{
		cloud_x+=cloud_v;
		if(cloud_x==-200)
		{
			cloud_x=650;
		}
	}
};
class Background//定义背景类
{
public:
	IMAGE im_background1,im_background2,im_background3;//背景图像
	int x1;//背景横坐标
	void draw()//在不同时间段显示不同背景
	{
		if(p<=333)
			putimagePng(x1,0,&im_background1);
		else if(p<=666&&p>333)
			putimagePng(x1,0,&im_background2);
		else if(p>666)
			putimagePng(x1,0,&im_background3);
	}
	void update()//更新背景
	{
		x1=x1-1;
		if(x1==-1200)
		{
			x1=0;
		}
	}
};
class Block1//32个砖块
{
public:
	IMAGE im_block1;//砖块图像
	int x2;//砖块横坐标
	int y2;//砖块纵坐标
	void draw()// 显示砖块
	{
		putimagePng(x2,y2,&im_block1);
	}
	void update()//更新砖块
	{
		x2=x2-1;
	}
};
class Block2//6个砖块
{
public:
	IMAGE im_block2;//砖块图像
	int x2;//砖块横坐标
	int y2;//砖块纵坐标
	void draw()//显示砖块
	{
		putimagePng(x2,y2,&im_block2);
	}
	void update()//更新砖块
	{
		x2=x2-1;
	}
};
class Block3//9个砖块
{
public:
	IMAGE im_block3;//砖块图像
	int x2;//砖块横坐标
	int y2;//砖块纵坐标
	void draw()//显示砖块
	{
		putimagePng(x2,y2,&im_block3);
	}
	void update()//更新砖块
	{
		x2=x2-1;
		if(x2==-120)
		{
			x2=400;
		}
	}
};
class Block//1个砖块
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

	void draw()//显示头部相关信息	
	{
		putimagePng(m,n,&im_chickenHead);
	}
};
class Chickenbody //定义身体类
{
public:
	IMAGE im_chickenbody;//身体图像
	int a,b;//身体横纵坐标

	void draw()//显示身体相关信息	
	{
		if(bodynum>0) 
		{
			putimagePng(a,b,&im_chickenbody);
		}
	}
};
Block block[numMax];//定义砖块对象
Block1 block1;//定义砖块对象
Block2 block2;//定义砖块对象
Block3 block3;//定义砖块对象
Chickenhead chickenhead;//定义头部对象
Chickenbody chickenbody[numMax];//定义身体对象
Background background;//定义背景对象
Cloud cloud;//定义云朵对象
Timer timer;//精准延时
int blockNum;//砖块数量
int gap=-43;
int pushNum=0;//被推走的身体数量

//初始化每种类型砖块所对应的数据	
void threeNext()//三个紧邻
{
	blockNum=3;
	pushNum=blockNum-1;
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth-gao*i;
		block[i].y=screenHeight*3/4-gap;
		block[i].v=1;
	}
	block[2].im_block=im_block; 
	block[2].x=screenWidth+gao;
	block[2].y=screenHeight*3/4-gap-gao;
	block[2].v=1;
	finddd=1;
}
void nLengthwise()//n个斜向
{
	blockNum=rand()%2+7;
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth+gao*i;
		block[i].y=screenHeight*3/4-gap-gao*i;
		block[i].v=1;
	}
	pushNum=blockNum;
	finddd=1;
}  
void nBroadwise()//n个横向
{
	blockNum=3;
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth+gao*i;
		block[i].y=screenHeight*3/4-gap;
		block[i].v=1;
	}
	pushNum=1;
	finddd=1;
}
void n32Block()
{
	block1.x2=400;
	block1.y2=screenHeight*3/4-gap-gao*7;
	finddd=2;
}
void n6Block()
{
	int suiji=rand()%3+5;
	block2.x2=400;
	block2.y2=screenHeight*3/4-gap-gao*suiji;
	finddd=3;
}


void startup()//初始化
{
	srand(time(0));	
	//读取图片
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);//头部
	loadimage(&im_block,_T("block.png"),gao,gao);//砖块
	loadimage(&im_block1,_T("block1.png"),3*40,8*40);//砖块1
	loadimage(&im_block2,_T("block2.png"));//砖块2
	loadimage(&im_block3,_T("block3.png"),3*42,3*42);//砖块3
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);//身体
	loadimage(&im_background1,_T("background1.png"));//背景1
	loadimage(&im_background2,_T("background2.png"));//背景2
	loadimage(&im_background3,_T("background3.png"));//背景3
	loadimage(&im_cloud,_T("cloud.png"));//云朵
	loadimage(&im_cover,_T("cover.png"));//封面（开始界面）
	loadimage(&im_startKey,_T("startKey.png"));//开始键
	loadimage(&im_startKeyGlow,_T("startKeyGlow.png"));//发光的开始键
	loadimage(&im_statementKey,_T("statementKey.png"));//游戏说明键
	loadimage(&im_statementKeyGlow,_T("statementKeyGlow.png"));//发光的游戏说明键
	loadimage(&im_statement,_T("statement.png"));//游戏说明
	loadimage(&im_gameover,_T("gameover.png"));//游戏失败界面
	loadimage(&im_victory,_T("victory.png"));//游戏胜利界面
	initgraph(screenWidth,screenHeight); // 新开一个画面
	//初始化背景数据
	background.im_background1=im_background1;
	background.im_background2=im_background2;
	background.im_background3=im_background3;
	background.x1=0;
	//初始化砖块数据
	block1.im_block1=im_block1;
	block2.im_block2=im_block2;
	block3.im_block3=im_block3;
	block3.x2=400;
	block3.y2=153;
	//初始化云的数据
	cloud.im_cloud=im_cloud;
	cloud.cloud_x=400;
	cloud.cloud_y=200;
	cloud.cloud_v=-1;
	//初始化小鸡头部数据
	chickenhead.im_chickenHead=im_chickenHead; 
	chickenhead.m=screenWidth/4;
	chickenhead.n=screenHeight*2/3+69;
	//初始化小鸡身体数据
	for (int i=0;i<numMax;i++)
	{
		chickenbody[i].im_chickenbody=im_chickenbody;
		chickenbody[i].a=chickenhead.m+58;
		chickenbody[i].b=chickenhead.n+gao-gao*i;
	}
}
void startupBlock()//砖块初始化，生成随机类型的砖块
{
	int type=k%7;
	if(type==n_32Block||type==n32block)
		n32Block();
	else if(type==n_Lengthwise)
		nLengthwise();
	else if(type==three_Next)
		threeNext();
	else if(type==n_Broadwise)
		nBroadwise();
	else if(type==n_6Block||type==n6block)
		n6Block();
	k++;
	BeginBatchDraw();// 开始批量绘制
}

void show()
{
	cleardevice();//清空画布
	background.draw();//绘制背景
	cloud.draw();//绘制云朵
	if(crash!=1)
	{
		nowSecond =p;
		p=p+0.1;//时间变化
	}
	block3.draw();//绘制上方砖块
	
	if(finddd==1)//适用于第一种类型的砖块
	{
		for (int i=0;i<blockNum;i++)
		{
			block[i].draw();//绘制砖块
			if(block[blockNum-1].x==-gao)
			{
				startupBlock();//重新生成随机类型的砖块
			}
		}
	}
	else if(finddd==2)//适用于第二种类型的砖块
	{
		block1.draw();//绘制砖块
		if(block1.x2==-3*gao)
		{
			startupBlock();//重新生成随机类型的砖块
		}
	}
	else if(finddd==3)//适用于第二种类型的砖块
	{
		block2.draw();//绘制砖块
		if(block2.x2==-3*gao)
		{
			startupBlock();//重新生成随机类型的砖块
		}
	}
	for (int i=0;i<bodynum;i++)
	{
		chickenbody[i].draw();//绘制小鸡身体
	}
	chickenhead.draw();//绘制小鸡头部
	if(nowSecond==1000)
		crash=2;//跑完全程，游戏胜利
	else
		timer.Sleep((1000-nowSecond)/200);//精准延时，游戏进行速度逐渐加快
	//在上方显示进度条和小鸡运动距离
	TCHAR s[20];
	setbkmode(TRANSPARENT);//文字字体透明
	_stprintf(s, _T("%d"), nowSecond);
	settextcolor(BLUE);//设定文字颜色
	settextstyle(50, 0, _T("幼圆"));//设定文字大小、样式
	outtextxy(screenWidth*0.45, 30, s);//输出文字内容
	FlushBatchDraw();// 批量绘制
}
void updateWithoutInput() // 和输入无关的更新
{
	//判断头部是否下落的变量
	int find1=0;
	int find2=0;
	int find3=0;

	if(crash!=1&&crash!=2)//判断游戏是否仍在进行（不胜利不失败）
	{
		background.update();//更新背景
		cloud.update();//更新云朵
		block3.update();//更新砖块
		if(chickenhead.m>=block3.x2-gao-60&&chickenhead.m<=block3.x2+60&&chickenhead.n<=block3.y2+80)
			crash=1;//小鸡头部和上方砖块碰撞，游戏失败
		if(chickenhead.n<=block3.y2-20)
			crash=1;//小鸡头和顶部砖块碰撞，游戏失败

		if(finddd==3)//适用于第三种类型的砖块
		{
			block2.update();//更新砖块2

			if(chickenhead.n<=block2.y2&&chickenhead.n>=block2.y2-2*gao+20&&chickenhead.m>=block2.x2-100&&chickenhead.m<=block2.x2+gao*3-60)
				crash=1;//小鸡头和前方砖块碰撞，游戏失败
			
			if(chickenhead.n<=block2.y2+10)//小鸡头部在砖块上方
			{
			
				//判断身体是否与砖块碰撞
				for (int i=0;i<numMax;i++)//遍历所有身体,找出与砖块相碰的身体
				{
					if(chickenbody[i].a+gao==block2.x2)//如果身体右侧和砖块左侧的横坐标恰好相等（即判断身体与砖块相撞）
					{
						if(chickenbody[i].b<=block2.y2+gao+20&&chickenbody[i].b>=block2.y2-20)//找出与每个砖块距离最近的身体
						{
							chickenbody[i].a=chickenbody[i].a-1;//该身体被砖块推走，向左移动，速度与砖块相同
						}
					}
				}
				
				//某身体被砖块推走后，其上方身体及头部的下落
				for (int i=0;i<numMax;i++)//遍历所有身体
				{
					if(chickenbody[i].b<=block2.y2+20)//找出砖块上面的身体
					{
						if(chickenbody[i].a==block2.x2+gao*3)//如果上方身体左侧与砖块右侧恰好相等（即将下落）
						{
							chickenbody[i].b=chickenbody[i].b+gao*2;//身体下落
							find3=1;//该变量赋值为1，表示头部应下落
						}
					}
				}
				if(find3==1)//如果头部应下落
					chickenhead.n=chickenhead.n+gao*2;//头部下降对应高度
			}
		}

		else if(finddd==2)//适用于第二种类型的砖块
		{
			if(chickenhead.m>=block1.x2-gao-60&&chickenhead.m<block1.x2+3*gao-60&&chickenhead.n>=screenHeight*3/4-gap-gao*8-20)
				crash=1;//小鸡头和前方砖块碰撞，游戏失败
			block1.update();//更新砖块1
			
			//判断身体是否与砖块碰撞
			for (int i=0;i<numMax;i++)////遍历所有身体,找出与砖块相碰的身体
			{
				if(chickenbody[i].a+gao==block1.x2)//如果身体右侧和砖块左侧的横坐标恰好相等（即判断身体与砖块相撞）
				{
					if(chickenbody[i].b>=block1.y2-20)//找出与砖块相碰的身体
					{
						chickenbody[i].a=chickenbody[i].a-1;//该身体被砖块推走，向左移动，速度与砖块相同
					}
				}
			}
			
			//某身体被砖块推走后，其上方身体及头部的下落
			for (int i=0;i<numMax;i++)//遍历所有身体
			{
				if(chickenbody[i].a==block1.x2+4*gao-40)//如果上方身体左侧与砖块右侧恰好相等（即将下落）
				{
					if(chickenbody[i].b<block1.y2-20)//找出砖块上面的身体
					{
						chickenbody[i].b=chickenbody[i].b+8*gao;//身体下落
						find2=1;//该变量赋值为1，表示头部应下落
					}
				}
			}
			if(find2==1)//如果头部应下落
				chickenhead.n=chickenhead.n+8*gao;//头部下降对应高度
		}

		else if(finddd==1)//适用于第一种类型的砖块
		{
			for (int i=0;i<blockNum;i++)//遍历所有砖块
			{
				block[i].update(); // 更新砖块的位置、速度
			}
			
			//判断身体是否与砖块碰撞
			for(int i=0;i<blockNum;i++)//遍历所有砖块
			{

				if(chickenhead.m>=block[i].x-100&&chickenhead.m<=block[i].x-60)//限制小鸡头部与砖块碰撞范围
				{
					if(chickenhead.n<=block[i].y+10&&chickenhead.n>=block[i].y-50)
					{
						crash=1;//小鸡头和前方砖块碰撞，游戏失败
					}
				}
			}
			
			//某身体被砖块推走后，其上方身体及头部的下落
			for (int i=0;i<blockNum;i++)//遍历所有砖块
			{
				for(int j=0;j<numMax;j++)//遍历所有身体,找出与砖块相碰的身体
				{
					if(chickenbody[j].b>=block[i].y-20&&chickenbody[j].b<=block[i].y+20)//找出与每个砖块距离最近的身体
					{
						if(chickenbody[j].a+gao==block[i].x)//如果身体右侧和砖块左侧的横坐标恰好相等（即判断身体与砖块相撞）
						{
							chickenbody[j].a=chickenbody[j].a-1;//该身体被砖块推走，向左移动，速度与砖块相同
						}
					}
				}
			}
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
								chickenbody[j].b=chickenbody[j].b+gao*pushNum;//身体下落
								find1=1;//该变量赋值为1，表示头部应下落
							}
						}
					}
				}
			}
			if(find1==1)//如果头部应下落
				chickenhead.n=chickenhead.n+gao*pushNum;//头部下降对应高度
		}
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
				bodynum++;//小鸡身体数加一
				if(bodynum>numMax)//如果身体数量超过最大值
				{
					bodynum=numMax;//身体数量不变
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
	startupBlock();

	while(1)// 重复运行
	{
		srand(time(0));
		show();
		updateWithoutInput(); 
		updateWithInput();
		
	}

	_getch();
	return 0;
	EndBatchDraw();
}