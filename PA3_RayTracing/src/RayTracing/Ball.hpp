#pragma once
#include<math.h>
#include<GL/glut.h>
#include"Point.hpp"
#include"Board.hpp"
#include"Static.hpp"
using namespace std;

#pragma once
class Ball
{
public:
	//位置信息
	Point CurrentPlace;
	float Radius;
	int Complexity;

	//材质，纹理，颜色信息
	GLfloat Color[3] = { 0, 0, 0 }; //颜色
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //环境光
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //漫反射
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //镜面反射
	GLfloat Shininess[4] = { 0 }; //镜面指数
public:
	Ball(){}

	//初始化位置，速度信息
	void InitPlace(float x, float z, float radius, int complexity)
	{
		CurrentPlace.SetPlace(x, radius, z);
		Radius = radius;
		Complexity = complexity;
	}

	//初始化颜色，纹理，材质信息
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

	//求点到球心距离
	float GetDistance(Point p)
	{ 
		return sqrt((p - CurrentPlace) * (p - CurrentPlace));
	}

	//绘制自身
	void Draw()
	{
		//设置纹理，材质等信息
		glMaterialfv(GL_FRONT, GL_AMBIENT, Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Shininess);

		//平移到坐标原点，绘制，恢复坐标
		glPushMatrix();
		glTranslatef(CurrentPlace.x, CurrentPlace.y, CurrentPlace.z);
		glutSolidSphere(Radius, Complexity, Complexity);
		glPopMatrix();
	}
};