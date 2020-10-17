#pragma once
#include<cstdio>
#include<cstdlib>
#include<GL/glut.h>
#include"Point.hpp"
#define _CRT_SECURE_NO_WARNINGS

#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����


class Static
{
public:
	float LargeRadius = 0;
	float SmallRadius = 0;
	float DownHeight = 0;
	float UpHeight = 0;
	Point BasePlace;

	//������ʾ�ĵ�
	Point BottomPoints[4];
	Point MiddlePoints[4];
	Point UpPoints[4];
	Point CircleCenter;
	float CircleRadius;

	//��Χ��
	Point BoundingBoxDown;
	Point BoundingBoxUp;

	//���ʣ�������ɫ��Ϣ
	GLfloat Color[3] = { 0, 0, 0 }; //��ɫ
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //������
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //������
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //���淴��
	GLfloat Shininess[4] = { 0 }; //����ָ��

	//��ʼ��λ����Ϣ
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

	//��ʼ����ɫ������������Ϣ
	void InitColor(GLfloat color[], GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess)
	{
		for (int i = 0; i < 3; i++)
		{
			Color[i] = color[i];
			Ambient[i] = ambient[i];
			Diffuse[i] = diffuse[i];
			Specular[i] = specular[i];
		}
		//͸���ȣ�1
		Ambient[3] = 1.0;
		Diffuse[3] = 1.0;
		Specular[3] = 1.0;
		Shininess[0] = shininess;
	}

};
