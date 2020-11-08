#include"Ray.hpp"


//光线追踪相关函数
/*
描述：光线追踪主函数
参数：光线，深度
返回：得到的颜色
*/
/*
Color RayTracing(Ray& the_ray, int depth)
{
	if (depth > MaxDepth)
	{
		return Color(0.0, 0.0, 0.0);
	}

	//和所有物体求交得到最近的t
	vector<float> t_list;
	int i_floor = -1;
	int j_floor = -1;
	float t_floor = -1;
	the_ray.GetIntersection(Floor, i_floor, j_floor, t_floor);
	t_list.push_back(t_floor);

	int i_bunny = -1;
	float t_bunny = -1;
	the_ray.GetIntersection(Bunny, i_bunny, t_bunny);
	t_list.push_back(t_bunny);

	int i_dragon = -1;
	float t_dragon = -1;
	the_ray.GetIntersection(Dragon, i_dragon, t_dragon);
	t_list.push_back(t_dragon);

	int i_happy = -1;
	float t_happy = -1;
	the_ray.GetIntersection(Happy, i_happy, t_happy);
	t_list.push_back(t_happy);

	int min_id = GetSmallestNum(t_list);
	if (min_id <= 0 || min_id >= 4)
	{
		return Color(0.0, 0.0, 0.0);
	}

	//求交点，反射光线，颜色
	Point intersection;
	Ray reflection;
	Color color;
	if (min_id == 0)
	{
		//TODO 交点处光强是啥？
		RectangleMesh the_rectangle = Floor.RectangleList[i_floor][j_floor];
		color.R = the_rectangle.Color[0];
		color.G = the_rectangle.Color[1];
		color.B = the_rectangle.Color[2];

		reflection = the_ray.GetReflection(Floor, i_floor, j_floor, t_floor);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 1)
	{
		//TODO 交点处光强是啥？
		TriangleMesh the_triangle = Bunny.Faces[i_bunny];
		color.R = the_triangle.Color[0];
		color.G = the_triangle.Color[1];
		color.B = the_triangle.Color[2];

		reflection = the_ray.GetReflection(Bunny, i_bunny, t_bunny);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 2)
	{
		//TODO 交点处光强是啥？
		TriangleMesh the_triangle = Dragon.Faces[i_dragon];
		color.R = the_triangle.Color[0];
		color.G = the_triangle.Color[1];
		color.B = the_triangle.Color[2];

		reflection = the_ray.GetReflection(Dragon, i_dragon, t_dragon);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 3)
	{
		//TODO 交点处光强是啥？
		TriangleMesh the_triangle = Happy.Faces[i_happy];
		color.R = the_triangle.Color[0];
		color.G = the_triangle.Color[1];
		color.B = the_triangle.Color[2];

		reflection = the_ray.GetReflection(Happy, i_happy, t_happy);
		intersection = reflection.StartPlace;
	}

	Color color_reflection = RayTracing(reflection, depth + 1);

	//TODO:KS
	Color result = color;
	return result;
}


int main_ray_trace(int argc, char**argv)
{
	return 0;
}*/
