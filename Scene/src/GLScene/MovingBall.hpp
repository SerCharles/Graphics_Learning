#include<math.h>
#include"Object.hpp"
using namespace std;

#pragma once
class MovingBall
{
public:
	Point CurrentPlace;
	float Radius;
	Point CurrentSpeed;
public:
	MovingBall()
	{

	}
	MovingBall(float x, float z, float radius, float speed_x, float speed_z)
	{
		CurrentPlace.SetPlace(x, radius, z);
		CurrentSpeed.SetPlace(speed_x, 0, speed_z);
		Radius = radius;
	}
	void Init(float x, float z, float radius, float speed_x, float speed_z)
	{
		CurrentPlace.SetPlace(x, radius, z);
		CurrentSpeed.SetPlace(speed_x, 0, speed_z);
		Radius = radius;
	}

	Point SetCurrentSpeed(float speed_x, float speed_z)
	{
		CurrentSpeed.SetPlace(speed_x, 0, speed_z);
	}

	//¥¶¿Ì“∆∂Ø
	void Move(float time)
	{
		float dx = CurrentSpeed.x * time;
		float dy = CurrentSpeed.y * time;
		float dz = CurrentSpeed.z * time;
		float new_x = dx + CurrentPlace.x;
		float new_y = dy + CurrentPlace.y;
		float new_z = dz + CurrentPlace.z;
		CurrentPlace.SetPlace(new_x, new_y, new_z);
	}




};