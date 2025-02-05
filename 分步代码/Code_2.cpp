// ÿ���������һ������
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include "EasyXPng.h"
#define screenWidth 400
#define screenHeight 800
#define numMax 300
int gao=40;//gaoΪש�������ͼƬ�ı߳�

int bodynum=0;//�������
class Block//����ש����
{
public:
	IMAGE im_block;//ש��ͼ��
	int x,y;//ש���������
	int v;//ש���ٶ�

	void draw()//��ʾש��	
	{
		putimagePng(x,y,&im_block);
	}
	void update()
	{
		x-=v;//ש�������ƶ�
	}
};

class Chickenhead//����ͷ����
{
public:
	IMAGE im_chickenHead;//ͷ��ͼ��
	int m,n;//����ͷ������

	void draw()//��ʾͷ�������Ϣ	
	{
		putimagePng(m,n,&im_chickenHead);
	}
};

class Chickenbody//����������
{
public:
	IMAGE im_chickenbody;
	int a,b;

	void draw()//��ʾ���������Ϣ	
	{
		if(bodynum>0)//�������������ʱ
		{
			for(int i=0;i<bodynum;i++)//��ʾ��Ӧ����������
			{
				putimagePng(a,b+i*gao,&im_chickenbody);
			}
		}
	}
};

IMAGE im_block,im_chickenHead,im_chickenbody;//����ͼ�����	
Block block[numMax];//����ש�����
Chickenhead chickenhead;//����ͷ������
Chickenbody chickenbody;//�����������

void startup()//��ʼ��
{
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);
	loadimage(&im_block,_T("block.png"),gao,gao);
	loadimage(&im_chickenbody,_T("body.png"),gao,gao);
	initgraph(screenWidth,screenHeight); // �¿�һ������

	//��ʼ������
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

	BeginBatchDraw();//��ʼ��������
}

void show()//���ƺ���
{
	setbkcolor(RGB(189,247,255));
	cleardevice();//��ʾ����

	chickenhead.draw();
	chickenbody.draw();
	for (int i=0;i<numMax;i++)
	{
		block[i].draw();//��ʾש��
	}
	FlushBatchDraw();//��������
}


void updateWithoutInput()//�������޹صĸ���
{
	for (int i=0;i<numMax;i++)
	{
		block[i].update();//����ש���λ�á��ٶ�
	}
}

void updateWithInput()//�������йصĸ���
{
	MOUSEMSG m;//���������Ϣ
	if(MouseHit())
	{
		m=GetMouseMsg();//��������Ϣ
		if(m.uMsg==WM_LBUTTONDOWN)//�������������
		{
			chickenbody.b=chickenhead.n+gao;//���������������
			chickenhead.n=chickenhead.n-gao;//ͷ�������ƶ�һ������ľ���
			bodynum++;
		}
	}
}

int main()
{
	startup();
	while(1)// �ظ�����
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