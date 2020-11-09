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
	float Intensity;
	float RefractionRate;

	Ray(){}

	void Init(Point start, Point direction, float intensity, float refraction)
	{
		StartPlace = start;
		Direction = direction;
		Intensity = intensity;
		RefractionRate = refraction;
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

		float k = b.KReflection;
		float new_intensity = Intensity * k;

		Ray new_ray;
		new_ray.Init(intersection_point, new_direction, new_intensity, RefractionRate);
		return new_ray;
	}

	/*
	描述：和边界求交生成折射光线（空）
	参数：边界，交点对应的网格id i，j，交点的t值
	返回：新的光线
	*/
	Ray GetRefraction(Board& b, int i, int j, float t)
	{
		Point intersection_point = StartPlace + Direction * t;
		intersection_point.y = b.Y;
		Ray new_ray;
		new_ray.Init(intersection_point, Direction, 0, 1);
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
	描述：和包围盒求交
	参数：包围盒
	返回：交点最小的t，不存在返回-1
	*/
	float GetIntersection(BoundingBox* m)
	{
		
		float t_x_down = -1, t_x_up = -1, t_y_down = -1, t_y_up = -1, t_z_down = -1, t_z_up = -1;
		if (Direction.x != 0)
		{
			t_x_down = (m->PointDown.x - StartPlace.x) / Direction.x;
			t_x_up = (m->PointUp.x - StartPlace.x) / Direction.x;
			Point p_x_down = StartPlace + Direction * t_x_down;
			Point p_x_up = StartPlace + Direction * t_x_up;
			if (t_x_down <= 0 ||
				JudgePointInsideRectangle(p_x_down.y, p_x_down.z,
					m->PointDown.y, m->PointDown.z, m->PointUp.y, m->PointUp.z) == 0)
			{
				t_x_down = -1;
			}
			if (t_x_up <= 0 ||
				JudgePointInsideRectangle(p_x_up.y, p_x_up.z,
					m->PointDown.y, m->PointDown.z, m->PointUp.y, m->PointUp.z) == 0)
			{
				t_x_up = -1;
			}

		}
		if (Direction.y != 0)
		{
			t_y_down = (m->PointDown.y - StartPlace.y) / Direction.y;
			t_y_up = (m->PointUp.y - StartPlace.y) / Direction.y;
			Point p_y_down = StartPlace + Direction * t_y_down;
			Point p_y_up = StartPlace + Direction * t_y_up;
			if (t_y_down <= 0 ||
				JudgePointInsideRectangle(p_y_down.x, p_y_down.z,
					m->PointDown.x, m->PointDown.z, m->PointUp.x, m->PointUp.z) == 0)
			{
				t_y_down = -1;
			}
			if (t_y_up <= 0 ||
				JudgePointInsideRectangle(p_y_up.x, p_y_up.z,
					m->PointDown.x, m->PointDown.z, m->PointUp.x, m->PointUp.z) == 0)
			{
				t_y_up = -1;
			}
		}
		if (Direction.z != 0)
		{
			t_z_down = (m->PointDown.z - StartPlace.z) / Direction.z;
			t_z_up = (m->PointUp.z - StartPlace.z) / Direction.z;
			Point p_z_down = StartPlace + Direction * t_z_down;
			Point p_z_up = StartPlace + Direction * t_z_up;
			if (t_z_down <= 0 ||
				JudgePointInsideRectangle(p_z_down.x, p_z_down.y,
					m->PointDown.x, m->PointDown.y, m->PointUp.x, m->PointUp.y) == 0)
			{
				t_z_down = -1;
			}
			if (t_z_up <= 0 ||
				JudgePointInsideRectangle(p_z_up.x, p_z_up.y,
					m->PointDown.x, m->PointDown.y, m->PointUp.x, m->PointUp.y) == 0)
			{
				t_z_up = -1;
			}
		}
		vector<float> t_list;
		t_list.clear();
		t_list.push_back(t_x_down);
		t_list.push_back(t_x_up);
		t_list.push_back(t_y_down);
		t_list.push_back(t_y_up);
		t_list.push_back(t_z_down);
		t_list.push_back(t_z_up);
		int min_id = GetSmallestNum(t_list);
		if (min_id < 0)
		{
			return -1;
		}
		return t_list[min_id];
	}

	/*
	描述：递归找包围盒里需要比较的面片
	参数：包围盒指针
	返回：在vector里添加面片和id
	*/
	void GetPossibleMeshRecursive(vector<TriangleMesh>& mesh_list, vector<int>& mesh_id_list, BoundingBox* m)
	{
		if (m == NULL) return;
		float t = GetIntersection(m);
		if (t <= 0) return;
		if (m->Sons[0] != NULL)
		{
			for (int i = 0; i < 8; i++)
			{
				GetPossibleMeshRecursive(mesh_list, mesh_id_list, m->Sons[i]);
			}
		}
		else
		{
			for (int i = 0; i < m->MeshList.size(); i++)
			{
				mesh_list.push_back(m->MeshList[i]);
				mesh_id_list.push_back(m->MeshID[i]);
			}
		}
	}

	/*
	描述：和面片模型求交（加速了）
	参数：面片模型
	返回：交点对应的面片id i，交点的t值，不存在返回-1
	*/
	void GetIntersectionFast(MeshModel&m, int&i, float& t)
	{
		float min_t = 14530529;
		float min_id = -1;
		
		vector<TriangleMesh> mesh_list;
		vector<int> mesh_id_list;
		mesh_list.clear();
		mesh_id_list.clear();
		GetPossibleMeshRecursive(mesh_list, mesh_id_list, m.TheBoundingBox);
		for (int i = 0; i < mesh_list.size(); i++)
		{
			float t = -1;
			GetIntersection(mesh_list[i], t);
			if (t > 0 && t < min_t)
			{
				min_t = t;
				min_id = mesh_id_list[i];
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

		float k = m.Faces[i].KReflection;
		float new_intensity = Intensity * k;

		Ray new_ray;
		new_ray.Init(intersection_point, new_direction, new_intensity, RefractionRate);
		return new_ray;
	}

	/*
	描述：和三角面片模型求交生成折射光线
	参数：三角面片模型，交点对应的面片id i，交点的t值
	返回：新的光线
	*/
	Ray GetRefraction(MeshModel& m, int i, float t)
	{
		Point intersection_point = StartPlace + Direction * t;
		
		float dist = sqrt(Direction * m.Faces[i].Norm);
		Point norm_speed = m.Faces[i].Norm * dist;
		Point tangent_speed = Direction - norm_speed;
		
		float x = tangent_speed.Dist();
		float y = norm_speed.Dist();
		float n1 = RefractionRate;
		float n2 = m.RefractionRate;
		float k_x = n1 * y / sqrt(n2 * n2 * (x * x + y * y) - n1 * n1 * x * x);
		Point new_speed = norm_speed + tangent_speed * k_x;
		new_speed.Normalize();

		float k = m.Faces[i].KRefraction;
		float new_intensity = Intensity * k;

		Ray new_ray;
		new_ray.Init(intersection_point, new_speed, new_intensity, n2);
		return new_ray;
	}

};

