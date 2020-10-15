#include<math.h>
using namespace std;

#define PI 3.1415926



//3D点的基类
class Point
{
public:
	float x;
	float y;
	float z;
	Point()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Point(float tx, float ty, float tz)
	{
		x = tx;
		y = ty;
		z = tz;
	}
	void SetPlace(float tx, float ty, float tz)
	{
		x = tx;
		y = ty;
		z = tz;
	}
};

