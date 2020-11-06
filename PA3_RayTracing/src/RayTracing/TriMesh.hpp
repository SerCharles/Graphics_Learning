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

//三角面片定义
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

//三角面片顶点编号定义
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
	//位置信息：大小和中心；点和面片的个数；点和面片的位置以及面片的点id
	float Size;
	Point Center;
	int VertexNum;
	int FaceNum;
	vector<Point> Vertexs;
	vector<Triangle> Faces;
	vector<MeshPoints> FaceIDs;
	TriMesh() {}

	//材质，纹理，颜色信息
	GLfloat Color[3] = { 0, 0, 0 }; //颜色
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //环境光
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //漫反射
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //镜面反射
	GLfloat Shininess[4] = { 0 }; //镜面指数
	
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

	/*
	描述：归一化面片的顶点
	参数：无
	返回：无
	*/
	void NormalizeVertexs()
	{
		//求均值
		Point mean(0, 0, 0);
		for (int i = 0; i < VertexNum; i++)
		{
			mean = mean + Vertexs[i];
		}
		mean = mean / float(VertexNum);

		//求标准差
		float square_error = 0;
		for (int i = 0; i < VertexNum; i++)
		{
			Point dist = mean - Vertexs[i];
			float error = dist.Square();
			square_error += error;
		}
		square_error /= float(VertexNum);
		float std_error = sqrt(square_error);

		//-均值 /标准差 *大小 +偏置 
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
	描述：初始化面片，并且进行归一化
	参数：文件名，设置的大小，设置的相对位置
	返回：无
	*/
	void InitPlace(const char filename[], float size, Point base)
	{
		Size = size;
		Center = base;
		VertexNum = 35947;
		FaceNum = 69451;
		int HeadLen = 12;
		fstream f(filename);

		//前12行是头
		string line;
		for (int i = 0; i < HeadLen; i++)
		{
			getline(f, line);
		}

		//之后35947行每行五个数字，前三个xyz
		for (int i = 0; i < VertexNum; i++)
		{
			float x, y, z, confidence, intensity;
			f >> x >> y >> z >> confidence >> intensity;
			Point nova(x, y, z);
			Vertexs.push_back(nova);
		}
		//归一化
		NormalizeVertexs();


		//之后69451行每行4个数字，3，以及三个点编号
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

	//绘制自身
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