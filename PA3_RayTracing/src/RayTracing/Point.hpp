/*
描述：3D点类和各种常用函数
日期：2020/11/6
*/
#pragma once


#include<math.h>
#include<GL/glut.h>
#include<vector>
using namespace std;

#define PI 3.1415926535

//3D点的基类
class Point
{
public:
	float x;
	float y;
	float z;
	Point()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Point(float tx, float ty, float tz)
	{
		x = tx;
		y = ty;
		z = tz;
	}
	void SetPlace(float tx, float ty, float tz)
	{
		x = tx;
		y = ty;
		z = tz;
	}
	Point operator+(const Point& b)
	{
		Point c;
		c.x = x + b.x;
		c.y = y + b.y;
		c.z = z + b.z;
		return c;
	}
	Point operator-(const Point& b)
	{
		Point c;
		c.x = x - b.x;
		c.y = y - b.y;
		c.z = z - b.z;
		return c;
	}
	Point operator*(const float& b)
	{
		Point c;
		c.x = x * b;
		c.y = y * b;
		c.z = z * b;
		return c;
	}
	float operator*(const Point& b)
	{
		float sum = 0;
		sum += x * b.x;
		sum += y * b.y;
		sum += z * b.z;
		return sum;
	}
	Point operator/(const float& b)
	{
		Point c;
		c.x = x / b;
		c.y = y / b;
		c.z = z / b;
		return c;
	}


	float Square()
	{
		return x * x + y * y + z * z;
	}

	float Dist()
	{
		return sqrt(Square());
	}
};

//绘制一个四边形
void DrawRectangle(Point a, Point b, Point c, Point d)
{
	glBegin(GL_POLYGON);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glVertex3f(d.x, d.y, d.z);
	glEnd();
}

//绘制一个三角形
void DrawTriangle(Point a, Point b, Point c)
{
	glBegin(GL_POLYGON);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glEnd();
}

class Color
{
public:
	float R;
	float G;
	float B;
	Color()
	{
		R = 0;
		G = 0;
		B = 0;
	}
	Color(float r, float g, float b)
	{
		R = r;
		G = g;
		B = b;
	}
};

/*
描述：判断点是否在多边形内，用角度法求点和每条边交点夹角和，为2pi才在内否则在外
参数：点，多边形
返回：是/否
*/
bool JudgeInside(Point& p, vector<Point>& polygon)
{
	float sum_arc = 0;
	for (int i = 0; i < polygon.size(); i++)
	{
		Point current = polygon[i];
		Point next;
		if (i == polygon.size() - 1)
		{
			next = polygon[0];
		}
		else
		{
			next = polygon[i + 1];
		}
		float a = (current - p).Dist();
		float b = (next - p).Dist();
		float c = (current - next).Dist();
		float up = a * a + b * b - c * c;
		float down = 2 * a * b;
		if (down == 0) return 0;
		float arc = acos(up / down);
		sum_arc += arc;
	}
	if (abs(sum_arc - 2 * PI) < 0.01) return 1;
	else return 0;
}