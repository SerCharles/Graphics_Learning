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
	float Size = 0;
	Point BasePlace;

	//������ʾ�ĵ�
	Point BottomPoints[4];
	Point UpPoints[4];


	//���ʣ�������ɫ��Ϣ
	GLfloat Color[3] = { 0, 0, 0 }; //��ɫ
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //������
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //������
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //���淴��
	GLfloat Shininess[4] = { 0 }; //����ָ��

	//��ʼ��λ����Ϣ
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

	//��������
	void Draw()
	{
		glColor3f(Color[0], Color[1], Color[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Shininess);


		DrawRectangle(BottomPoints[0], BottomPoints[1], UpPoints[1], UpPoints[0]);
		DrawRectangle(BottomPoints[1], BottomPoints[2], UpPoints[2], UpPoints[1]);
		DrawRectangle(BottomPoints[2], BottomPoints[3], UpPoints[3], UpPoints[2]);
		DrawRectangle(BottomPoints[3], BottomPoints[0], UpPoints[0], UpPoints[3]);
		DrawRectangle(UpPoints[0], UpPoints[1], UpPoints[2], UpPoints[3]);
		DrawRectangle(BottomPoints[0], BottomPoints[1], BottomPoints[2], BottomPoints[3]);

		glFlush();
	}
};
