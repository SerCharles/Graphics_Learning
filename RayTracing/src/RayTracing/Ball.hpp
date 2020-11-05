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
	//λ�ã��ٶ���Ϣ
	Point CurrentPlace;
	float Radius;

	//���ʣ�������ɫ��Ϣ
	GLfloat Color[3] = { 0, 0, 0 }; //��ɫ
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //������
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //������
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //���淴��
	GLfloat Shininess[4] = { 0 }; //����ָ��
public:
	Ball(){}

	//��ʼ��λ�ã��ٶ���Ϣ
	void InitPlace(float x, float z, float radius)
	{
		CurrentPlace.SetPlace(x, radius, z);
		Radius = radius;
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

	//��㵽���ľ���
	float GetDistance(Point p)
	{ 
		return sqrt((p - CurrentPlace) * (p - CurrentPlace));
	}

};