#include<math.h>
#include"Object.hpp"
using namespace std;


class Camera
{
private:
	Point CurrentPlace; //��ǰ�������λ��
	float R_Horizontal; //XOZƽ��İ뾶
	float Arc_Horizontal; //XOZƽ��Ļ��ȣ�0-2pi��
	float H_Vertical; //Y��߶�
	int MouseX; //�ϴ�����λ��
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

	//����������¼�
	void MouseDown(int x, int y)
	{
		MouseX = x;
		MouseY = y;
	}

	//��������ƶ��¼�
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