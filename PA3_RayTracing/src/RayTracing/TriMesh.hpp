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
	//�洢λ��
	string BaseDir = "res//";
	string FullDir = BaseDir;
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
	������stringתint
	������string
	���أ�int
	*/
	int StringToInt(string str)
	{
		int length = str.size();
		int int_base = 1;
		int sum = 0;
		for (int i = length - 1; i >= 0; i--)
		{
			sum += (str[i] - '0') * int_base;
			int_base *= 10;
		}
		return sum;
	}

	/*
	��������ȡply�ļ�ͷ
	�������ļ���
	���أ�property����
	*/
	int ReadPLYHead(fstream& f)
	{
		int properties = 0;
		while (1)
		{
			string line;
			string vertex_line = "element vertex ";
			string face_line = "element face ";
			string property_line = "property";
			string end_line = "end_header";
			getline(f, line);
			string substr_15 = line.substr(0, 15);
			string substr_13 = line.substr(0, 13);
			string substr_8 = line.substr(0, 8);
			if (substr_15 == vertex_line)
			{
				string vertex_num = line.substr(15);
				VertexNum = StringToInt(vertex_num);
			}
			else if (substr_13 == face_line)
			{
				string face_num = line.substr(13);
				FaceNum = StringToInt(face_num);
			}
			else if (substr_8 == property_line)
			{
				properties++;
			}
			else if (line == end_line)
			{
				break;
			}

		}
		return properties;
	}

	/*
	��������ʼ����Ƭ�����ҽ��й�һ��
	�������ļ��������õĴ�С�����õ����λ��
	���أ���
	*/
	void InitPlace(string filename, float size, Point base)
	{
		FullDir += filename;
		Size = size;
		Center = base;
		VertexNum = 0;
		FaceNum = 0;
		fstream f(FullDir);

		int properties = ReadPLYHead(f);

		//֮��VertexNum��ÿ�����ֺͣ�ǰ����xyz
		for (int i = 0; i < VertexNum; i++)
		{
			float x, y, z;
			f >> x >> y >> z ;
			Point nova(x, y, z);
			Vertexs.push_back(nova);
			for (int i = 3; i < properties - 1; i++)
			{
				float temp;
				f >> temp;
			}
		}
		//��һ��
		NormalizeVertexs();


		//֮��FaceNum��ÿ��4�����֣�3���Լ���������
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