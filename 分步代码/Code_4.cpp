//������ɲ�ͬ���͵�ש��
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

//ȫ�ֱ���
int blockNum;//ÿ��ש���ש������
int bodynum=0;
//ö�����ͣ��о�ש�����еĿ���״̬
enum BlockAppearance{n_Lengthwise,n_Broadwise};
	
class Block //����ש����
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

	void draw() // ��ʾͷ�������Ϣ	
	{
		putimagePng(m,n,&im_chickenHead);
	}
};

class Chickenbody //����������
{
public:
	IMAGE im_chickenbody;//����ͼ��
	int a,b;//������������

	void draw()//��ʾ���������Ϣ	
	{
		if(bodynum>0) 
		{
			putimagePng(a,b,&im_chickenbody);
		}
	}
};

IMAGE im_block,im_chickenHead,im_chickenbody;//����ͼ�����	
Block block[numMax];//����ש�����
Chickenhead chickenhead;//����ͷ������
Chickenbody chickenbody[numMax];//�����������


void nLengthwise()//�������ש��
{
	blockNum=rand()%5+1;//�������ÿ��ש������
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth;
		block[i].y=screenHeight/2-9+gao*i;
		block[i].v=1;
	}
}

void nBroadwise()//�������ש��
{
	blockNum=rand()%5+1;//�������ÿ��ש������
	int i;
	for(i=0;i<blockNum;i++)
	{
		block[i].im_block=im_block; 
		block[i].x=screenWidth+gao*i;
		block[i].y=screenHeight/2-9;
		block[i].v=1;
	}
}

void startup()//��ʼ��
{
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);//����ͼƬ
	loadimage(&im_block,_T("block.png"),gao,gao);
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);
	initgraph(screenWidth,screenHeight); // �¿�һ������

	//�����ͷ����������
	chickenhead.im_chickenHead=im_chickenHead; 
	chickenhead.m=screenWidth/4;
	chickenhead.n=screenHeight*2/3;
	for (int i=0;i<numMax;i++)
	{
		chickenbody[i].im_chickenbody=im_chickenbody;
		chickenbody[i].a=chickenhead.m+58;
		chickenbody[i].b=chickenhead.n+gao-gao*i;
		
	}
	BeginBatchDraw();//��ʼ��������
}

void startupBlock()//ש���ʼ��������������ɲ�ͬ���͵�ש��
{
	int k=rand();//�����������
	int type=k%2;//�������0��1����
	if(type==n_Lengthwise)//���type=0
		nLengthwise();//���ɶ������ש��
	else if(type==n_Broadwise)//���type=1
		nBroadwise();//���ɶ������ש��
	
	BeginBatchDraw();// ��ʼ��������
}


void show()// ���ƺ���
{
	setbkcolor(RGB(189,247,255));//���ñ�����ɫ
	cleardevice();//��ʾ����	

	for (int i=0;i<blockNum;i++)
	{
		block[i].draw();// ��ʾש��
		if(block[i].x==-gao*6)
		{
			startupBlock();//��������������͵�ש��
		}
	}

	for (int i=0;i<bodynum;i++)
	{
		chickenbody[i].draw();
	}
	chickenhead.draw();

	
	FlushBatchDraw();// ��������

}

void updateWithoutInput() // �������޹صĸ���
{
	int find1=0;//�ж�ͷ���Ƿ�����ı���
	int pushNum=0;//�����ߵ���������

	//�ж������Ƿ���ש����ײ
	for (int i=0;i<blockNum;i++)//��������ש��
	{
		for(int j=0;j<numMax;j++)//������������,�ҳ���ש������������
		{
			if(chickenbody[j].a+gao==block[i].x)//��������Ҳ��ש�����ĺ�����ǡ����ȣ����ж�������ש����ײ��
			{
				if(chickenbody[j].b>=block[i].y-20&&chickenbody[j].b<=block[i].y+20)//�ҳ���ÿ��ש��������������
				{
					chickenbody[j].a=chickenbody[j].a-1;//�����屻ש�����ߣ������ƶ����ٶ���ש����ͬ
					pushNum++;//�����ߵ�����������һ
				}
			}
		}
	}

	//ĳ���屻ש�����ߺ����Ϸ����弰ͷ��������
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
							chickenbody[j].b=chickenbody[j].b+gao;//��������
							find1=1;//�ñ�����ֵΪ1����ʾͷ��Ӧ����
						}
					}
				}
		}
	}
	if(find1==1)//���ͷ��Ӧ����
	{
		chickenhead.n=chickenhead.n+gao*pushNum;//ͷ���½���Ӧ�߶�
	} 
	for (int i=0;i<numMax;i++)
	{
		block[i].update();//����ש���λ�á��ٶ�
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
			bodynum++;
			if(bodynum>numMax)//������������������ֵ
			{
				bodynum=numMax;
				chickenhead.n=chickenhead.n;//ͷ�����ֲ���
			}
			else
				chickenhead.n=chickenhead.n-40;//ͷ������
		}
	}
}

int main()
{
	srand(time(0));//��ʼ���������
	startup();//��ʼ��
	startupBlock();//��ʼ��ש����Ϣ

	while(1)// �ظ�����
	{
		show();//����ͼ��
		updateWithoutInput();//�������޹صĸ���
		updateWithInput();//�������йصĸ���
		Sleep(1);//��ͣ
	}

	_getch();
	return 0;

	EndBatchDraw();
}