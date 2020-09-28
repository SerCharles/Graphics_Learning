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
	void mousePressEvent(QMouseEvent *event);        //单击
	vector<list<Point>> SourcePoints;
	vector<list<Point>> WindowPoints;
	list<Point> BufferPoints;
	
	//1:默认待裁剪多边形，0：窗口多边形
	bool mode = 1;

	//闭合图形
	void CloseLink();
	//判断顺时针
	bool IsClockWise(list<Point> PointLink);
	//判断点在多边形内
	bool JudgeInside(Point p, list<Point> Polygon);
	//画线
	void paintEvent(QPaintEvent *event);
	//切换模式

private slots:
	void Cliping::SwitchToSource();
	void Cliping::SwitchToWindow();

private:
    Ui::ClipingClass ui;
};
