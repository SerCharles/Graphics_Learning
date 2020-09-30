#include "Cliping.h"
#include <QMouseEvent>
#include <cmath>
#include <QPainter> 
#include <qpushbutton.h>


Cliping::Cliping(QWidget *parent)
    : QMainWindow(parent)
{
	mode = SOURCE;
	SourcePoints.clear();
	WindowPoints.clear();
	BufferPoints.clear();
    ui.setupUi(this);
	connect(ui.SourceButton, &QPushButton::clicked, this, &Cliping::SwitchToSource);
	connect(ui.WindowButton, &QPushButton::clicked, this, &Cliping::SwitchToWindow);
	connect(ui.SaveButton, &QPushButton::clicked, this, &Cliping::Weiler_Atherton);
}

void Cliping::mousePressEvent(QMouseEvent *event)
{
	QPoint p_re = event->pos();
	int x = p_re.x();
	int y = p_re.y();
	Point p(x, y);
	// 如果是鼠标左键按下
	if (event->button() == Qt::LeftButton) 
	{
		BufferPoints.push_back(p);
		update();
	}
	// 如果是鼠标右键按下
	else if (event->button() == Qt::RightButton) 
	{
		if (BufferPoints.size() >= 3)
		{
			CloseLink();
			update();
		}
	}

}

//判断是否顺时针
bool Cliping::IsClockWise(list<Point> PointLink)
{
	int sum = 0;
	auto current = PointLink.begin();
	auto previous = current;
	current++;
	while (current != PointLink.end())
	{
		sum += (current->x - previous->x) * (current->y + previous->y);
		current++;
		previous++;
	}
	return sum > 0;
}

bool Cliping::JudgeInside(Point p, list<Point> Polygon)
{
	int sum = 0;
	auto current = BufferPoints.begin();
	auto previous = current;
	current++;
	while (current != BufferPoints.end())
	{
		Point q(current->x, current->y);
		Point r(previous->x, previous->y);
		double a = p.GetDist(q);
		double b = p.GetDist(r);
		double c = q.GetDist(r);
		double angle = acos((a * a + b * b - c * c) / (2 * a * b));
		sum += angle;
		current++;
		previous++;
	}

	//最后一个角
	double last_angle = 0;
	current = BufferPoints.begin();
	{
		Point q(current->x, current->y);
		Point r(previous->x, previous->y);
		double a = p.GetDist(q);
		double b = p.GetDist(r);
		double c = q.GetDist(r);
		double angle = acos((a * a + b * b - c * c) / (2 * a * b));
		last_angle = angle;
	}
	if (abs(last_angle - sum) < 0.001) return 0;
	return 1;
}

//闭合图形，存储链表
void Cliping::CloseLink()
{
	//更新存储链表
	if (mode == SOURCE)
	{
		//找外环和内环
		Point start = BufferPoints.front();
		int out = -1;
		for (int i = 0; i < SourcePoints.size(); i++)
		{
			bool inside = JudgeInside(start, SourcePoints[i]);
			if (inside)
			{
				out = i;
				break;
			}
		}
		SourcePoints.push_back(BufferPoints);
		if (out == -1) out = SourcePoints.size() - 1;
		OutSource = out;
		//外环按照逆时针，内环顺时针
		for (int i = 0; i < SourcePoints.size(); i++)
		{
			if (i == out)
			{
				if (IsClockWise(SourcePoints[i]))
				{
					SourcePoints[i].reverse();
				}
			}
			else
			{
				if (!IsClockWise(SourcePoints[i]))
				{
					SourcePoints[i].reverse();
				}
			}
		}
		BufferPoints.clear();
	}

	else
	{
		//找外环和内环
		Point start = BufferPoints.front();
		int out = -1;
		for (int i = 0; i < WindowPoints.size(); i++)
		{
			bool inside = JudgeInside(start, WindowPoints[i]);
			if (inside)
			{
				out = i;
				break;
			}
		}
		WindowPoints.push_back(BufferPoints);
		if (out == -1) out = WindowPoints.size() - 1;
		OutWindow = out;

		//外环按照逆时针，内环顺时针
		for (int i = 0; i < WindowPoints.size(); i++)
		{
			if (i == out)
			{
				if (IsClockWise(WindowPoints[i]))
				{
					WindowPoints[i].reverse();
				}
			}
			else
			{
				if (!IsClockWise(WindowPoints[i]))
				{
					WindowPoints[i].reverse();
				}
			}
		}

	}

}

//画线
void Cliping::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	//待裁剪：红
	paint.setPen(Qt::red);
	for (int i = 0; i < SourcePoints.size(); i++)
	{
		auto current = SourcePoints[i].begin();
		auto previous = current;
		current++;
		while (current != SourcePoints[i].end())
		{
			paint.drawLine(current->x, current->y, previous->x, previous->y);
			current++;
			previous++;
		}
		current = SourcePoints[i].begin();;
		paint.drawLine(current->x, current->y, previous->x, previous->y);
	}
	//窗口：蓝
	paint.setPen(Qt::blue);
	for (int i = 0; i < WindowPoints.size(); i++)
	{
		auto current = WindowPoints[i].begin();
		auto previous = current;
		current++;
		while (current != WindowPoints[i].end())
		{
			paint.drawLine(current->x, current->y, previous->x, previous->y);
			current++;
			previous++;
		}
		current = WindowPoints[i].begin();;
		paint.drawLine(current->x, current->y, previous->x, previous->y);
	}

	//正在画的图形
	if(mode == SOURCE) 	paint.setPen(Qt::red);
	else paint.setPen(Qt::blue);
	if (BufferPoints.size() >= 2)
	{
		auto current = BufferPoints.begin();
		auto previous = current;
		current++;
		while (current != BufferPoints.end())
		{
			paint.drawLine(current->x, current->y, previous->x, previous->y);
			current++;
			previous++;
		}
	}

	//绘制交点TODO
	paint.setPen(Qt::black);

}

//切换模式
void Cliping::SwitchToSource()
{
	mode = SOURCE;
	BufferPoints.clear();
}
void Cliping::SwitchToWindow()
{
	mode = WINDOW;
	BufferPoints.clear();
}

Point Cliping::GetIntersect(Point p_start, Point p_end, Point q_start, Point q_end)
{
	int a_p = p_end.x - p_start.x;
	int b_p = p_end.y - p_start.y;
	int c_p = 0 - a_p * p_start.x - b_p * p_start.y;

	int a_q = q_end.x - q_start.x;
	int b_q = q_end.y - q_start.y;
	int c_q = 0 - a_q * q_start.x - b_q * q_start.y;

	int det = a_p * b_q - a_q * b_p;
	if (det == 0)
	{
		return Point(-1, -1);
	}

	int x = (b_q * c_p - b_p * c_q) / det;
	int y = (a_p * c_q - a_q * c_p) / det;

	if(x > max(p_end.x, p_start.x) || x < min(p_end.x, p_start.x) || x > max(q_end.x, q_start.x) || x < min(q_end.x, q_start.x)) return Point(-1, -1);
	if(y > max(p_end.y, p_start.y) || y < min(p_end.y, p_start.y) || y > max(q_end.y, q_start.y) || y < min(q_end.y, q_start.y)) return Point(-1, -1);

	Point the_p(x, y);
	if (det < 0) the_p.enter = 1;

	return the_p;

}

double Cliping::GetK(Point start, Point end, Point middle)
{
	if (end.x != start.x)
	{
		double d = double(middle.x - start.x);
		double t = double(end.x - start.x);
		return d / t;
	}
	else
	{
		double d = double(middle.y - start.y);
		double t = double(end.y - start.y);
		return d / t;
	}
}

void Cliping::GetIntersections(Point start_a, Point end_a, int npolygon, int nline, vector<list<Point>>& Window, 
	vector<vector<vector<Intersection>>>& list_a, vector<vector<vector<Intersection>>>& list_b)
{

	for (int i = 0; i < Window.size(); i++)
	{
		list<Point> window_list = Window[i];
		auto current = window_list.begin();
		auto previous = current;
		int current_num = 0;
		current++;
		while (current != window_list.end())
		{
			Point start_b = Point(previous->x, previous->y);
			Point end_b = Point(current->x, current->y);
			Point intersect = GetIntersect(start_a, end_a, start_b, end_b);
			if (intersect.x >= 0 && intersect.y >= 0)
			{
				double k = GetK(start_a, end_a, intersect);
				double kk = GetK(start_b, end_b, intersect);
				Intersection a = Intersection(intersect.x, intersect.y, intersect.enter, k, SOURCE, i, current_num);
				Intersection b = Intersection(intersect.x, intersect.y, intersect.enter, kk, WINDOW, npolygon, nline);
				list_a[npolygon][nline].push_back(a);
				list_b[i][current_num].push_back(b);
			}
			current++;
			previous++;
			current_num++;
		}
		current = WindowPoints[i].begin();
		{
			Point start_b = Point(previous->x, previous->y);
			Point end_b = Point(current->x, current->y);
			Point intersect = GetIntersect(start_a, end_a, start_b, end_b);
			if (intersect.x >= 0 && intersect.y >= 0)
			{
				double k = GetK(start_a, end_a, intersect);
				double kk = GetK(start_b, end_b, intersect);
				Intersection a = Intersection(intersect.x, intersect.y, intersect.enter, k, SOURCE, i, current_num);
				Intersection b = Intersection(intersect.x, intersect.y, intersect.enter, kk, WINDOW, npolygon, nline);
				list_a[npolygon][nline].push_back(a);
				list_b[i][current_num].push_back(b);
			}
		}
	}
}

void Cliping::SortIntersections(vector<Intersection>& t_list)
{
	for (int i = 0; i < t_list.size() - 1; i++)
	{
		for (int j = i + 1; j < t_list.size(); j++)
		{
			if (t_list[i].k > t_list[j].k)
			{
				Intersection temp = t_list[i];
				t_list[i] = t_list[j];
				t_list[j] = temp;
			}
		}
	}
}

void Cliping::Weiler_Atherton()
{
	vector<vector<vector<Intersection>>> list_source;
	vector<vector<vector<Intersection>>> list_window;
	for (int ii = 0; ii < SourcePoints.size(); ii++)
	{
		list<Point> source_list = SourcePoints[ii];
		vector<list<Point>> window_lists = WindowPoints;
		auto current = source_list.begin();
		auto previous = current;
		current++;
		int current_num = 0;
		while (current != source_list.end())
		{
			Point start = Point(previous->x, previous->y);
			Point end = Point(current->x, current->y);
			GetIntersections(start, end, ii, current_num, window_lists, list_source, list_window);
			

			current++;
			previous++;
			current_num++;
		}
		current = WindowPoints[ii].begin();
		{
			Point start = Point(previous->x, previous->y);
			Point end = Point(current->x, current->y);
			GetIntersections(start, end, ii, current_num, window_lists, list_source, list_window);
		}
	}
	for (int i = 0; i < list_source.size(); i++)
	{
		for (int j = 0; j < list_source[i].size(); j++)
		{
			SortIntersections(list_source[i][j]);
		}
	}
	for (int i = 0; i < list_window.size(); i++)
	{
		for (int j = 0; j < list_window[i].size(); j++)
		{
			SortIntersections(list_window[i][j]);
		}
	}
	int kebab = 0;
}