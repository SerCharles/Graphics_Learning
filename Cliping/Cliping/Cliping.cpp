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
	// ���������������
	if (event->button() == Qt::LeftButton) 
	{
		BufferPoints.push_back(p);
		update();
	}
	// ���������Ҽ�����
	else if (event->button() == Qt::RightButton) 
	{
		if (BufferPoints.size() >= 3)
		{
			CloseLink();
			update();
		}
	}

}

//�ж��Ƿ�˳ʱ��
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

	//���һ����
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

//�պ�ͼ�Σ��洢����
void Cliping::CloseLink()
{
	//���´洢����
	if (mode == SOURCE)
	{
		//���⻷���ڻ�
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
		//�⻷������ʱ�룬�ڻ�˳ʱ��
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
		//���⻷���ڻ�
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
		//�⻷������ʱ�룬�ڻ�˳ʱ��
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

//����
void Cliping::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	//���ü�����
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
	//���ڣ���
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

	//���ڻ���ͼ��
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

	//���ƽ���TODO
	paint.setPen(Qt::black);

}

//�л�ģʽ
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