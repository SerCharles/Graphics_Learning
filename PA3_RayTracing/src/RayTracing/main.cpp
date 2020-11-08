#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include "Point.hpp"
#include "GLLight.hpp"
#include "Camera.hpp"
#include "Board.hpp"
#include "MeshModel.hpp"
#include "Ray.hpp"
using namespace std;

#define OPENGL_LIGHT 1
#define MY_LIGHT 2


//全局常量
const int WindowSizeX = 800, WindowSizeY = 600, WindowPlaceX = 100, WindowPlaceY = 100;
const char WindowName[] = "MyScene";
const float TimeOnce = 0.02; //刷新时间
const float XRange = 10, ZRange = 10, Height = 8, YFloor = 0; //场景的X,Y,Z范围（-X,X),(0,H),(-Z,Z)
const int BallComplexity = 40; //小球绘制精细程度
int CurrentMode = -1; //模式：模式1是OpenGL光照，模式2是光线追踪


//光照，相机
Camera TheCamera;
GLLight TheGLLight;

//物体
Board Floor; //边界
//面片物体
MeshModel Bunny, Dragon, Happy;


//初始化函数集合
//初始化窗口
void InitWindow()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WindowSizeX, WindowSizeY);
	glutInitWindowPosition(WindowPlaceX, WindowPlaceY);
	glutCreateWindow(WindowName);
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION);
	const GLubyte* gluVersion = gluGetString(GLU_VERSION);
	printf("OpenGL实现的版本号：%s\n", OpenGLVersion);
	printf("OGLU工具库版本：%s\n", gluVersion);
}

//初始化OpenGL光照
void InitGLLight()
{
	GLfloat background_color[3] = { 0.0, 0.0, 0.0 };
	GLfloat ambient[3] = { 1, 1, 1};
	GLfloat diffuse[3] = { 1, 1, 1};
	GLfloat specular[3] = { 1, 1, 1};
	GLfloat position[3] = { 0.0f, 10.0f, 0.0f};
	TheGLLight.Init(background_color, ambient, diffuse, specular, position);

	//设置着色模式
	glShadeModel(GL_SMOOTH);
	//设置初始背景色，清除颜色缓存和深度缓存
	glClearColor(TheGLLight.Color[0], TheGLLight.Color[1], TheGLLight.Color[2], TheGLLight.Color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (CurrentMode == OPENGL_LIGHT)
	{
		//设置光源信息
		glLightfv(GL_LIGHT0, GL_AMBIENT, TheGLLight.Ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, TheGLLight.Diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, TheGLLight.Specular);
		glLightfv(GL_LIGHT0, GL_POSITION, TheGLLight.Position);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	//设置深度检测，即只绘制最前面的一层
	glEnable(GL_DEPTH_TEST);
}

//初始化相机
void InitCamera()
{
	//设置初始相机位置
	TheCamera.Init(10.0f, 10.0f);
}

//初始化边界和地板
void InitBoards()
{
	//设置地板和挡板位置
	Floor.InitPlace(XRange, -XRange, ZRange, -ZRange, YFloor);

	//地板材质
	GLfloat color_floor[3] = { 1.0, 1.0, 1.0 };
	GLfloat ambient_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat diffuse_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat specular_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat shininess_floor = 40;
	Floor.InitColor(color_floor, ambient_floor, diffuse_floor, specular_floor, shininess_floor);
}

//初始化面片
void InitMeshs()
{
	//兔子
	{
		//面片的大小，位置
		float size = 2;
		Point center = Point(0, 4, 4);

		//面片的纹理，材质，颜色
		GLfloat color[3] = { 0.0, 0.0, 1.0 };
		GLfloat ambient[3] = { 0.2, 0.2, 0.6 };
		GLfloat diffuse[3] = { 0.4, 0.4, 0.4 };
		GLfloat specular[3] = { 0.2, 0.2, 0.2 };
		GLfloat shininess = 100;

		Bunny.InitPlace("bunny.ply", size, center);
		Bunny.InitColor(color, ambient, diffuse, specular, shininess);
	}

	//龙
	{
		//面片的大小，位置
		float size = 2;
		Point center = Point(-5, 5, -3);

		//面片的纹理，材质，颜色
		GLfloat color[3] = { 1.0, 0.0, 0.0 };
		GLfloat ambient[3] = { 0.6, 0.2, 0.2 };
		GLfloat diffuse[3] = { 0.4, 0.4, 0.4 };
		GLfloat specular[3] = { 0.2, 0.2, 0.2 };
		GLfloat shininess = 100;

		Dragon.InitPlace("dragon.ply", size, center);
		Dragon.InitColor(color, ambient, diffuse, specular, shininess);
	}

	//佛
	{
		//面片的大小，位置
		float size = 2;
		Point center = Point(5, 4, -3);

		//面片的纹理，材质，颜色
		GLfloat color[3] = { 1.0, 1.0, 0.0 };
		GLfloat ambient[3] = { 0.6, 0.6, 0.2 };
		GLfloat diffuse[3] = { 0.4, 0.4, 0.4 };
		GLfloat specular[3] = { 0.2, 0.2, 0.2 };
		GLfloat shininess = 100;

		Happy.InitPlace("happy.ply", size, center);
		Happy.InitColor(color, ambient, diffuse, specular, shininess);
	}
}

//初始化的主函数
void InitScene()
{

	InitGLLight();
	InitCamera();
	InitBoards();
	InitMeshs();
}

//绘制函数集合
//设置相机位置
void SetCamera()
{
	glLoadIdentity();
	Point camera_place = TheCamera.CurrentPlace;//这就是视点的坐标  
	Point camera_center = TheCamera.LookCenter;//这是视点中心坐标
	gluLookAt(camera_place.x, camera_place.y, camera_place.z, camera_center.x, camera_center.y, camera_center.z, 0, 1, 0); //从视点看远点,y轴方向(0,1,0)是上方向  
}

/*
描述：计算环境光颜色
参数：光源环境光参数，面片环境光参数
*/
Color GetAmbient(GLfloat ambient_light[], GLfloat ambient_mesh[])
{
	Color ambient;
	ambient.R = ambient_light[0] * ambient_mesh[0];
	ambient.G = ambient_light[1] * ambient_mesh[1];
	ambient.B = ambient_light[2] * ambient_mesh[2];
	return ambient;
}

/*
描述：计算漫反射光颜色
参数：光源漫反射参数，面片漫反射参数，光照的方向（0,1,0），面片法向量
*/
Color GetDiffuse(GLfloat diffuse_light[], GLfloat diffuse_mesh[], Point light_vector, Point mesh_norm)
{
	float weight = (light_vector * mesh_norm) / light_vector.Dist() / mesh_norm.Dist();
	Color diffuse;
	diffuse.R = diffuse_light[0] * diffuse_mesh[0] * weight;
	diffuse.G = diffuse_light[1] * diffuse_mesh[1] * weight;
	diffuse.B = diffuse_light[2] * diffuse_mesh[2] * weight;
	return diffuse;
}

/*
描述：计算镜面反射光颜色
参数：光源镜面反射参数，面片镜面反射参数，光照的方向（0,1,0），视线方向，面片法向量
*/
Color GetSpecular(GLfloat specular_light[], GLfloat specular_mesh[], Point light_vector, Point seeing_direction, Point mesh_norm)
{
	int n = 10;
	Point reflection = mesh_norm * 2 * (mesh_norm * light_vector) - light_vector;
	float weight = reflection * seeing_direction;
	float original_weight = weight;
	for (int i = 0; i < n - 1; i++)
	{
		weight *= original_weight;
	}

	Color specular;
	specular.R = specular_light[0] * specular_mesh[0] * weight;
	specular.G = specular_light[1] * specular_mesh[1] * weight;
	specular.B = specular_light[2] * specular_mesh[2] * weight;
	return specular;
}

/*
描述：针对三角面片的phong模型
参数：当前光源，当前相机，三角面片
返回：颜色
*/
Color PhongModel(GLLight& light, Camera& camera, TriangleMesh& mesh)
{
	Point see_direction = camera.LookCenter - camera.CurrentPlace;
	see_direction = see_direction / see_direction.Dist();
	Color ambient = GetAmbient(light.Ambient, mesh.Ambient);
	Color diffuse = GetDiffuse(light.Diffuse, mesh.Diffuse, light.LightVector, mesh.Norm);
	Color specular = GetSpecular(light.Diffuse, mesh.Diffuse, light.LightVector, see_direction, mesh.Norm);
	Color phong = ambient + diffuse + specular;
	return phong;
}

/*
描述：针对四边形面片的phong模型
参数：当前光源，当前相机，三角面片
返回：颜色
*/
Color PhongModel(GLLight& light, Camera& camera, RectangleMesh& mesh)
{
	Point see_direction = camera.LookCenter - camera.CurrentPlace;
	see_direction = see_direction / see_direction.Dist();
	Color ambient = GetAmbient(light.Ambient, mesh.Ambient);
	Color diffuse = GetDiffuse(light.Diffuse, mesh.Diffuse, light.LightVector, mesh.Norm);
	Color specular = GetSpecular(light.Diffuse, mesh.Diffuse, light.LightVector, see_direction, mesh.Norm);
	Color phong = ambient + diffuse + specular;
	return phong;
}

void DrawOneBoard(Board& board)
{
	for (int i = 0; i < board.XNum; i++)
	{
		for (int j = 0; j < board.ZNum; j++)
		{
			RectangleMesh mesh = board.RectangleList[i][j];
			if (CurrentMode == OPENGL_LIGHT)
			{
				glColor3f(mesh.Color[0], mesh.Color[1], mesh.Color[2]);
				glMaterialfv(GL_FRONT, GL_AMBIENT, mesh.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mesh.Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, mesh.Specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, mesh.Shininess);
			}
			else
			{
				Color phong = PhongModel(TheGLLight, TheCamera, mesh);
				glColor3f(phong.R, phong.G, phong.B);
			}
			DrawRectangle(mesh.PointList[0], mesh.PointList[1], mesh.PointList[2], mesh.PointList[3]);
		}
	}
	glFlush();
}

//绘制边界
void DrawBoards()
{
	DrawOneBoard(Floor);
}

void DrawOneMesh(MeshModel& model)
{
	for (int i = 0; i < model.FaceNum; i++)
	{
		TriangleMesh the_face = model.Faces[i];
		if (CurrentMode == OPENGL_LIGHT)
		{
			glColor3f(the_face.Color[0], the_face.Color[1], the_face.Color[2]);
			glMaterialfv(GL_FRONT, GL_AMBIENT, the_face.Ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, the_face.Diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, the_face.Specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, the_face.Shininess);
		}
		else if (CurrentMode == MY_LIGHT)
		{
			Color phong = PhongModel(TheGLLight, TheCamera, the_face);
			glColor3f(phong.R, phong.G, phong.B);
		}
		DrawTriangle(the_face.v1, the_face.v2, the_face.v3);
	}
	glFlush();
}

//绘制mesh
void DrawMeshs()
{
	DrawOneMesh(Bunny);
	DrawOneMesh(Dragon);
	DrawOneMesh(Happy);
}

//绘制的主函数
void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除颜色缓存
	SetCamera();//设置相机
	DrawBoards();//绘制地板和边框
	DrawMeshs();//绘制面片
	glutSwapBuffers();
}

//全局定时器
void OnTimer(int value)
{
	glutPostRedisplay();//标记当前窗口需要重新绘制，调用myDisplay()
	glutTimerFunc(20, OnTimer, 1);
}


//交互函数集合
//处理鼠标点击 
void OnMouseClick(int button, int state, int x, int y)  
{
	if (state == GLUT_DOWN)
	{
		TheCamera.MouseDown(x, y);
	}
}

//处理鼠标拖动  
void OnMouseMove(int x, int y) 
{
	TheCamera.MouseMove(x, y);
}

//处理键盘点击（WASD）
void OnKeyClick(unsigned char key, int x, int y)
{
	int type = -1;
	if (key == 'w')
	{
		type = 0;
	}
	else if (key == 'a')
	{
		type = 1;
	}
	else if (key == 's')
	{
		type = 2;
	}
	else if (key == 'd')
	{
		type = 3;
	}
	TheCamera.KeyboardMove(type);
}

//处理键盘点击（前后左右）
void OnSpecialKeyClick(GLint key, GLint x, GLint y)
{
	int type = -1;
	if (key == GLUT_KEY_UP)
	{
		type = 0;
	}
	if (key == GLUT_KEY_LEFT)
	{
		type = 1;
	}
	if (key == GLUT_KEY_DOWN)
	{
		type = 2;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		type = 3;
	}
	TheCamera.KeyboardMove(type);
}


//reshape函数
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0f, (float)w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

//选择全局模式
void ChooseMode()
{
	while (1)
	{
		cout << "请输入要使用的模式" << endl;
		cout << "1：OpenGL光照模型" << endl;
		cout << "2：我的phong光照模型" << endl;
		int mode;
		cin >> mode;
		if (mode == OPENGL_LIGHT || mode == MY_LIGHT)
		{
			CurrentMode = mode;
			break;
		}
		else
		{
			cout << "输入不合法，请重新输入！" << endl;
			cout << "---------------------------------------" << endl;
		}
	}
	cout << "当前模式为";
	if (CurrentMode == OPENGL_LIGHT)
	{
		cout << "1：OpenGL光照模型" << endl;
	}
	else if (CurrentMode == MY_LIGHT)
	{
		cout << "2：我的phong光照模型" << endl;
	}
}

//光线追踪相关函数
/*
描述：光线追踪主函数
参数：光线，深度
返回：得到的颜色
*/
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
	if(min_id <= 0 || min_id >= 4) 
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



int main(int argc, char**argv)
{
	ChooseMode();
	glutInit(&argc, argv); 
	InitWindow();             //初始化窗口
	InitScene();              //初始化场景
	glutReshapeFunc(Reshape); //绑定reshape函数
	glutDisplayFunc(DrawScene); //绑定显示函数
	glutTimerFunc(20, OnTimer, 1);  //启动计时器
	glutMouseFunc(OnMouseClick); //绑定鼠标点击函数
	glutMotionFunc(OnMouseMove); //绑定鼠标移动函数
	glutKeyboardFunc(OnKeyClick);//绑定键盘点击函数
	glutSpecialFunc(OnSpecialKeyClick);//绑定特殊键盘点击函数
	glutMainLoop();
}
