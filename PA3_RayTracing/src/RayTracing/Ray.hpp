/*
描述：光线追踪光源和光线定义
日期：2020/11/7
*/
#pragma once
#include<cmath>
#include<vector>
#include"Point.hpp"
#include"Board.hpp"
#include"MeshModel.hpp"



class Ray
{

public:
	Point StartPlace;
	Point Direction;
	Color TheColor;
	float Intensity;

	Ray(){}

	void Init(Point start, Point direction, Color color, float intensity)
	{
		StartPlace = start;
		Direction = direction;
		TheColor = color;
		Intensity = intensity;
	}

	/*
	描述：和边界求交（默认y=0）
	参数：边界
	返回：交点对应的网格id i，j，交点的t值，交点不存在的话返回-1
	*/
	void GetIntersection(Board& b, int& i, int& j, float& t)
	{
		
		float current_y = StartPlace.y;
		float dy = Direction.y;
		if (dy == 0)
		{
			i = -1;
			j = -1;
			t = -1;
			return;
		}
		t = (b.Y - current_y) / dy;
		if (t <= 0)
		{
			i = -1;
			j = -1;
			t = -1;
			return;
		}
		
		Point intersection_point = StartPlace + Direction * t;
		intersection_point.y = b.Y;
		b.GetIntersectionID(intersection_point, i, j);
		if (i == -1 || j == -1)
		{
			i = -1;
			j = -1;
			t = -1;
			return;
		}
	}

	/*
	描述：和边界求交生成反射光线
	参数：边界，交点对应的网格id i，j，交点的t值
	返回：新的光线
	*/
	Ray GetReflection(Board& b, int i, int j, float t)
	{
		Point intersection_point = StartPlace + Direction * t;
		intersection_point.y = b.Y;
		Point new_direction = Point(Direction.x, -Direction.y, Direction.z);

		//TODO：颜色和强度
		Color new_color = TheColor;
		float new_intensity = Intensity;

		Ray new_ray;
		new_ray.Init(intersection_point, new_direction, new_color, new_intensity);
		return new_ray;
	}

	

	/*
	描述：和一个面片求交
	参数：面片
	返回：交点的t值，不存在返回-1
	*/
	void GetIntersection(TriangleMesh& m, float& t)
	{
		float D = -(m.v1 * m.Norm);
		float up = (m.Norm * StartPlace) + D;
		float down = (m.Norm * Direction);
		if (down == 0)
		{
			t = -1;
			return;
		}
		float get_t = -up / down;
		if (get_t <= 0)
		{
			t = -1;
			return;
		}
		Point intersection = StartPlace + Direction * get_t;
		vector<Point> polygon;
		polygon.clear();
		polygon.push_back(m.v1);
		polygon.push_back(m.v2);
		polygon.push_back(m.v3);
		bool result = JudgeInside(intersection, polygon);
		if (result == 0)
		{
			t = -1;
			return;
		}
		t = get_t;
	}


	/*
	描述：和面片模型求交
	参数：面片模型
	返回：交点对应的面片id i，交点的t值，不存在返回-1
	*/
	void GetIntersection(MeshModel& m, int& i, float& t)
	{
		float min_t = 14530529;
		float min_id = -1;
		for (int j = 0; j < m.FaceNum; j++)
		{
			TriangleMesh mesh = m.Faces[j];
			float current_t = -1;
			GetIntersection(m.Faces[j], current_t);
			if (current_t > 0 && current_t < min_t)
			{
				min_t = current_t;
				min_id = j;
			}
		}
		if (min_id < 0)
		{
			i = -1;
			t = -1;
			return;
		}
		i = min_id;
		t = min_t;
	}

	/*
	描述：和三角面片模型求交生成反射光线
	参数：三角面片模型，交点对应的面片id i，交点的t值
	返回：新的光线
	*/
	Ray GetReflection(MeshModel& m, int i, float t)
	{
		Point intersection_point = StartPlace + Direction * t;
		//切向不变法向取反
		float dist = sqrt(Direction * m.Faces[i].Norm);
		Point norm_speed = m.Faces[i].Norm * dist;
		Point tangent_speed = Direction - norm_speed;
		Point new_direction = tangent_speed - norm_speed;

		//TODO：颜色和强度
		Color new_color = TheColor;
		float new_intensity = Intensity;

		Ray new_ray;
		new_ray.Init(intersection_point, new_direction, new_color, new_intensity);
		return new_ray;
	}

	
};