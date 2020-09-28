#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Cliping.h"
#include <QMouseEvent>
#include <vector>
#include <list>
using namespace std;

#define SOURCE 1
#define WINDOW 0

struct Point
{
	int x;
	int y;
	Point(int tx, int ty)
	{
		x = tx;
		y = ty;
	}

	double GetDist(Point a)
	{
		int dx = a.x - x;
		int dy = a.y - y;
		return sqrt(dx * dx + dy * dy);
	}
};

class Cliping : public QMainWindow
{
    Q_OBJECT

public:
    Cliping(QWidget *parent = Q_NULLPTR);

protected:	//mouse
	void mousePressEvent(QMouseEvent *event);        //����
	vector<list<Point>> SourcePoints;
	vector<list<Point>> WindowPoints;
	list<Point> BufferPoints;
	
	//1:Ĭ�ϴ��ü�����Σ�0�����ڶ����
	bool mode = 1;

	//�պ�ͼ��
	void CloseLink();
	//�ж�˳ʱ��
	bool IsClockWise(list<Point> PointLink);
	//�жϵ��ڶ������
	bool JudgeInside(Point p, list<Point> Polygon);
	//����
	void paintEvent(QPaintEvent *event);
	//�л�ģʽ

private slots:
	void Cliping::SwitchToSource();
	void Cliping::SwitchToWindow();

private:
    Ui::ClipingClass ui;
};
