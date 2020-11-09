#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <time.h>
#include "Point.hpp"
#include "GLLight.hpp"
#include "Camera.hpp"
#include "Board.hpp"
#include "Ray.hpp"
#include "MeshModel.hpp"
#include "PhongModel.hpp"
using namespace std;

#define OPENGL_LIGHT 1
#define MY_LIGHT 2
#define RAY_TRACE 3
#define RAY_TRACE_ACCELERATE 4

//ȫ�ֳ���
const int WindowSizeX = 800, WindowSizeY = 600, WindowPlaceX = 100, WindowPlaceY = 100;
const char WindowName[] = "MyScene";
const float XRange = 10, ZRange = 10, Height = 8, YFloor = 0; //������X,Y,Z��Χ��-X,X),(0,H),(-Z,Z)
int CurrentMode = -1; //ģʽ��ģʽ1��OpenGL���գ�ģʽ2�ǹ���׷��
float GlobalRefractionRate = 1;

//���գ����
Camera TheCamera;
GLLight TheGLLight;

//����
Board Floor; //�߽�
//��Ƭ����
MeshModel Bunny, Dragon, Happy;


//��ʼ����������
//��ʼ������
void InitWindow()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WindowSizeX, WindowSizeY);
	glutInitWindowPosition(WindowPlaceX, WindowPlaceY);
	glutCreateWindow(WindowName);
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION);
	const GLubyte* gluVersion = gluGetString(GLU_VERSION);
	printf("OpenGLʵ�ֵİ汾�ţ�%s\n", OpenGLVersion);
	printf("OGLU���߿�汾��%s\n", gluVersion);
}

//��ʼ��OpenGL����
void InitLight()
{
	GLfloat background_color[3] = { 0.0, 0.0, 0.0 };
	GLfloat ambient[3] = { 1, 1, 1 };
	GLfloat diffuse[3] = { 1, 1, 1 };
	GLfloat specular[3] = { 1, 1, 1 };
	GLfloat position[3] = { 0.0f, 10.0f, 0.0f };
	TheGLLight.Init(background_color, ambient, diffuse, specular, position);

	//������ɫģʽ
	glShadeModel(GL_SMOOTH);
	//���ó�ʼ����ɫ�������ɫ�������Ȼ���
	glClearColor(TheGLLight.Color[0], TheGLLight.Color[1], TheGLLight.Color[2], TheGLLight.Color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (CurrentMode == OPENGL_LIGHT)
	{
		//���ù�Դ��Ϣ
		glLightfv(GL_LIGHT0, GL_AMBIENT, TheGLLight.Ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, TheGLLight.Diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, TheGLLight.Specular);
		glLightfv(GL_LIGHT0, GL_POSITION, TheGLLight.Position);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	//������ȼ�⣬��ֻ������ǰ���һ��
	glEnable(GL_DEPTH_TEST);
}

//��ʼ�����
void InitCamera()
{
	//���ó�ʼ���λ��
	TheCamera.Init(10.0f, 10.0f);
}

//��ʼ���߽�͵ذ�
void InitBoards()
{
	//���õذ�͵���λ��
	Floor.InitPlace(XRange, -XRange, ZRange, -ZRange, YFloor);

	//�ذ����
	GLfloat color_floor[3] = { 1.0, 1.0, 1.0 };
	GLfloat ambient_floor[3] = { 0.2, 0.2, 0.2 };
	GLfloat diffuse_floor[3] = { 0.2, 0.2, 0.2 };
	GLfloat specular_floor[3] = { 0.2, 0.2, 0.2 };
	GLfloat shininess_floor = 40;
	float k_reflection = 0.2;
	float k_refraction = 0;
	float refraction_rate = 1;
	Floor.InitColor(color_floor, ambient_floor, diffuse_floor, specular_floor, shininess_floor, k_reflection, k_refraction, refraction_rate);
}

//��ʼ����Ƭ
void InitMeshs()
{
	//����
	{
		//��Ƭ�Ĵ�С��λ��
		float size = 2;
		Point center = Point(0, 4, 4);

		//��Ƭ���������ʣ���ɫ
		GLfloat color[3] = { 0.0, 0.0, 1.0 };
		GLfloat ambient[3] = { 0.2, 0.2, 0.6 };
		GLfloat diffuse[3] = { 0.4, 0.4, 0.4 };
		GLfloat specular[3] = { 0.2, 0.2, 0.2 };
		GLfloat shininess = 100;
		float k_reflection = 0.2;
		float k_refraction = 0.1;
		float refraction_rate = 1.4;

		Bunny.InitPlace("bunny.ply", size, center);
		Bunny.InitColor(color, ambient, diffuse, specular, shininess, k_reflection, k_refraction, refraction_rate);
	}

	//��
	{
		//��Ƭ�Ĵ�С��λ��
		float size = 2;
		Point center = Point(-5, 5, -3);

		//��Ƭ���������ʣ���ɫ
		GLfloat color[3] = { 1.0, 0.0, 0.0 };
		GLfloat ambient[3] = { 0.6, 0.2, 0.2 };
		GLfloat diffuse[3] = { 0.4, 0.4, 0.4 };
		GLfloat specular[3] = { 0.2, 0.2, 0.2 };
		GLfloat shininess = 100;
		float k_reflection = 0.3;
		float k_refraction = 0.1;
		float refraction_rate = 1.6;

		Dragon.InitPlace("dragon.ply", size, center);
		Dragon.InitColor(color, ambient, diffuse, specular, shininess, k_reflection, k_refraction, refraction_rate);
	}

	//��
	{
		//��Ƭ�Ĵ�С��λ��
		float size = 2;
		Point center = Point(5, 4, -3);

		//��Ƭ���������ʣ���ɫ
		GLfloat color[3] = { 1.0, 1.0, 0.0 };
		GLfloat ambient[3] = { 0.6, 0.6, 0.2 };
		GLfloat diffuse[3] = { 0.4, 0.4, 0.4 };
		GLfloat specular[3] = { 0.2, 0.2, 0.2 };
		GLfloat shininess = 100;
		float k_reflection = 0.6;
		float k_refraction = 0.9;
		float refraction_rate = 1.8;

		Happy.InitPlace("happy.ply", size, center);
		Happy.InitColor(color, ambient, diffuse, specular, shininess, k_reflection, k_refraction, refraction_rate);
	}
}

//��ʼ����������
void InitScene()
{
	InitLight();
	InitCamera();
	InitBoards();
	InitMeshs();
}

//���ƺ�������
//�������λ��
void SetCamera()
{
	glLoadIdentity();
	Point camera_place = TheCamera.CurrentPlace;//������ӵ������  
	Point camera_center = TheCamera.LookCenter;//�����ӵ���������
	gluLookAt(camera_place.x, camera_place.y, camera_place.z, camera_center.x, camera_center.y, camera_center.z, 0, 1, 0); //���ӵ㿴Զ��,y�᷽��(0,1,0)���Ϸ���  
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

//���Ʊ߽�
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

//����mesh
void DrawMeshs()
{
	DrawOneMesh(Bunny);
	DrawOneMesh(Dragon);
	DrawOneMesh(Happy);
}

//���Ƶ�������
void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//�����ɫ����
	SetCamera();//�������
	DrawBoards();//���Ƶذ�ͱ߿�
	DrawMeshs();//������Ƭ
	glutSwapBuffers();
}

//ȫ�ֶ�ʱ��
void OnTimer(int value)
{
	glutPostRedisplay();//��ǵ�ǰ������Ҫ���»��ƣ�����myDisplay()
	glutTimerFunc(20, OnTimer, 1);
}


//������������
//��������� 
void OnMouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		TheCamera.MouseDown(x, y);
	}
}

//��������϶�  
void OnMouseMove(int x, int y)
{
	TheCamera.MouseMove(x, y);
}

//������̵����WASD��
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

//������̵����ǰ�����ң�
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

//reshape����
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0f, (float)w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}




//����׷����غ���
const int GridX = 500;
const int GridY = 500;
const float LengthX = 20;
const float LengthY = 20;
const int MaxDepth = 6;
Color Result[GridX][GridY];
/*
����������׷��������
���������ߣ����
���أ��õ�����ɫ
*/
Color RayTracing(Ray& the_ray, int depth)
{
	const float Threshold = 0.01;

	if (depth > MaxDepth || the_ray.Intensity <= Threshold)
	{
		return Color(0.0, 0.0, 0.0);
	}

	//�����������󽻵õ������t
	vector<float> t_list;
	t_list.clear();
	int i_floor = -1;
	int j_floor = -1;
	float t_floor = -1;

	the_ray.GetIntersection(Floor, i_floor, j_floor, t_floor);
	t_list.push_back(t_floor);

	int i_bunny = -1;
	float t_bunny = -1;
	if (CurrentMode == RAY_TRACE)
	{
		the_ray.GetIntersection(Bunny, i_bunny, t_bunny);
	}
	else if (CurrentMode == RAY_TRACE_ACCELERATE)
	{
		the_ray.GetIntersectionFast(Bunny, i_bunny, t_bunny);
	}
	t_list.push_back(t_bunny);
	
	int i_dragon = -1;
	float t_dragon = -1;
	if (CurrentMode == RAY_TRACE)
	{
		the_ray.GetIntersection(Dragon, i_dragon, t_dragon);
	}
	else if (CurrentMode == RAY_TRACE_ACCELERATE)
	{
		the_ray.GetIntersectionFast(Dragon, i_dragon, t_dragon);
	}
	t_list.push_back(t_dragon);

	int i_happy = -1;
	float t_happy = -1;
	if (CurrentMode == RAY_TRACE)
	{
		the_ray.GetIntersection(Happy, i_happy, t_happy);
	}
	else if (CurrentMode == RAY_TRACE_ACCELERATE)
	{
		the_ray.GetIntersectionFast(Happy, i_happy, t_happy);
	}
	t_list.push_back(t_happy);
	
	int min_id = GetSmallestNum(t_list);
	if (min_id < 0)
	{
		return Color(0.0, 0.0, 0.0);
	}

	//�󽻵㣬������ߣ�������ߣ���ɫ
	Point intersection;
	Ray reflection;
	Ray refraction;
	Color color;
	float k_reflection = 0.0;
	float k_refraction = 0.0;
	if (min_id == 0)
	{
		RectangleMesh the_rectangle = Floor.RectangleList[i_floor][j_floor];
		k_reflection = the_rectangle.KReflection;
		k_refraction = the_rectangle.KRefraction;
		color = PhongModel(TheGLLight, TheCamera, the_rectangle);
		reflection = the_ray.GetReflection(Floor, i_floor, j_floor, t_floor);
		refraction = the_ray.GetRefraction(Floor, i_floor, j_floor, t_floor);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 1)
	{
		TriangleMesh the_triangle = Bunny.Faces[i_bunny];
		k_reflection = the_triangle.KReflection;
		k_refraction = the_triangle.KRefraction;
		color = PhongModel(TheGLLight, TheCamera, the_triangle);
		reflection = the_ray.GetReflection(Bunny, i_bunny, t_bunny);
		refraction = the_ray.GetRefraction(Bunny, i_bunny, t_bunny);
		intersection = reflection.StartPlace;
	}
	
	else if (min_id == 2)
	{
		TriangleMesh the_triangle = Dragon.Faces[i_dragon];
		k_reflection = the_triangle.KReflection;
		k_refraction = the_triangle.KRefraction;
		color = PhongModel(TheGLLight, TheCamera, the_triangle);
		reflection = the_ray.GetReflection(Dragon, i_dragon, t_dragon);
		refraction = the_ray.GetRefraction(Dragon, i_dragon, t_dragon);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 3)
	{
		TriangleMesh the_triangle = Happy.Faces[i_happy];
		k_reflection = the_triangle.KReflection;
		k_refraction = the_triangle.KRefraction;
		color = PhongModel(TheGLLight, TheCamera, the_triangle);
		reflection = the_ray.GetReflection(Happy, i_happy, t_happy);
		refraction = the_ray.GetRefraction(Happy, i_happy, t_happy);
		intersection = reflection.StartPlace;
	}

	Color color_reflection = RayTracing(reflection, depth + 1);
	Color color_refraction = RayTracing(refraction, depth + 1);
	Color result = color + color_reflection * k_reflection + color_refraction * k_refraction;
	return result;
}

void InitCameraRayTrace(Point& x_axis, Point& y_axis, Point& z_axis)
{
	TheCamera.CurrentPlace.SetPlace(10, 10, 0);
	TheCamera.LookCenter.SetPlace(0, 0, 0);
	x_axis.SetPlace(1, -1, -sqrt(2));
	y_axis.SetPlace(-1, 1, -sqrt(2));
	z_axis.SetPlace(-1, -1, 0);
	x_axis.Normalize();
	y_axis.Normalize();
	z_axis.Normalize();

}

//����׷��������
void RayTracingMain()
{
	clock_t start, end;
	start = clock();
	cout << "��ʱ��ʼ" << endl;

	//������-�ӵ��Ӧ�����꣬�Լ�ÿ�����ص�λ��
	float dist_x = LengthX / GridX;
	float dist_y = LengthY / GridY;
	Point x_axis;
	Point y_axis;
	Point z_axis;
	InitCameraRayTrace(x_axis, y_axis, z_axis);
	Point half_x = x_axis * (LengthX / 2);
	Point half_y = y_axis * (LengthY / 2);
	Point base = TheCamera.CurrentPlace - half_x - half_y;

	//����ÿ�����ط��⣬����׷��
	for (int i = 0; i < GridX; i++)
	{
		Point dx = x_axis * (dist_x * i);
		for (int j = 0; j < GridY; j++) 
		{
			Point dy = y_axis * (dist_y * j);
			Point start = base + dx + dy;
			Ray the_ray;
			the_ray.Init(start, z_axis, 1, GlobalRefractionRate);
			Color the_color = RayTracing(the_ray, 1);
			Result[i][j] = the_color;
		}
	}

	end = clock();
	cout << "��ʱ������һ����ʱ" << double(end - start) / CLOCKS_PER_SEC << "s" << endl;
}

//����׷�ٵ�opengl��ʾ
void RayTracingDisplay()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(TheGLLight.Color[0], TheGLLight.Color[1], TheGLLight.Color[2], TheGLLight.Color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, 18, 0, 1, 0); //���ӵ㿴Զ��,y�᷽��(0,1,0)���Ϸ���  
	Point base = Point(-LengthX / 2, -LengthY / 2, 18);
	float dist_x = LengthX / GridX;
	float dist_y = LengthY / GridY;

	//����ÿ�����ط��⣬����׷��
	for (int i = 0; i < GridX; i++)
	{
		for (int j = 0; j < GridY; j++)
		{
			Point v1 = Point(base.x + i * dist_x, base.y + j * dist_y, base.z);
			Point v2 = Point(v1.x + dist_x, v1.y, v1.z);
			Point v3 = Point(v1.x + dist_x, v1.y + dist_y, v1.z);
			Point v4 = Point(v1.x, v1.y + dist_y, v1.z);
			Color color = Result[i][j];
			glColor3f(color.R, color.G, color.B);
			DrawRectangle(v1, v2, v3, v4);
		}
	}
	glutSwapBuffers();
}

//ѡ��ȫ��ģʽ
void ChooseModeGL()
{
	while (1)
	{
		cout << "������Ҫʹ�õ�ģʽ" << endl;
		cout << "1��OpenGL����ģ��" << endl;
		cout << "2���ҵ�phong����ģ��" << endl;
		cout << "3������׷��" << endl;
		cout << "4�����ٹ���׷��" << endl;
		int mode;
		cin >> mode;
		if (mode == OPENGL_LIGHT || mode == MY_LIGHT || mode == RAY_TRACE || mode == RAY_TRACE_ACCELERATE)
		{
			CurrentMode = mode;
			break;
		}
		else
		{
			cout << "���벻�Ϸ������������룡" << endl;
			cout << "---------------------------------------" << endl;
		}
	}
	cout << "��ǰģʽΪ";
	if (CurrentMode == OPENGL_LIGHT)
	{
		cout << "OpenGL����ģ��" << endl;
	}
	else if (CurrentMode == MY_LIGHT)
	{
		cout << "�ҵ�phong����ģ��" << endl;
	}
	else if (CurrentMode == RAY_TRACE)
	{
		cout << "����׷��" << endl;
	}
	else if (CurrentMode == RAY_TRACE_ACCELERATE)
	{
		cout << "���ٹ���׷��" << endl;
	}
}

int main(int argc, char**argv)
{
	ChooseModeGL();
	if (CurrentMode == MY_LIGHT || CurrentMode == OPENGL_LIGHT)
	{
		glutInit(&argc, argv);
		InitWindow();             //��ʼ������
		InitScene();              //��ʼ������
		glutReshapeFunc(Reshape); //��reshape����
		glutDisplayFunc(DrawScene); //����ʾ����
		glutTimerFunc(20, OnTimer, 1);  //������ʱ��
		glutMouseFunc(OnMouseClick); //�����������
		glutMotionFunc(OnMouseMove); //������ƶ�����
		glutKeyboardFunc(OnKeyClick);//�󶨼��̵������
		glutSpecialFunc(OnSpecialKeyClick);//��������̵������
		glutMainLoop();
	}
	else
	{
		glutInit(&argc, argv);
		InitWindow();             //��ʼ������
		InitScene();              //��ʼ������
		RayTracingMain();
		glutReshapeFunc(Reshape); //��reshape����
		glutTimerFunc(20, OnTimer, 1);  //������ʱ��
		glutDisplayFunc(RayTracingDisplay); //����ʾ����
		glutMainLoop();
	}
	return 0;
}
