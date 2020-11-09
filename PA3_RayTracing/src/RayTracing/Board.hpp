/*
�������߽���
���ڣ�2020/11/6
*/
#pragma once
#include<math.h>
#include"Point.hpp"
using namespace std;

//�߽������
class RectangleMesh
{
public:
	//λ����Ϣ
	float SizeX = 0, SizeZ = 0;
	Point PointList[4];
	Point Norm;
	//���ʣ�������ɫ��Ϣ
	GLfloat Color[3] = { 0, 0, 0 }; //��ɫ
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //������
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //������
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //���淴��
	GLfloat Shininess[1] = { 0 }; //����ָ��
	float KReflection = 0.0;
	float KRefraction = 0.0;
	float RefractionRate = 0.0;

	RectangleMesh() {}
	void InitPlace(float start_x, float start_z, float size_x, float size_z, float y)
	{
		PointList[0].SetPlace(start_x, y, start_z);
		PointList[1].SetPlace(start_x + size_x, y, start_z);
		PointList[2].SetPlace(start_x + size_x, y, start_z + size_z);
		PointList[3].SetPlace(start_x, y, start_z + size_z);
		Norm = Point(0, 1, 0);
	}

	void InitColor(GLfloat color[], GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess,
		float k_reflection, float k_refraction, float refraction_rate)
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
		KReflection = k_reflection;
		KRefraction = k_refraction;
		RefractionRate = refraction_rate;
	}
};


//�߽���
class Board
{
public:
	//λ����Ϣ
	float XHigh = 0, XLow = 0, ZHigh = 0, ZLow = 0, Y = 0;
	const int XNum = 200;
	const int ZNum = 200;
	float XSize = 0, ZSize = 0;
	RectangleMesh RectangleList[200][200];

	//���ʣ�������ɫ��Ϣ
	GLfloat Color[3] = { 0, 0, 0 }; //��ɫ
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //������
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //������
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //���淴��
	GLfloat Shininess[1] = { 0 }; //����ָ��
	float KReflection = 0.0;
	float KRefraction = 0.0;
	float RefractionRate = 0.0;


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

	void InitColor(GLfloat color[], GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess, 
		float k_reflection, float k_refraction, float refraction_rate)
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
		KReflection = k_reflection;
		KRefraction = k_refraction;
		RefractionRate = refraction_rate;
		for (int i = 0; i < XNum; i++)
		{
			for (int j = 0; j < ZNum; j++)
			{
				RectangleList[i][j].InitColor(color, ambient, diffuse, specular, shininess, k_reflection, k_refraction, refraction_rate);
			}
		}
	}


	/*
	��������ȡ�������ڵ���������
	��������������
	���أ���������i��j��������ڱ߽��ڣ�����-1
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