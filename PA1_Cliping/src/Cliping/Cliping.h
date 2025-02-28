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
	bool enter;

	Point(int tx, int ty)
	{
		x = tx;
		y = ty;
		enter = 0;
	}

	double GetDist(Point a)
	{
		int dx = a.x - x;
		int dy = a.y - y;
		return sqrt(dx * dx + dy * dy);
	}
};

struct Intersection
{
	int x;
	int y;
	bool enter;
	double k;
	bool type;
	int num_polygon; //对位
	int num_line;   //对位
	Intersection(int tx, int ty, bool tenter, double tk, bool ttype, int npolygon, int nline)
	{
		x = tx;
		y = ty;
		enter = tenter;
		k = tk;
		type = ttype;
		num_polygon = npolygon;
		num_line = nline;
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
	int OutSource = -1;
	vector<list<Point>> WindowPoints;
	int OutWindow = -1;
	list<Point> BufferPoints;
	//1:默认待裁剪多边形，0：窗口多边形
	bool mode = 1;
	vector<vector<vector<Intersection>>> list_source;
	vector<vector<vector<bool>>> visit;
	vector<vector<vector<Intersection>>> list_window;
	vector<vector<Point>> Result;
	//求交
	Point GetIntersect(Point p_start, Point p_end, Point q_start, Point q_end);
	//求交点k
	double GetK(Point start, Point end, Point middle);
	//求所有交点
	void GetIntersections(Point start_a, Point end_a, int npolygon, int nline, vector<list<Point>>& Window,
		vector<vector<vector<Intersection>>>& list_a, vector<vector<vector<Intersection>>>& list_b);	//闭合图形
	//排序交点
	void SortIntersections(vector<Intersection>& t_list);
	//找对应点位置
	void FindRelatedPlace(bool type, Intersection p, int& npolygon, int& nline, int& npoint);
	//找开始位置
	void FindNextPlace(int& npolygon, int& nline, int& npoint, bool& pmode,
		int cpolygon, int cline, int cpoint, bool cmode, int current);
	void CloseLink();
	//判断顺时针
	bool IsClockWise(list<Point> PointLink);
	//判断点在多边形内
	bool JudgeInside(Point p, list<Point> Polygon);
	//画线
	void paintEvent(QPaintEvent *event);
	//特判没有交点的图像
	void JudgeSpecial();
	//判断有没有交点
	bool Cliping::HasNoIntersection(bool mode, int num);
	//判断是否符合特判条件
	bool Cliping::JudgeAdd(bool mode, int num);
	//切换模式
private slots:
	void Cliping::SwitchToSource();
	void Cliping::SwitchToWindow();
	//主函数
	void Cliping::Weiler_Atherton();
private:
    Ui::ClipingClass ui;
};
