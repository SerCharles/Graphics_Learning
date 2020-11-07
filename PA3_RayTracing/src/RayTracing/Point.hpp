/*
描述：3D点类和各种常用函数
日期：2020/11/6
*/
#pragma once


#include<math.h>
#include<GL/glut.h>
using namespace std;

#define PI 3.1415926

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