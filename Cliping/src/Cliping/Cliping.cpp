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
	current = PointLink.begin();
	sum += (current->x - previous->x) * (current->y + previous->y);
	return sum < 0;
}

bool Cliping::JudgeInside(Point p, list<Point> Polygon)
{
	double sum = 0;
	auto current = Polygon.begin();
	auto previous = current;
	current++;
	while (current != Polygon.end())
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
	current = Polygon.begin();
	{
		Point q(current->x, current->y);
		Point r(previous->x, previous->y);
		double a = p.GetDist(q);
		double b = p.GetDist(r);
		double c = q.GetDist(r);
		double angle = acos((a * a + b * b - c * c) / (2 * a * b));
		sum += angle;
	}
	if (sum - 2 * 3.1415926535 < - 0.01) return 0;
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
	BufferPoints.clear();
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
	for (int i = 0; i < Result.size(); i++)
	{
		if (Result[i].size() > 2)
		{
			int current = 1;
			int previous = 0;
			while (current < Result[i].size())
			{
				paint.drawLine(Result[i][current].x, Result[i][current].y, Result[i][previous].x, Result[i][previous].y);
				current++;
				previous++;
			}
			current = 0;
			paint.drawLine(Result[i][current].x, Result[i][current].y, Result[i][previous].x, Result[i][previous].y);
		}
	}
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
	int a_p = p_end.y - p_start.y;
	int b_p = p_start.x - p_end.x;
	int c_p = a_p * p_start.x + b_p * p_start.y;

	int a_q = q_end.y - q_start.y;
	int b_q = q_start.x - q_end.x;
	int c_q = a_q * q_start.x + b_q * q_start.y;

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
	if (det > 0) the_p.enter = 1;

	return the_p;

}

double Cliping::GetK(Point start, Point end, Point middle)
{

	double t = sqrt((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y));
	double d = sqrt((middle.x - start.x) * (middle.x - start.x) + (middle.y - start.y) * (middle.y - start.y));
	return d / t;
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
	if (t_list.size() <= 1) return;
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

void Cliping::FindRelatedPlace(bool type, Intersection p, int& npolygon, int& nline, int& npoint)
{
	nline = p.num_line;
	npolygon = p.num_polygon;
	//在window找
	if (type == SOURCE)
	{
		for (int i = 0; i < list_window[npolygon][nline].size(); i++)
		{
			if (list_window[npolygon][nline][i].x == p.x && list_window[npolygon][nline][i].y == p.y)
			{
				npoint = i;
				return;
			}
		}
	}
	else
	{
		for (int i = 0; i < list_source[npolygon][nline].size(); i++)
		{
			if (list_source[npolygon][nline][i].x == p.x && list_source[npolygon][nline][i].y == p.y)
			{
				npoint = i;
				return;
			}
		}
	}
}

void Cliping::FindNextPlace(int& npolygon, int& nline, int& npoint, bool& pmode,
	int cpolygon, int cline, int cpoint, bool cmode, int current)
{
	//cpolygon cline cpoint-1：初始化
	if (cpolygon == -1 || cline == -1 || cpoint == -1)
	{
		for (int i = 0; i < visit.size(); i++)
		{
			for (int j = 0; j < visit[i].size(); j++)
			{
				for (int k = 0; k < visit[i][j].size(); k++)
				{
					if (visit[i][j][k] == 0)
					{
						npolygon = i;
						nline = j;
						npoint = k;
						pmode = 1;
						Intersection the_point = list_source[i][j][k];
						if (!the_point.enter)
						{
							//切换
							pmode = WINDOW;
							FindRelatedPlace(SOURCE, the_point, npolygon, nline, npoint);
							return;
						}
						return;
					}
				}
			}
		}
		npolygon = -1;
		nline = -1;
		npoint = -1;
		return;
	}
	//否则：找下一个点
	else
	{
		int length = 0;
		list<Point> tlist;
		if (cmode == SOURCE)
		{
			length = list_source[cpolygon][cline].size();
			while (cpoint + 1 >= length)
			{
				//找下一个顶点
				cline++;
				if (cline >= SourcePoints[cpolygon].size()) cline = 0;
				cpoint = -1;
				length = list_source[cpolygon][cline].size();

				auto t_iterator = SourcePoints[cpolygon].begin();
				for (int i = 0; i < cline; i++)
				{
					t_iterator++;
				}
				Point the_vertex = *t_iterator;
				Result[current].push_back(the_vertex);
			}
			cpoint++;
			Intersection the_point = list_source[cpolygon][cline][cpoint];
			if (!the_point.enter)
			{
				//切换
				pmode = WINDOW;
				FindRelatedPlace(SOURCE, the_point, npolygon, nline, npoint);
				return;
			}
			else
			{
				pmode = SOURCE;
				npolygon = cpolygon;
				nline = cline;
				npoint = cpoint;
				return;
			}
		}
		else
		{
			length = list_window[cpolygon][cline].size();
			while (cpoint + 1 >= length)
			{
				//找下一个顶点
				cline++;
				if (cline >= WindowPoints[cpolygon].size()) cline = 0;
				cpoint = -1;
				length = list_window[cpolygon][cline].size();

				auto t_iterator = WindowPoints[cpolygon].begin();
				for (int i = 0; i < cline; i++)
				{
					t_iterator++;
				}
				Point the_vertex = *t_iterator;
				Result[current].push_back(the_vertex);
			}
			cpoint++;
			Intersection the_point = list_window[cpolygon][cline][cpoint];
			if (the_point.enter)
			{
				//切换
				pmode = SOURCE;
				FindRelatedPlace(WINDOW, the_point, npolygon, nline, npoint);
				return;
			}
			else
			{
				pmode = WINDOW;
				npolygon = cpolygon;
				nline = cline;
				npoint = cpoint;
				return;
			}
		}


	}


		
}

void Cliping::Weiler_Atherton()
{

	list_source.clear();
	visit.clear();
	for (int i = 0; i < SourcePoints.size(); i++)
	{
		vector<vector<Intersection>> empty_one;
		vector<vector<bool>> empty_boolean;
		empty_one.clear();
		empty_boolean.clear();
		list_source.push_back(empty_one);
		visit.push_back(empty_boolean);
		for (int j = 0; j < SourcePoints[i].size(); j++)
		{
			vector<Intersection> new_empty;
			vector<bool> new_bool;
			new_empty.clear();
			new_bool.clear();
			list_source[i].push_back(new_empty);
			visit[i].push_back(new_bool);
		}
	}
	list_window.clear();
	for (int i = 0; i < WindowPoints.size(); i++)
	{
		vector<vector<Intersection>> empty_one;
		empty_one.clear();
		list_window.push_back(empty_one);
		for (int j = 0; j < WindowPoints[i].size(); j++)
		{
			vector<Intersection> new_empty;
			new_empty.clear();
			list_window[i].push_back(new_empty);
		}
	}


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
		current = source_list.begin();
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
			for (int k = 0; k < list_source[i][j].size(); k++)
			{
				visit[i][j].push_back(0);
			}
		}
	}
	for (int i = 0; i < list_window.size(); i++)
	{
		for (int j = 0; j < list_window[i].size(); j++)
		{
			SortIntersections(list_window[i][j]);
		}
	}
	
	//正式
	Result.clear();
	vector<Point> empty_kebab; 
	empty_kebab.clear();
	Result.push_back(empty_kebab);
	int current = 0;
	int p_polygon = -1;
	int p_line = -1;
	int p_point = -1;
	bool p_mode = SOURCE;
	while(1)
	{
		int n_polygon = -1;
		int n_line = -1;
		int n_point = -1;
		bool n_mode = 0;
		int v_polygon = -1;
		int v_line = -1;
		int v_point = -1;
		FindNextPlace(n_polygon, n_line, n_point, n_mode, p_polygon, p_line, p_point, p_mode, current);
		if (n_polygon == -1 || n_line == -1 || n_point == -1)
		{
			//没找到空的，gg
			break;
		}
		if (n_mode == WINDOW)
		{
			//去source找
			FindRelatedPlace(WINDOW, list_window[n_polygon][n_line][n_point], v_polygon, v_line, v_point);
		}
		else
		{
			v_polygon = n_polygon; v_line = n_line; v_point = n_point;
		}
		if (visit[v_polygon][v_line][v_point] == 1)
		{
			//重复，找新的
			vector<Point> empty_k;
			empty_k.clear();
			Result.push_back(empty_k);
			current++;
			p_polygon = -1;
			p_line = -1;
			p_point = -1;
			p_mode = SOURCE;
		}
		else
		{
			Point nova = Point(list_source[v_polygon][v_line][v_point].x, list_source[v_polygon][v_line][v_point].y);
			Result[current].push_back(nova);
			visit[v_polygon][v_line][v_point] = 1;
			p_polygon = n_polygon;
			p_line = n_line;
			p_point = n_point;
			p_mode = n_mode;
		}
	}
	JudgeSpecial();
	repaint();
}

bool Cliping::HasNoIntersection(bool mode, int num)
{
	if (mode == SOURCE)
	{
		for (int i = 0; i < list_source[num].size(); i++)
		{
			if (list_source[num][i].size() != 0) return 0;
		}
	}
	else
	{
		for (int i = 0; i < list_window[num].size(); i++)
		{
			if (list_window[num][i].size() != 0) return 0;
		}
	}
	return 1;
}

bool Cliping::JudgeAdd(bool mode, int num)
{
	if (mode == SOURCE)
	{
		for (int i = 0; i < WindowPoints.size(); i++)
		{
			auto current = SourcePoints[num].begin();
			while (current != SourcePoints[num].end())
			{
				Point c_point = Point(current->x, current->y);
				if (i != OutWindow)
				{
					//不在任何内环里
					if (JudgeInside(c_point, WindowPoints[i])) return 0;
				}
				else
				{
					//在外环里
					if (!JudgeInside(c_point, WindowPoints[i])) return 0;
				}
				current++;
			}
		}
	}
	else
	{
		for (int i = 0; i < SourcePoints.size(); i++)
		{
			auto current = WindowPoints[num].begin();
			while (current != WindowPoints[num].end())
			{
				Point c_point = Point(current->x, current->y);
				if (i != OutSource)
				{
					//不在任何内环里
					if (JudgeInside(c_point, SourcePoints[i])) return 0;
				}
				else
				{
					//在外环里
					if (!JudgeInside(c_point, SourcePoints[i])) return 0;
				}
				current++;
			}
		}
	}
	return 1;
}

void Cliping::JudgeSpecial()
{
//特判：没交点情况
//1.目标外环没交点：外环在窗口最外层里面，而且不在任何窗口内环内，就标上
//2.目标内环没交点：内环在窗口最外层里面，而且不在任何窗口内环内，就标上
//3.窗口外环没交点：在目标外环内，而且不在任何目标内环内，就标上
//4.窗口内环没交点：在目标外环内，而且不在任何目标内环内，就标上
	for (int i = 0; i < SourcePoints.size(); i++)
	{
		if (HasNoIntersection(SOURCE, i))
		{
			if (JudgeAdd(SOURCE, i))
			{
				vector<Point> nova_list;
				nova_list.clear();
				Result.push_back(nova_list);
				auto current = SourcePoints[i].begin();
				while (current != SourcePoints[i].end())
				{
					Point c_point = Point(current->x, current->y);
					Result[Result.size() - 1].push_back(c_point);
					current++;
				}
			}
		}
	}

	for (int i = 0; i < WindowPoints.size(); i++)
	{
		if (HasNoIntersection(WINDOW, i))
		{
			if (JudgeAdd(WINDOW, i))
			{
				vector<Point> nova_list;
				nova_list.clear();
				Result.push_back(nova_list);
				auto current = WindowPoints[i].begin();
				while (current != WindowPoints[i].end())
				{
					Point c_point = Point(current->x, current->y);
					Result[Result.size() - 1].push_back(c_point);
					current++;
				}
			}
		}
	}
}
