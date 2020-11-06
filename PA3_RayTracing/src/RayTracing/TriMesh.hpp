#pragma once
#include"Point.hpp"
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<GL/glut.h>
#include<string>
#include<vector>
#include<cmath>
using namespace std;

//������Ƭ����
class Triangle
{
public:
	Point v1;
	Point v2;
	Point v3;
	Triangle(){}
	void InitPlace(Point a, Point b, Point c)
	{
		v1 = a;
		v2 = b;
		v3 = c;
	}
};

//������Ƭ�����Ŷ���
class MeshPoints
{
public:
	int id1;
	int id2;
	int id3;
	MeshPoints() {}
	void InitPlace(int a, int b, int c)
	{
		id1 = a;
		id2 = b;
		id3 = c;
	}
};

class TriMesh
{
public:
	//λ����Ϣ����С�����ģ������Ƭ�ĸ����������Ƭ��λ���Լ���Ƭ�ĵ�id
	float Size;
	Point Center;
	int VertexNum;
	int FaceNum;
	vector<Point> Vertexs;
	vector<Triangle> Faces;
	vector<MeshPoints> FaceIDs;
	TriMesh() {}

	//���ʣ�������ɫ��Ϣ
	GLfloat Color[3] = { 0, 0, 0 }; //��ɫ
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //������
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //������
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //���淴��
	GLfloat Shininess[4] = { 0 }; //����ָ��
	
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

	/*
	��������һ����Ƭ�Ķ���
	��������
	���أ���
	*/
	void NormalizeVertexs()
	{
		//���ֵ
		Point mean(0, 0, 0);
		for (int i = 0; i < VertexNum; i++)
		{
			mean = mean + Vertexs[i];
		}
		mean = mean / float(VertexNum);

		//���׼��
		float square_error = 0;
		for (int i = 0; i < VertexNum; i++)
		{
			Point dist = mean - Vertexs[i];
			float error = dist.Square();
			square_error += error;
		}
		square_error /= float(VertexNum);
		float std_error = sqrt(square_error);

		//-��ֵ /��׼�� *��С +ƫ�� 
		for (int i = 0; i < VertexNum; i++)
		{
			Point p = Vertexs[i];
			p = p - mean;
			p = p / std_error;
			p = p * Size;
			p = p + Center;
			Vertexs[i] = p;
		}
	}

	/*
	��������ʼ����Ƭ�����ҽ��й�һ��
	�������ļ��������õĴ�С�����õ����λ��
	���أ���
	*/
	void InitPlace(const char filename[], float size, Point base)
	{
		Size = size;
		Center = base;
		VertexNum = 35947;
		FaceNum = 69451;
		int HeadLen = 12;
		fstream f(filename);

		//ǰ12����ͷ
		string line;
		for (int i = 0; i < HeadLen; i++)
		{
			getline(f, line);
		}

		//֮��35947��ÿ��������֣�ǰ����xyz
		for (int i = 0; i < VertexNum; i++)
		{
			float x, y, z, confidence, intensity;
			f >> x >> y >> z >> confidence >> intensity;
			Point nova(x, y, z);
			Vertexs.push_back(nova);
		}
		//��һ��
		NormalizeVertexs();


		//֮��69451��ÿ��4�����֣�3���Լ���������
		for (int i = 0; i < FaceNum; i++)
		{
			int num, v1_num, v2_num, v3_num;
			f >> num >> v1_num >> v2_num >> v3_num;
			Point v1 = Vertexs[v1_num];
			Point v2 = Vertexs[v2_num];
			Point v3 = Vertexs[v3_num];
			Triangle nova;
			nova.InitPlace(v1, v2, v3);
			MeshPoints nova_num;
			nova_num.InitPlace(v1_num, v2_num, v3_num);
			Faces.push_back(nova);
			FaceIDs.push_back(nova_num);
		}
	}

	//��������
	void Draw()
	{
		glColor3f(Color[0], Color[1], Color[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Shininess);

		for (int i = 0; i < FaceNum; i++)
		{
			Triangle the_face = Faces[i];
			DrawTriangle(the_face.v1, the_face.v2, the_face.v3);
		}
		glFlush();
	}
};