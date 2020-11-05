#pragma once
#include<cstdio>
#include<cstdlib>
#include<GL/glut.h>
#include"Point.hpp"
#define _CRT_SECURE_NO_WARNINGS

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量


class Static
{
public:
	float Size = 0;
	Point BasePlace;

	//具体显示的点
	Point BottomPoints[4];
	Point UpPoints[4];


	//材质，纹理，颜色信息
	GLfloat Color[3] = { 0, 0, 0 }; //颜色
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //环境光
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //漫反射
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //镜面反射
	GLfloat Shininess[4] = { 0 }; //镜面指数

	//初始化位置信息
	void InitPlace(float size, float x, float y, float z)
	{
		Size = size;
		BasePlace.SetPlace(x, y, z);

		BottomPoints[0].SetPlace(x, y, z);
		BottomPoints[1].SetPlace(x + size, y, z);
		BottomPoints[2].SetPlace(x + size, y, z + size);
		BottomPoints[3].SetPlace(x, y, z + size);

		UpPoints[0].SetPlace(x, y + size, z);
		UpPoints[1].SetPlace(x + size, y + size, z);
		UpPoints[2].SetPlace(x + size, y + size, z + size);
		UpPoints[3].SetPlace(x, y + size, z + size);
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

};
