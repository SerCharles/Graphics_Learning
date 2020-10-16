#pragma once
#include<math.h>
#include"Object.hpp"
#include"Board.hpp"
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

	//处理移动
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

	void HandleCollisionBoard(float XRange, float ZRange)
	{
		if (CurrentPlace.x - Radius < -XRange)
		{
			CurrentPlace.x = -XRange + Radius;
			CurrentSpeed.x = -CurrentSpeed.x;
		}
		else if (CurrentPlace.x + Radius > XRange)
		{
			CurrentPlace.x = XRange - Radius;
			CurrentSpeed.x = -CurrentSpeed.x;
		}
		if (CurrentPlace.z - Radius < -ZRange)
		{
			CurrentPlace.z = -ZRange + Radius;
			CurrentSpeed.z = -CurrentSpeed.z;
		}
		else if (CurrentPlace.z + Radius > ZRange)
		{
			CurrentPlace.z = ZRange - Radius;
			CurrentSpeed.z = -CurrentSpeed.z;
		}
	}

	void HandleCollisionBall(MovingBall& b)
	{
		Point diff = b.CurrentPlace - CurrentPlace;
		float dist = sqrt(diff * diff);
		if (dist < Radius + b.Radius)
		{
			//径向交换速度，法向速度不变
			Point speed_collide_self = diff * (CurrentSpeed * diff / dist / dist);
			Point speed_collide_b = diff * (b.CurrentSpeed * diff / dist / dist);
			Point unchanged_self = CurrentSpeed - speed_collide_self;
			Point unchanged_b = b.CurrentSpeed - speed_collide_b;
			Point new_self = unchanged_self + speed_collide_b;
			Point new_b = unchanged_b + speed_collide_self;
			CurrentSpeed = new_self;
			b.CurrentSpeed = new_b;

			
		}
	}
};