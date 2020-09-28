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