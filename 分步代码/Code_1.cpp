//ש���ƶ���С������
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include "EasyXPng.h"
#define screenWidth 400//���廭����
#define screenHeight 800//���廭��߶�
#define numMax 300//�����������

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

class Chicken //���������
{
public:
	IMAGE im_chickenHead;//�������ͼ��
	int x,y;//�����������

	void draw() // ��ʾ���ͼ��	
	{
		putimagePng(screenWidth/4,screenHeight*2/3,&im_chickenHead);
	}
};
IMAGE im_block,im_chickenHead;// ����ͼ�����	
Block block[numMax];//����ש�����
Chicken chicken[numMax];

void startup()//��ʼ��
{
	IMAGE im_chickenHead;
	loadimage(&im_chickenHead,_T("chicken.png"),156,150);//��ȡͼƬ��img������
	loadimage(&im_block,_T("block.png"),40,40);//��ȡͼƬ��img������
	initgraph(screenWidth,screenHeight); // �¿�һ������
	
	for (int i=0;i<numMax;i++)//�趨ש��������Ϣ�ĳ�ʼֵ
	{
		block[i].im_block=im_block;
		block[i].x=screenWidth;
		block[i].y=screenHeight/2;
		block[i].v=2;
		chicken[i].im_chickenHead=im_chickenHead;
		chicken[i].x=screenWidth/4;
		chicken[i].y=screenHeight*2/3;
	}
	
	BeginBatchDraw();// ��ʼ��������
}
void show()// ���ƺ���
{
	setbkcolor(RGB(189,247,255));//���ñ�����ɫ
	cleardevice();// ��ʾ����	

	for (int i=0;i<numMax;i++)
	{
		block[i].draw();//��ʾש��
		chicken[i].draw();//��ʾ���
	}

	FlushBatchDraw();// ��������
	Sleep(10);//�����ٶ�
}

void updateWithoutInput() // �������޹صĸ���
{
	for (int i=0;i<numMax;i++)
	{
		block[i].update(); //����ש���λ�á��ٶ�
	}
}

int main()//������
{
	startup();
	while(1)// �ظ�����
	{
		show();
		updateWithoutInput(); 
	}
	_getch();
	return 0;

	EndBatchDraw();
}