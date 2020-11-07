/*
描述：边界类
日期：2020/11/6
*/
#pragma once
#include<math.h>
#include"Point.hpp"
using namespace std;

//边界格子类
class RectangleMesh
{
public:
	//位置信息
	float SizeX = 0, SizeZ = 0;
	Point PointList[4];
	//材质，纹理，颜色信息
	GLfloat Color[3] = { 0, 0, 0 }; //颜色
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //环境光
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //漫反射
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //镜面反射
	GLfloat Shininess[1] = { 0 }; //镜面指数
	RectangleMesh() {}
	void InitPlace(float start_x, float start_z, float size_x, float size_z, float y)
	{
		PointList[0].SetPlace(start_x, y, start_z);
		PointList[1].SetPlace(start_x + size_x, y, start_z);
		PointList[2].SetPlace(start_x + size_x, y, start_z + size_z);
		PointList[3].SetPlace(start_x, y, start_z + size_z);
	}

	void InitColor(GLfloat color[], GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess)
	{
		for (int i = 0; i < 3; i++)
		{
			Color[i] = color[i];
			Ambient[i] = ambient[i];
			Diffuse[i] = diffuse[i];
			Specular[i] = specular[i];
		}
		//透明度：1
		Ambient[3] = 1.0;
		Diffuse[3] = 1.0;
		Specular[3] = 1.0;
		Shininess[0] = shininess;
	}
};


//边界类
class Board
{
public:
	//位置信息
	float XHigh = 0, XLow = 0, ZHigh = 0, ZLow = 0, Y = 0;
	const int XNum = 200;
	const int ZNum = 200;
	float XSize = 0, ZSize = 0;
	RectangleMesh RectangleList[200][200];

	//材质，纹理，颜色信息
	GLfloat Color[3] = { 0, 0, 0 }; //颜色
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //环境光
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //漫反射
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //镜面反射
	GLfloat Shininess[1] = { 0 }; //镜面指数


	Board(){}
	void InitPlace(float xhigh, float xlow, float zhigh, float zlow, float y)
	{
		XHigh = xhigh;
		XLow = xlow;
		ZHigh = zhigh;
		ZLow = zlow;
		Y = y;
		XSize = (xhigh - xlow) / XNum;
		ZSize = (zhigh - zlow) / ZNum;
		for (int i = 0; i < XNum; i++)
		{
			for (int j = 0; j < ZNum; j++)
			{
				RectangleList[i][j].InitPlace(XLow + XSize * i, ZLow + ZSize * j, XSize, ZSize, Y);
			}
		}

	}

	void InitColor(GLfloat color[], GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess)
	{
		for (int i = 0; i < 3; i++)
		{
			Color[i] = color[i];
			Ambient[i] = ambient[i];
			Diffuse[i] = diffuse[i];
			Specular[i] = specular[i];
		}
		//透明度：1
		Ambient[3] = 1.0;
		Diffuse[3] = 1.0;
		Specular[3] = 1.0;
		Shininess[0] = shininess;
		for (int i = 0; i < XNum; i++)
		{
			for (int j = 0; j < ZNum; j++)
			{
				RectangleList[i][j].InitColor(color, ambient, diffuse, specular, shininess);
			}
		}
	}



	//绘制自身
	void Draw()
	{
		for (int i = 0; i < XNum; i++)
		{
			for (int j = 0; j < ZNum; j++)
			{
				RectangleMesh mesh = RectangleList[i][j];
				glColor3f(mesh.Color[0], mesh.Color[1], mesh.Color[2]);
				glMaterialfv(GL_FRONT, GL_AMBIENT, mesh.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mesh.Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, mesh.Specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, mesh.Shininess);
				DrawRectangle(mesh.PointList[0], mesh.PointList[1], mesh.PointList[2], mesh.PointList[3]);
			}
		}
		glFlush();
	}

	/*
	描述：获取交点所在的网格坐标
	参数：交点坐标
	返回：网格坐标i，j；如果不在边界内，返回-1
	*/
	void GetIntersectionID(Point intersection, int& i, int& j)
	{
		i = int((intersection.x - XLow) / XSize);
		j = int((intersection.z - ZLow) / ZSize);
		if (i < 0 || i >= XNum)
		{
			i = -1;
			j = -1;
			return;
		}
		if (j < 0 || j >= ZNum)
		{
			i = -1;
			j = -1;
			return;
		}
	}
};