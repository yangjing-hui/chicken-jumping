#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include <stdlib.h>
#include "EasyXPng.h"
#include <time.h>
#include"Timer.h"
#pragma comment(lib,"Winmm.lib")
#define screenWidth 400
#define screenHeight 800
#define numMax 300
#define gao 40
 
IMAGE im_block,im_block1,im_block2,im_block3,im_chickenHead,im_chickenbody,im_background1,im_background2,im_background3,im_cloud;// 定¨义?图?像?对?象ó	
IMAGE im_progressbar,im_victory,im_gameover,im_cover,im_startKey,im_statementKey,im_statement,im_startKeyGlow,im_statementKeyGlow;//********
IMAGE im_progressbarHead;
float p=0;
int bodynum=0;
int finddd=0;
int crash=0;
int display=0;
int clicknum=0;
int glow=0;
static int nowSecond = 0; // 记录当前程序运行了多少秒
//枚举类型，砖块所有可能的状态
enum blockType{n_Lengthwise,three_Next,n_Broadwise,n_32Block};
enum backgoundType{one,two,three};

void PlayMusicOnce(TCHAR fileName[80]) // 播放一次音乐函数
{
	TCHAR cmdString1[50];
	_stprintf(cmdString1, _T("open %s alias tmpmusic"), fileName);	// 生成命令字符串
	mciSendString(_T("close tmpmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
	mciSendString(cmdString1, NULL, 0, NULL); // 打开音乐
	mciSendString(_T("play tmpmusic"), NULL, 0, NULL); // 仅播放一次	
}


class Cloud //定¨¨§义??玩??家òòА?类え?え琛?*************************
{
public:
	IMAGE im_cloud;
	int cloud_x,cloud_y;
	int cloud_v;
	void draw() // 显??示??玩??家òòА?相ààА?关??信??息￠￠é	
	{
		putimagePng(cloud_x,cloud_y,&im_cloud);
	}
	void update()
	{
		cloud_x+=cloud_v;
		if(cloud_x==-200)
		{
			cloud_x=650;
			
		}
	}

};
class Background
{
public:
	IMAGE im_background1,im_background2,im_background3;
	int x1;
	void draw()// 显示砖块
	{
		if(p<=200)
			putimagePng(x1,0,&im_background1);
		else if(p<=350&&p>200)
			putimagePng(x1,0,&im_background2);
		else if(p>350)
			putimagePng(x1,0,&im_background3);
	}
	void update()
	{
		x1=x1-1;
		if(x1==-1200)
		{
			x1=0;
		}
	}
};
class Block1
{
public:
	IMAGE im_block1;
	int x2;
	int y2;
	void draw()// 显示砖块
	{
		putimagePng(x2,y2,&im_block1);
	}
	void update()
	{
		x2=x2-1;
		if(x2==-120)
		{
			x2=400;
		}
	}
};
class Block2
{
public:
	IMAGE im_block2;
	int x2;
	void draw()//显示砖块
	{
//		putimagePng(x2,100,&im_block2);
	}
	void update()
	{
		x2=x2-1;
		if(x2==-120)
		{
			x2=400;
		}
	}
};
class Block3
{
public:
	IMAGE im_block3;
	int x2;
	int y2;
	void draw()//显示砖块
	{
		putimagePng(x2,y2,&im_block3);
	}
	void update()
	{
		x2=x2-1;
		if(x2==-120)
		{
			x2=400;
		}
	}
};
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
class Chickenhead //定义玩家类
{
public:
	IMAGE im_chickenHead;
	int m,n;//定义玩家坐标

	void draw() // 显示玩家相关信息	
	{
		putimagePng(m,n,&im_chickenHead);
	}
};
class Chickenbody //定义玩家类
{
public:
	IMAGE im_chickenbody;
	int a,b;

	void draw() // 显示玩家相关信息	
	{
		if(bodynum>0) 
		{
			putimagePng(a,b,&im_chickenbody);
		}
	}
};
Block block[numMax];//定义砖块对象
Block1 block1;
Block2 block2;
Block3 block3;
Chickenhead chickenhead;
Chickenbody chickenbody[numMax];
Background background;
Cloud cloud;
Timer timer;
int blockNum;
int gap=-43;
int pushNum=0;
int progress_v;


void threeNext()//三个紧邻
{
	blockNum=3;
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth-gao*i;
		block[i].y=screenHeight*3/4-gap;
		block[i].v=1;
	}
	pushNum=blockNum-1;
	block[2].im_block=im_block; 
	block[2].x=screenWidth+gao;
	block[2].y=screenHeight*3/4-gap-gao;
	block[2].v=1;
	finddd=1;
}
void nLengthwise()//n个斜向
{
	blockNum=rand()%9+1;
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
	blockNum=rand()%5+1;
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
void startup()//初始化
{
	mciSendString(_T("open BGM.mp3 alias bkmusic"), NULL, 0, NULL);//打开背景音乐+++++++++++++
	mciSendString(_T("play bkmusic"), NULL, 0, NULL);  // 循环播放+++++++++++++
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);
	loadimage(&im_block,_T("block.png"),gao,gao);
	loadimage(&im_block1,_T("block1.png"),3*40,8*40);
	loadimage(&im_block2,_T("block2.png"));
	loadimage(&im_block3,_T("block3.png"),3*42,3*42);
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);
	loadimage(&im_background1,_T("background1.png"));
	loadimage(&im_background2,_T("background2.png"));
	loadimage(&im_background3,_T("background3.png"));
	loadimage(&im_cloud,_T("cloud.png"));
	loadimage(&im_cover,_T("cover.png"));
	loadimage(&im_startKey,_T("startKey.png"));
	loadimage(&im_startKeyGlow,_T("startKeyGlow.png"));
	loadimage(&im_statementKey,_T("statementKey.png"));
	loadimage(&im_statementKeyGlow,_T("statementKeyGlow.png"));
	loadimage(&im_statement,_T("statement.png"));
	loadimage(&im_gameover,_T("gameover.png"));
	loadimage(&im_victory,_T("victory.png"));
	loadimage(&im_progressbar,_T("progressbar.png"));
	loadimage(&im_progressbarHead,_T("chicken.png"),100,90);
	initgraph(screenWidth,screenHeight); // 新开一个画面
	
	background.im_background1=im_background1;
	background.im_background2=im_background2;
	background.im_background3=im_background3;
	block1.im_block1=im_block1;
	block2.im_block2=im_block2;
	block3.im_block3=im_block3;

	block2.x2=400;
	block3.x2=400;
	block3.y2=153;
	background.x1=0;

	cloud.im_cloud=im_cloud;
	cloud.cloud_x=400;
	cloud.cloud_y=200;
	cloud.cloud_v=-1;

	//nLengthwise();
	//nBroadwise();
	//threeNext();
	
		chickenhead.im_chickenHead=im_chickenHead; 
		chickenhead.m=screenWidth/4;
		chickenhead.n=screenHeight*2/3+69;
	for (int i=0;i<numMax;i++)
	{
		chickenbody[i].im_chickenbody=im_chickenbody;
		chickenbody[i].a=chickenhead.m+56;
		chickenbody[i].b=chickenhead.n+gao-gao*i;
		
	}
	
}
void startupBlock()//初始化
{
	int type=rand()%4;
	if(type==n_Lengthwise)
		nLengthwise();
	else if(type==three_Next)
		threeNext();
	else if(type==n_Broadwise)
		nBroadwise();
	else if(type==n_32Block)
		n32Block();
	BeginBatchDraw();// 开始批量绘制
}


void showStartGame()// 绘制函数
{
	nowSecond=0;
	cleardevice();
	background.draw();
	cloud.draw();

	nowSecond =p;
	p=p+0.1;

	// 窗口正上方显示坚持了多少秒//	block2.draw();
	block3.draw();
	if(finddd==1)
	{
		for (int i=0;i<blockNum;i++)
		{
			block[i].draw();// 显?示?砖?块é
			if(block[blockNum-1].x==-gao)
			{
				startupBlock();
				finddd=1;//	blockNum=rand()%5+1;
			}
		}
	}
	else if(finddd==2)
	{
		block1.draw();
		if(block1.x2==-3*gao)
		{
			finddd=1;
			startupBlock();
		}
	}
	for (int i=0;i<bodynum;i++)
	{
		chickenbody[i].draw();
	}
	chickenhead.draw();
	if(nowSecond==500)
		crash=2;
	else
		timer.Sleep((600-nowSecond)/100);
	if(crash==1)
	{
		putimage(0,0,&im_gameover);

	}
	else if(crash==2)
	{
		putimage(0,0,&im_victory);

	}
	else
	{
		TCHAR s[20];
		setbkmode(TRANSPARENT); // 文字字体透明
		_stprintf(s, _T("%d"), nowSecond);
		settextcolor(BLUE); // 设定文字颜色
		settextstyle(50, 0, _T("幼圆"));//  设定文字大小、样式
		outtextxy(screenWidth*0.45, 30, s); //  输出文字内容	
		putimage(50,100,300,110,&im_progressbar,-nowSecond*3/5+300,0);
		putimagePng(nowSecond*3/5,68,&im_progressbarHead);
	}
}

void show()
{
	clock_t start=clock();
	putimagePng(0,0,&im_cover);
	putimagePng(120,725,&im_statementKey);
	if(glow==1)
	{
		putimagePng(120,725,&im_statementKeyGlow);
	}
	if(display==1)
	{
		showStartGame();
	}
	else if(display==2)
	{
		putimagePng(0,0,&im_statement);
		putimagePng(160,725,&im_startKey);
		if(glow==2)
		{
			putimagePng(160,725,&im_startKeyGlow);
		}
	}
	
	FlushBatchDraw();// 批ú量?绘?制?

}
void updateWithoutInput() // 和输入无关的更新
{
	int find=0;
	int find1=0;
	int find2=0;
	int find3=0;
	int find4=0;
	int m=0;

	progress_v=1;
	if(crash!=1&&crash!=2)
	{
		background.update();
		cloud.update();
		block3.update();
		if(chickenhead.m>=block3.x2-gao-60&&chickenhead.m<=block3.x2+60&&chickenhead.n<=block3.y2+80)
			crash=1;//小鸡头部和上方砖块碰撞，游戏失败
		else if(chickenhead.m>=block1.x2-gao-60&&chickenhead.m<block1.x2+3*gao-60&&chickenhead.n>=screenHeight*3/4-gap-gao*8-20)
			crash=1;



		if(finddd==2)
		{
			block1.update();
			//	block2.update();


			for (int i=0;i<numMax;i++)// 被推走
			{
				if(chickenbody[i].a+gao==block1.x2)
				{
					if(chickenbody[i].b>=block1.y2-20)
					{
						chickenbody[i].a=chickenbody[i].a-1;
						find2=1;//小鸡身体与32相撞左移
					}
				}
			}

			for (int i=0;i<numMax;i++)// 剩下的身体下落
			{
				if(find2==1)
				{
					if(chickenbody[i].a==block1.x2+4*gao-40)
					{
						if(chickenbody[i].b<block1.y2-20)
						{
							chickenbody[i].b=chickenbody[i].b+8*gao;
							find3=1;
						}
					}
				}
			}
			if(find3==1)
				chickenhead.n=chickenhead.n+8*gao;
		}

		else if(finddd==1)
		{
			for (int i=0;i<numMax;i++)
			{
				block[i].update(); // 更新砖块的位置、速度
			}
			for(int i=0;i<numMax;i++)
			{
				if(chickenhead.m+100==block[i].x)
				{
					if(chickenhead.n>=block[i].y-10&&chickenhead.n<=block[i].y+10)
					{
						crash=1;//小鸡头和前方砖块碰撞，游戏失败
					}
				}
			}
			for (int i=0;i<bodynum;i++)// 被推走
			{
				for(int j=0;j<bodynum;j++)
				{
					if(chickenbody[j].b>=block[i].y-10&&chickenbody[j].b<=block[i].y+10)//若body与block接触
					{
						if(chickenbody[j].a+gao==block[i].x)//若body纵坐标小于block纵坐标
						{
							chickenbody[j].a=chickenbody[j].a-1;
							find=1;//判断相撞
						}
					}
				}
			}
			for (int i=0;i<bodynum;i++)//下落
			{
				for(int j=0;j<numMax;j++)//遍历所有body
				{
					if(find==1)//若相撞
					{
						if(chickenbody[j].a==block[i].x+40)//找出相撞的那个body
						{
							if(chickenbody[j].b<block[i].y)
							{
								if(block[blockNum-1].x<chickenbody[j].a)//如果最后画的那个block的横坐标小于body，即所有砖块离开非碰撞body
								{
									chickenbody[j].b=chickenbody[j].b+gao*pushNum;
									find1=1;//判断此时身体和头应该下落
								}
							}
						}
					}
				}
			}

			if(find1==1)
				chickenhead.n=chickenhead.n+gao*pushNum;
		}
	}
}
void updateWithInput() // 和输入有关的更新
{
	MOUSEMSG m;//定义鼠标消息
	if(MouseHit())
	{
		m=GetMouseMsg();//获得鼠标消息
		if(m.x>=120&&m.x<=280&&m.y>=725&&m.y<=773&&clicknum==0)
		{
			glow=1;//游戏说明键亮
		}
		else 
		{
			glow=0;//不亮
		}
		if(m.x>=160&&m.x<=240&&m.y>=725&&m.y<=773&&clicknum==1&&display==2)
		{
			glow=2;//开始键亮
		}
		

		if(m.uMsg==WM_LBUTTONDOWN)//如?果?按恪?下?鼠酣?标括?左哩?键ü
		{
			
			clicknum++;
			PlayMusicOnce(_T("soundEffect.mp3")); 
			if(clicknum==1)
			{
				if(m.x>=120&&m.x<=280&&m.y>=725&&m.y<=773)
				{
					display=2;//按了游戏说明键
				}
				else
				{
					display=1;//开始游戏
				}
				//clicknum++;
			}
			else if(clicknum==2&&display==2&&m.x>=160&&m.x<=240&&m.y>=725&&m.y<=773)
			{
				display=1;
			}

			
			else
			{
				bodynum++;

				if(bodynum>100)
				{
					bodynum=100;
					chickenhead.n=chickenhead.n;
				}
				else
					chickenhead.n=chickenhead.n-40;
			}
		}
		if(chickenhead.n<=block3.y2-20)
			crash=1;
		
	}
}
int main()
{

	srand(time(0));	
	startup();
	startupBlock();

	while(1)// 重复运行
	{
		srand(time(0));
		show();

		if(display==1)
		{
			updateWithoutInput(); 
		}
		updateWithInput();
		
	}

	_getch();
	return 0;
	EndBatchDraw();
}
