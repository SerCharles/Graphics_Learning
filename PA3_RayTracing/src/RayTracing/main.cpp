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
#define RAY_TRACE 2
#define RAY_TRACE_ACCELERATE 3


//ȫ�ֳ���
const int WindowSizeX = 800, WindowSizeY = 600, WindowPlaceX = 100, WindowPlaceY = 100;
const char WindowName[] = "MyScene";
const float TimeOnce = 0.02; //ˢ��ʱ��
const float XRange = 10, ZRange = 10, Height = 8, YFloor = 0; //������X,Y,Z��Χ��-X,X),(0,H),(-Z,Z)
const int BallComplexity = 40; //С����ƾ�ϸ�̶�
int CurrentMode = -1; //ģʽ��ģʽ1��OpenGL���գ�ģʽ2�ǹ���׷��


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
void InitGLLight()
{
	GLfloat background_color[3] = { 0.0, 0.0, 0.0 };
	GLfloat ambient[3] = { 1, 1, 1};
	GLfloat diffuse[3] = { 1, 1, 1};
	GLfloat specular[3] = { 1, 1, 1};
	GLfloat position[3] = { 0.0f, 10.0f, 0.0f};
	TheGLLight.Init(background_color, ambient, diffuse, specular, position);

	//������ɫģʽ
	glShadeModel(GL_SMOOTH);
	//���ó�ʼ����ɫ�������ɫ�������Ȼ���
	glClearColor(TheGLLight.Color[0], TheGLLight.Color[1], TheGLLight.Color[2], TheGLLight.Color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���ù�Դ��Ϣ
	glLightfv(GL_LIGHT0, GL_AMBIENT, TheGLLight.Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, TheGLLight.Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, TheGLLight.Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, TheGLLight.Position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
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
	GLfloat ambient_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat diffuse_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat specular_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat shininess_floor = 40;
	Floor.InitColor(color_floor, ambient_floor, diffuse_floor, specular_floor, shininess_floor);
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
		GLfloat shininess = 20;

		Bunny.InitPlace("bunny.ply", size, center);
		Bunny.InitColor(color, ambient, diffuse, specular, shininess);
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
		GLfloat shininess = 50;

		Dragon.InitPlace("dragon.ply", size, center);
		Dragon.InitColor(color, ambient, diffuse, specular, shininess);
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
		GLfloat shininess = 80;

		Happy.InitPlace("happy.ply", size, center);
		Happy.InitColor(color, ambient, diffuse, specular, shininess);
	}
}

//��ʼ����������
void InitScene()
{
	if (CurrentMode == OPENGL_LIGHT)
	{
		InitGLLight();
	}
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

//���Ʊ߽�
void DrawBoards()
{
	Floor.Draw();
}


//����mesh
void DrawMeshs()
{
	Bunny.Draw();
	Dragon.Draw();
	Happy.Draw();

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

//ѡ��ȫ��ģʽ
void ChooseMode()
{
	while (1)
	{
		cout << "������Ҫʹ�õ�ģʽ" << endl;
		cout << "1��OpenGL����ģ��" << endl;
		cout << "2������׷�ٻ���ģ��" << endl;
		cout << "3������׷�ټ���ģ��" << endl;
		int mode;
		cin >> mode;
		if (mode == OPENGL_LIGHT || mode == RAY_TRACE || mode == RAY_TRACE_ACCELERATE)
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
		cout << "1��OpenGL����ģ��" << endl;
	}
	else if (CurrentMode == RAY_TRACE)
	{
		cout << "2������׷�ٻ���ģ��" << endl;
	}
	else if (CurrentMode == RAY_TRACE_ACCELERATE)
	{
		cout << "3������׷�ټ���ģ��" << endl;
	}
}

//����׷����غ���



/*
����������׷��������
���������ߣ����
���أ��õ�����ɫ
*/
Color RayTracing(Ray& the_ray, int depth)
{
	if (depth > MaxDepth)
	{
		return Color(0.0, 0.0, 0.0);
	}
	
	//�����������󽻵õ������t
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

	//�󽻵㣬������ߣ���ɫ
	Point intersection;
	Ray reflection;
	Color color;
	if (min_id == 0)
	{
		//TODO ���㴦��ǿ��ɶ��
		RectangleMesh the_rectangle = Floor.RectangleList[i_floor][j_floor];
		color.R = the_rectangle.Color[0];
		color.G = the_rectangle.Color[1];
		color.B = the_rectangle.Color[2];

		reflection = the_ray.GetReflection(Floor, i_floor, j_floor, t_floor);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 1)
	{
		//TODO ���㴦��ǿ��ɶ��
		TriangleMesh the_triangle = Bunny.Faces[i_bunny];
		color.R = the_triangle.Color[0];
		color.G = the_triangle.Color[1];
		color.B = the_triangle.Color[2];

		reflection = the_ray.GetReflection(Bunny, i_bunny, t_bunny);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 2)
	{
		//TODO ���㴦��ǿ��ɶ��
		TriangleMesh the_triangle = Dragon.Faces[i_dragon];
		color.R = the_triangle.Color[0];
		color.G = the_triangle.Color[1];
		color.B = the_triangle.Color[2];

		reflection = the_ray.GetReflection(Dragon, i_dragon, t_dragon);
		intersection = reflection.StartPlace;
	}
	else if (min_id == 3)
	{
		//TODO ���㴦��ǿ��ɶ��
		TriangleMesh the_triangle = Happy.Faces[i_happy];
		color.R = the_triangle.Color[0];
		color.G = the_triangle.Color[1];
		color.B = the_triangle.Color[2];

		reflection = the_ray.GetReflection(Happy, i_happy, t_happy);
		intersection = reflection.StartPlace;
	}

	Color color_reflection = RayTracing(reflection, depth + 1);
	Color result = color + color_reflection * ks;
	return result;
}



int main(int argc, char**argv)
{
	ChooseMode();
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
