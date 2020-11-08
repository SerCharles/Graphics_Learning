/*
描述：三角面片和模型类
日期：2020/11/6
*/
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
class TriangleMesh
{
public:
	Point v1;
	Point v2;
	Point v3;
	Point Norm;

	//材质，纹理，颜色信息
	GLfloat Color[3] = { 0, 0, 0 }; //颜色
	GLfloat Ambient[4] = { 0, 0, 0, 0 }; //环境光
	GLfloat Diffuse[4] = { 0, 0, 0, 0 }; //漫反射
	GLfloat Specular[4] = { 0, 0, 0, 0 }; //镜面反射
	GLfloat Shininess[4] = { 0 }; //镜面指数

	TriangleMesh(){}
	
	void GetNorm()
	{
		Point l1 = v1 - v2;
		Point l2 = v2 - v3;
		float norm_x = (l1.y * l2.z) - (l1.z * l2.y);
		float norm_y = -((l1.x * l2.z) - (l1.z * l2.x));
		float norm_z = (l1.x * l2.y) - (l1.y * l2.x);
		Norm.SetPlace(norm_x, norm_y, norm_z);
		float size = sqrt(Norm.Square());
		Norm = Norm / size;
	}

	void InitPlace(Point a, Point b, Point c)
	{
		v1 = a;
		v2 = b;
		v3 = c;
		GetNorm();
	}

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

class MeshModel
{
public:
	//存储位置
	string BaseDir = "res//";
	string FullDir = BaseDir;
	//位置信息：大小和中心；点和面片的个数；点和面片的位置以及面片的点id
	float Size;
	Point Center;
	int VertexNum;
	int FaceNum;
	vector<Point> Vertexs;
	vector<TriangleMesh> Faces;
	vector<MeshPoints> FaceIDs;
	MeshModel() {}

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
		for (int i = 0; i < FaceNum; i++)
		{
			Faces[i].InitColor(color, ambient, diffuse, specular, shininess);
		}
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
	描述：string转int
	参数：string
	返回：int
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
	描述：读取ply文件头
	参数：文件流
	返回：property个数
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
	描述：初始化面片，并且进行归一化
	参数：文件名，设置的大小，设置的相对位置
	返回：无
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

		//之后VertexNum行每行数字和，前三个xyz
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
		//归一化
		NormalizeVertexs();


		//之后FaceNum行每行4个数字，3，以及三个点编号
		for (int i = 0; i < FaceNum; i++)
		{
			int num, v1_num, v2_num, v3_num;
			f >> num >> v1_num >> v2_num >> v3_num;
			Point v1 = Vertexs[v1_num];
			Point v2 = Vertexs[v2_num];
			Point v3 = Vertexs[v3_num];
			TriangleMesh nova;
			nova.InitPlace(v1, v2, v3);
			MeshPoints nova_num;
			nova_num.InitPlace(v1_num, v2_num, v3_num);
			Faces.push_back(nova);
			FaceIDs.push_back(nova_num);
		}
	}

};