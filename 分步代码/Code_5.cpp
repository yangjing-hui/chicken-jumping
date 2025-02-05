//������Ϸ����
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include <stdlib.h>
#include "EasyXPng.h"//������ʾ��͸��ͨ����pngͼƬ
#include <time.h>
#include"Timer.h"//��׼��ʱ
#pragma comment(lib,"Winmm.lib")//���� Windows Multimedia API
#define screenWidth 400//������
#define screenHeight 800//����߶�
#define numMax 300//�������
#define gao 40//ש�������ͼƬ�ı߳�

//����ͼ�����
IMAGE im_block,im_block1,im_block2,im_block3;//ש��ͼ��	
IMAGE im_chickenHead,im_chickenbody;//ͷ��������ͼ��
IMAGE im_background1,im_background2,im_background3,im_cloud;//�������ƶ�ͼ��
IMAGE im_victory,im_gameover,im_cover,im_startKey,im_statementKey,im_statement,im_startKeyGlow,im_statementKeyGlow;//��Ϸʤ������Ϸʧ�ܡ���ʼ���桢��Ϸ˵��������ͼ��

//ȫ�ֱ���
float p=0;//��Ϸʱ��
int bodynum=0;//��������
int finddd=0;//ש������
int crash=0;//��ֵΪ1��ʾ��ײ����ֵΪ2��ʾ��Ϸʤ��
int display=0;//��ֵΪ1��ʾ��ʼ��Ϸ����ֵΪ2��ʾ�鿴��Ϸ˵��
int clicknum=0;//���������
int glow=0;//��ʾ��������Ч��
int k=rand();//�����
static int nowSecond = 0;//��¼��ǰ���������˶�����
//ö�����ͣ�ש�����п��ܵ�״̬
enum blockType{n_Lengthwise,three_Next,n_Broadwise,n_32Block,n_6Block,n32block,n6block};

void PlayMusicOnce(TCHAR fileName[80])//����һ�����ֺ���
{
	TCHAR cmdString1[50];
	_stprintf(cmdString1,_T("open %s alias tmpmusic"),fileName);//���������ַ���
	mciSendString(_T("close tmpmusic"),NULL,0,NULL);//�Ȱ�ǰ��һ�ε����ֹر�
	mciSendString(cmdString1,NULL,0,NULL);//������
	mciSendString(_T("play tmpmusic"),NULL,0,NULL);//������һ��	
}

class Cloud//�����ƶ���
{
public:
	IMAGE im_cloud;//�ƶ�ͼ��
	int cloud_x,cloud_y;//�ƶ��������
	int cloud_v;//�ƶ��ٶ�
	void draw()//��ʾ�ƶ�
	{
		putimagePng(cloud_x,cloud_y,&im_cloud);
	}
	void update()//�����ƶ�
	{
		cloud_x+=cloud_v;
		if(cloud_x==-200)
		{
			cloud_x=650;
		}
	}
};
class Background//���屳����
{
public:
	IMAGE im_background1,im_background2,im_background3;//����ͼ��
	int x1;//����������
	void draw()//�ڲ�ͬʱ�����ʾ��ͬ����
	{
		if(p<=333)
			putimagePng(x1,0,&im_background1);
		else if(p<=666&&p>333)
			putimagePng(x1,0,&im_background2);
		else if(p>666)
			putimagePng(x1,0,&im_background3);
	}
	void update()//���±���
	{
		x1=x1-1;
		if(x1==-1200)
		{
			x1=0;
		}
	}
};
class Block1//32��ש��
{
public:
	IMAGE im_block1;//ש��ͼ��
	int x2;//ש�������
	int y2;//ש��������
	void draw()// ��ʾש��
	{
		putimagePng(x2,y2,&im_block1);
	}
	void update()//����ש��
	{
		x2=x2-1;
	}
};
class Block2//6��ש��
{
public:
	IMAGE im_block2;//ש��ͼ��
	int x2;//ש�������
	int y2;//ש��������
	void draw()//��ʾש��
	{
		putimagePng(x2,y2,&im_block2);
	}
	void update()//����ש��
	{
		x2=x2-1;
	}
};
class Block3//9��ש��
{
public:
	IMAGE im_block3;//ש��ͼ��
	int x2;//ש�������
	int y2;//ש��������
	void draw()//��ʾש��
	{
		putimagePng(x2,y2,&im_block3);
	}
	void update()//����ש��
	{
		x2=x2-1;
		if(x2==-120)
		{
			x2=400;
		}
	}
};
class Block//1��ש��
{
public:
	IMAGE im_block;//ש��ͼ��
	int x,y;//ש���������
	int v;//ש���ٶ�

	void draw()// ��ʾש��
	{
		putimagePng(x,y,&im_block);
	}
	void update()
	{
		x-=v;//ש�������ƶ�
	}
};
class Chickenhead //����ͷ����
{
public:
	IMAGE im_chickenHead;//ͷ��ͼ��
	int m,n;//����ͷ������

	void draw()//��ʾͷ�������Ϣ	
	{
		putimagePng(m,n,&im_chickenHead);
	}
};
class Chickenbody //����������
{
public:
	IMAGE im_chickenbody;//����ͼ��
	int a,b;//�����������

	void draw()//��ʾ���������Ϣ	
	{
		if(bodynum>0) 
		{
			putimagePng(a,b,&im_chickenbody);
		}
	}
};
Block block[numMax];//����ש�����
Block1 block1;//����ש�����
Block2 block2;//����ש�����
Block3 block3;//����ש�����
Chickenhead chickenhead;//����ͷ������
Chickenbody chickenbody[numMax];//�����������
Background background;//���屳������
Cloud cloud;//�����ƶ����
Timer timer;//��׼��ʱ
int blockNum;//ש������
int gap=-43;
int pushNum=0;//�����ߵ���������

//��ʼ��ÿ������ש������Ӧ������	
void threeNext()//��������
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
void nLengthwise()//n��б��
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
void nBroadwise()//n������
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


void startup()//��ʼ��
{
	srand(time(0));	
	//��ȡͼƬ
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);//ͷ��
	loadimage(&im_block,_T("block.png"),gao,gao);//ש��
	loadimage(&im_block1,_T("block1.png"),3*40,8*40);//ש��1
	loadimage(&im_block2,_T("block2.png"));//ש��2
	loadimage(&im_block3,_T("block3.png"),3*42,3*42);//ש��3
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);//����
	loadimage(&im_background1,_T("background1.png"));//����1
	loadimage(&im_background2,_T("background2.png"));//����2
	loadimage(&im_background3,_T("background3.png"));//����3
	loadimage(&im_cloud,_T("cloud.png"));//�ƶ�
	loadimage(&im_cover,_T("cover.png"));//���棨��ʼ���棩
	loadimage(&im_startKey,_T("startKey.png"));//��ʼ��
	loadimage(&im_startKeyGlow,_T("startKeyGlow.png"));//����Ŀ�ʼ��
	loadimage(&im_statementKey,_T("statementKey.png"));//��Ϸ˵����
	loadimage(&im_statementKeyGlow,_T("statementKeyGlow.png"));//�������Ϸ˵����
	loadimage(&im_statement,_T("statement.png"));//��Ϸ˵��
	loadimage(&im_gameover,_T("gameover.png"));//��Ϸʧ�ܽ���
	loadimage(&im_victory,_T("victory.png"));//��Ϸʤ������
	initgraph(screenWidth,screenHeight); // �¿�һ������
	//��ʼ����������
	background.im_background1=im_background1;
	background.im_background2=im_background2;
	background.im_background3=im_background3;
	background.x1=0;
	//��ʼ��ש������
	block1.im_block1=im_block1;
	block2.im_block2=im_block2;
	block3.im_block3=im_block3;
	block3.x2=400;
	block3.y2=153;
	//��ʼ���Ƶ�����
	cloud.im_cloud=im_cloud;
	cloud.cloud_x=400;
	cloud.cloud_y=200;
	cloud.cloud_v=-1;
	//��ʼ��С��ͷ������
	chickenhead.im_chickenHead=im_chickenHead; 
	chickenhead.m=screenWidth/4;
	chickenhead.n=screenHeight*2/3+69;
	//��ʼ��С����������
	for (int i=0;i<numMax;i++)
	{
		chickenbody[i].im_chickenbody=im_chickenbody;
		chickenbody[i].a=chickenhead.m+58;
		chickenbody[i].b=chickenhead.n+gao-gao*i;
	}
}
void startupBlock()//ש���ʼ��������������͵�ש��
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
	BeginBatchDraw();// ��ʼ��������
}

void show()
{
	cleardevice();//��ջ���
	background.draw();//���Ʊ���
	cloud.draw();//�����ƶ�
	if(crash!=1)
	{
		nowSecond =p;
		p=p+0.1;//ʱ��仯
	}
	block3.draw();//�����Ϸ�ש��
	
	if(finddd==1)//�����ڵ�һ�����͵�ש��
	{
		for (int i=0;i<blockNum;i++)
		{
			block[i].draw();//����ש��
			if(block[blockNum-1].x==-gao)
			{
				startupBlock();//��������������͵�ש��
			}
		}
	}
	else if(finddd==2)//�����ڵڶ������͵�ש��
	{
		block1.draw();//����ש��
		if(block1.x2==-3*gao)
		{
			startupBlock();//��������������͵�ש��
		}
	}
	else if(finddd==3)//�����ڵڶ������͵�ש��
	{
		block2.draw();//����ש��
		if(block2.x2==-3*gao)
		{
			startupBlock();//��������������͵�ש��
		}
	}
	for (int i=0;i<bodynum;i++)
	{
		chickenbody[i].draw();//����С������
	}
	chickenhead.draw();//����С��ͷ��
	if(nowSecond==1000)
		crash=2;//����ȫ�̣���Ϸʤ��
	else
		timer.Sleep((1000-nowSecond)/200);//��׼��ʱ����Ϸ�����ٶ��𽥼ӿ�
	//���Ϸ���ʾ��������С���˶�����
	TCHAR s[20];
	setbkmode(TRANSPARENT);//��������͸��
	_stprintf(s, _T("%d"), nowSecond);
	settextcolor(BLUE);//�趨������ɫ
	settextstyle(50, 0, _T("��Բ"));//�趨���ִ�С����ʽ
	outtextxy(screenWidth*0.45, 30, s);//�����������
	FlushBatchDraw();// ��������
}
void updateWithoutInput() // �������޹صĸ���
{
	//�ж�ͷ���Ƿ�����ı���
	int find1=0;
	int find2=0;
	int find3=0;

	if(crash!=1&&crash!=2)//�ж���Ϸ�Ƿ����ڽ��У���ʤ����ʧ�ܣ�
	{
		background.update();//���±���
		cloud.update();//�����ƶ�
		block3.update();//����ש��
		if(chickenhead.m>=block3.x2-gao-60&&chickenhead.m<=block3.x2+60&&chickenhead.n<=block3.y2+80)
			crash=1;//С��ͷ�����Ϸ�ש����ײ����Ϸʧ��
		if(chickenhead.n<=block3.y2-20)
			crash=1;//С��ͷ�Ͷ���ש����ײ����Ϸʧ��

		if(finddd==3)//�����ڵ��������͵�ש��
		{
			block2.update();//����ש��2

			if(chickenhead.n<=block2.y2&&chickenhead.n>=block2.y2-2*gao+20&&chickenhead.m>=block2.x2-100&&chickenhead.m<=block2.x2+gao*3-60)
				crash=1;//С��ͷ��ǰ��ש����ײ����Ϸʧ��
			
			if(chickenhead.n<=block2.y2+10)//С��ͷ����ש���Ϸ�
			{
			
				//�ж������Ƿ���ש����ײ
				for (int i=0;i<numMax;i++)//������������,�ҳ���ש������������
				{
					if(chickenbody[i].a+gao==block2.x2)//��������Ҳ��ש�����ĺ�����ǡ����ȣ����ж�������ש����ײ��
					{
						if(chickenbody[i].b<=block2.y2+gao+20&&chickenbody[i].b>=block2.y2-20)//�ҳ���ÿ��ש��������������
						{
							chickenbody[i].a=chickenbody[i].a-1;//�����屻ש�����ߣ������ƶ����ٶ���ש����ͬ
						}
					}
				}
				
				//ĳ���屻ש�����ߺ����Ϸ����弰ͷ��������
				for (int i=0;i<numMax;i++)//������������
				{
					if(chickenbody[i].b<=block2.y2+20)//�ҳ�ש�����������
					{
						if(chickenbody[i].a==block2.x2+gao*3)//����Ϸ����������ש���Ҳ�ǡ����ȣ��������䣩
						{
							chickenbody[i].b=chickenbody[i].b+gao*2;//��������
							find3=1;//�ñ�����ֵΪ1����ʾͷ��Ӧ����
						}
					}
				}
				if(find3==1)//���ͷ��Ӧ����
					chickenhead.n=chickenhead.n+gao*2;//ͷ���½���Ӧ�߶�
			}
		}

		else if(finddd==2)//�����ڵڶ������͵�ש��
		{
			if(chickenhead.m>=block1.x2-gao-60&&chickenhead.m<block1.x2+3*gao-60&&chickenhead.n>=screenHeight*3/4-gap-gao*8-20)
				crash=1;//С��ͷ��ǰ��ש����ײ����Ϸʧ��
			block1.update();//����ש��1
			
			//�ж������Ƿ���ש����ײ
			for (int i=0;i<numMax;i++)////������������,�ҳ���ש������������
			{
				if(chickenbody[i].a+gao==block1.x2)//��������Ҳ��ש�����ĺ�����ǡ����ȣ����ж�������ש����ײ��
				{
					if(chickenbody[i].b>=block1.y2-20)//�ҳ���ש������������
					{
						chickenbody[i].a=chickenbody[i].a-1;//�����屻ש�����ߣ������ƶ����ٶ���ש����ͬ
					}
				}
			}
			
			//ĳ���屻ש�����ߺ����Ϸ����弰ͷ��������
			for (int i=0;i<numMax;i++)//������������
			{
				if(chickenbody[i].a==block1.x2+4*gao-40)//����Ϸ����������ש���Ҳ�ǡ����ȣ��������䣩
				{
					if(chickenbody[i].b<block1.y2-20)//�ҳ�ש�����������
					{
						chickenbody[i].b=chickenbody[i].b+8*gao;//��������
						find2=1;//�ñ�����ֵΪ1����ʾͷ��Ӧ����
					}
				}
			}
			if(find2==1)//���ͷ��Ӧ����
				chickenhead.n=chickenhead.n+8*gao;//ͷ���½���Ӧ�߶�
		}

		else if(finddd==1)//�����ڵ�һ�����͵�ש��
		{
			for (int i=0;i<blockNum;i++)//��������ש��
			{
				block[i].update(); // ����ש���λ�á��ٶ�
			}
			
			//�ж������Ƿ���ש����ײ
			for(int i=0;i<blockNum;i++)//��������ש��
			{

				if(chickenhead.m>=block[i].x-100&&chickenhead.m<=block[i].x-60)//����С��ͷ����ש����ײ��Χ
				{
					if(chickenhead.n<=block[i].y+10&&chickenhead.n>=block[i].y-50)
					{
						crash=1;//С��ͷ��ǰ��ש����ײ����Ϸʧ��
					}
				}
			}
			
			//ĳ���屻ש�����ߺ����Ϸ����弰ͷ��������
			for (int i=0;i<blockNum;i++)//��������ש��
			{
				for(int j=0;j<numMax;j++)//������������,�ҳ���ש������������
				{
					if(chickenbody[j].b>=block[i].y-20&&chickenbody[j].b<=block[i].y+20)//�ҳ���ÿ��ש��������������
					{
						if(chickenbody[j].a+gao==block[i].x)//��������Ҳ��ש�����ĺ�����ǡ����ȣ����ж�������ש����ײ��
						{
							chickenbody[j].a=chickenbody[j].a-1;//�����屻ש�����ߣ������ƶ����ٶ���ש����ͬ
						}
					}
				}
			}
			for (int i=0;i<blockNum;i++)//��������ש��
			{
				for(int j=0;j<numMax;j++)//������������
				{
					if(chickenbody[j].a==block[i].x+gao)//�����ש���뿪ͷ���У�С��ͷ����δ�����ߵ����������У�
					{
						if(chickenbody[j].b<block[i].y)//�ҳ�ש�����������
						{
							if(block[blockNum-1].x<chickenbody[j].a)//�����ש��Ϊÿ��ש�����һ��
							{
								chickenbody[j].b=chickenbody[j].b+gao*pushNum;//��������
								find1=1;//�ñ�����ֵΪ1����ʾͷ��Ӧ����
							}
						}
					}
				}
			}
			if(find1==1)//���ͷ��Ӧ����
				chickenhead.n=chickenhead.n+gao*pushNum;//ͷ���½���Ӧ�߶�
		}
	}
}
void updateWithInput() // �������йصĸ���
{
	MOUSEMSG m;//���������Ϣ
	if(MouseHit())//����������Ϣ
	{
		m=GetMouseMsg();//��������Ϣ
		if(m.uMsg==WM_LBUTTONDOWN)//�������������
		{
				bodynum++;//С����������һ
				if(bodynum>numMax)//������������������ֵ
				{
					bodynum=numMax;//������������
					chickenhead.n=chickenhead.n;//ͷ�����ֲ���
				}
				else
					chickenhead.n=chickenhead.n-gao;//ͷ������
		}
	}
}
int main()
{
	startup();
	startupBlock();

	while(1)// �ظ�����
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