#include<math.h>
#include"Object.hpp"
using namespace std;


class Camera
{
private:
	Point CurrentPlace; //当前相机所在位置
	float R_Horizontal; //XOZ平面的半径
	float Arc_Horizontal; //XOZ平面的弧度（0-2pi）
	float H_Vertical; //Y轴高度
	int MouseX; //上次鼠标的位置
	int MouseY;
	const float K_Horizontal = 0.002;
	const float K_Vertical = 0.03;
public:
	Camera(float R, float start_height)
	{
		R_Horizontal = R;
		Arc_Horizontal = 0;
		H_Vertical = start_height;
		ResetCurrentPlace();
		MouseX = -1;
		MouseY = -1;
	}
	Point GetCurrentPlace()
	{
		return CurrentPlace;
	}

	void ResetCurrentPlace()
	{
		float x = R_Horizontal * cos(Arc_Horizontal);
		float y = H_Vertical;
		float z = R_Horizontal * sin(Arc_Horizontal);
		CurrentPlace.SetPlace(x, y, z);
	}

	//处理按下鼠标事件
	void MouseDown(int x, int y)
	{
		MouseX = x;
		MouseY = y;
	}

	//处理鼠标移动事件
	void MouseMove(int x, int y)
	{
		int dx = x - MouseX;
		int dy = y - MouseY;
		Arc_Horizontal = Arc_Horizontal + dx * K_Horizontal;
		while (Arc_Horizontal < 0) Arc_Horizontal += 2.0 * PI;
		while (Arc_Horizontal >= 2.0 * PI) Arc_Horizontal -= 2.0 * PI;
		H_Vertical += dy * K_Vertical;
		ResetCurrentPlace();
		MouseX = x;
		MouseY = y;
	}
	
};