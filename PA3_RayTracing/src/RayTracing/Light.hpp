/*
����������׷�ٹ�Դ�͹��߶���
���ڣ�2020/11/7
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
	�������ͱ߽��󽻣�Ĭ��y=0��
	�������߽�
	���أ��н��㷵�ؽ����Ӧ�Ĺ���tֵ�����򷵻�-1
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
	����������Ƭ��
	�������߽�
	���أ��н��㷵�ؽ����Ӧ�Ĺ���tֵ�����򷵻�-1
	*/
	float GetIntersection(Board& b)
	{
		return 0;
	}
};