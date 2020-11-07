/*
描述：光线追踪光源和光线定义
日期：2020/11/7
*/
#pragma once
#include<cmath>
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
	返回：有交点返回交点对应的光线t值，否则返回-1
	*/
	float GetIntersection(Board& b)
	{
		/*
		float current_y = StartPlace.y;
		float dy = Direction.y;
		if (dy == 0)
		{
			return -1;
		}
		float t = (b.Y - current_y) / dy;
		if (t <= 0)
		{
			return -1;
		}
		
		Point intersection_point = StartPlace + Direction * t;
		intersection_point.y = b.Y;
		if(intersection_point.x <= b.XHigh && intersection_point.x >= b.XLow && intersection_point.x)
		*/
		return 0;
	}

	/*
	描述：和面片求交
	参数：边界
	返回：有交点返回交点对应的光线t值，否则返回-1
	*/
	float GetIntersection(Board& b)
	{
		return 0;
	}
};