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
	float LargeRadius = 0;
	float SmallRadius = 0;
	float DownHeight = 0;
	float UpHeight = 0;
	Point BasePlace;

	//具体显示的点
	Point BottomPoints[4];
	Point MiddlePoints[4];
	Point UpPoints[4];
	Point CircleCenter;
	float CircleRadius;

	//包围盒
	Point BoundingBoxDown;
	Point BoundingBoxUp;

	//材质，纹理，颜色信息
	GLfloat Color[3] = { 0, 0, 0 }; //颜色
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //环境光
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //漫反射
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //镜面反射
	GLfloat Shininess[4] = { 0 }; //镜面指数

	//初始化位置信息
	void InitPlace(float large_size, float small_size, float down_height, float up_height, float x, float y, float z)
	{
		LargeRadius = large_size;
		SmallRadius = small_size;
		DownHeight = down_height;
		UpHeight = up_height;
		BasePlace.SetPlace(x, y, z);

		BottomPoints[0].SetPlace(x - LargeRadius, y, z - LargeRadius);
		BottomPoints[1].SetPlace(x - LargeRadius, y, z + LargeRadius);
		BottomPoints[2].SetPlace(x + LargeRadius, y, z + LargeRadius);
		BottomPoints[3].SetPlace(x + LargeRadius, y, z - LargeRadius);

		MiddlePoints[0].SetPlace(x - SmallRadius, y + DownHeight, z - SmallRadius);
		MiddlePoints[1].SetPlace(x - SmallRadius, y + DownHeight, z + SmallRadius);
		MiddlePoints[2].SetPlace(x + SmallRadius, y + DownHeight, z + SmallRadius);
		MiddlePoints[3].SetPlace(x + SmallRadius, y + DownHeight, z - SmallRadius);

		UpPoints[0].SetPlace(x - SmallRadius, y + DownHeight + UpHeight, z - SmallRadius);
		UpPoints[1].SetPlace(x - SmallRadius, y + DownHeight + UpHeight, z + SmallRadius);
		UpPoints[2].SetPlace(x + SmallRadius, y + DownHeight + UpHeight, z + SmallRadius);
		UpPoints[3].SetPlace(x + SmallRadius, y + DownHeight + UpHeight, z - SmallRadius);

		CircleCenter.SetPlace(x, y + DownHeight + UpHeight + SmallRadius, z);
		CircleRadius = SmallRadius;

		BoundingBoxDown.SetPlace(x - LargeRadius, y, z - LargeRadius);
		BoundingBoxUp.SetPlace(x + LargeRadius, y + DownHeight + UpHeight + 2 * SmallRadius, z + LargeRadius);
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
