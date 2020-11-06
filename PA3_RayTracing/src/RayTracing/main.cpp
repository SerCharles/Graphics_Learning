#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include "Point.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Board.hpp"
#include "TriMesh.hpp"
using namespace std;






//ȫ�ֳ���
const int WindowSizeX = 800, WindowSizeY = 600, WindowPlaceX = 100, WindowPlaceY = 100;
const char WindowName[] = "MyScene";
const float TimeOnce = 0.02; //ˢ��ʱ��
const float XRange = 10, ZRange = 10, Height = 8; //������X,Y,Z��Χ��-X,X),(0,H),(-Z,Z)
const int BallComplexity = 40; //С����ƾ�ϸ�̶�

//���գ����
Camera TheCamera;
Light TheLight;

//����
Board BoardA; //�߽�
//��Ƭ����
TriMesh Bunny, Dragon, Happy;


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

//��ʼ������
void InitLight()
{
	GLfloat background_color[3] = { 0.0, 0.0, 0.0 };
	GLfloat ambient[3] = { 1, 1, 1};
	GLfloat diffuse[3] = { 1, 1, 1};
	GLfloat specular[3] = { 1, 1, 1};
	GLfloat position[3] = { 0.0f, 10.0f, 0.0f};
	TheLight.Init(background_color, ambient, diffuse, specular, position);

	//������ɫģʽ
	glShadeModel(GL_SMOOTH);
	//���ó�ʼ����ɫ�������ɫ�������Ȼ���
	glClearColor(TheLight.Color[0], TheLight.Color[1], TheLight.Color[2], TheLight.Color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���ù�Դ��Ϣ
	glLightfv(GL_LIGHT0, GL_AMBIENT, TheLight.Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, TheLight.Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, TheLight.Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, TheLight.Position);
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
	//4����
	Point DownA(-XRange, 0, -ZRange);
	Point DownB(-XRange, 0, ZRange);
	Point DownC(XRange, 0, -ZRange);
	Point DownD(XRange, 0, ZRange);

	//���õذ�͵���λ��
	BoardA.InitPlace(DownA, DownB, DownD, DownC);

	//�ذ����
	GLfloat color_floor[3] = { 1.0, 1.0, 1.0 };
	GLfloat ambient_floor[3] = { 0.4, 0.4, 0.4};
	GLfloat diffuse_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat specular_floor[3] = { 0.4, 0.4, 0.4};
	GLfloat shininess_floor = 90;
	BoardA.InitColor(color_floor, ambient_floor, diffuse_floor, specular_floor, shininess_floor);

	//�������ܵ������
	GLfloat color_border[3] = { 1.0, 1.0, 1.0 };
	GLfloat ambient_border[3] = { 0.2, 0.2, 0.2};
	GLfloat diffuse_border[3] = { 0.2, 0.2, 0.2};
	GLfloat specular_border[3] = { 0.2, 0.2, 0.2};
	GLfloat shininess_border = 40;

	BoardA.InitColor(color_border, ambient_border, diffuse_border, specular_border, shininess_border);
	
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
		GLfloat shininess = 40;

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
		GLfloat shininess = 60;

		Happy.InitPlace("happy.ply", size, center);
		Happy.InitColor(color, ambient, diffuse, specular, shininess);
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

//���Ʊ߽�
void DrawBoards()
{
	BoardA.Draw();
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
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0f, (float)w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char**argv)
{
	
	glutInit(&argc, argv); 
	InitWindow();             //��ʼ������
	InitScene();              //��ʼ������
	glutReshapeFunc(reshape); //��reshape����
	glutDisplayFunc(DrawScene); //����ʾ����
	glutTimerFunc(20, OnTimer, 1);  //������ʱ��
	glutMouseFunc(OnMouseClick); //�����������
	glutMotionFunc(OnMouseMove); //������ƶ�����
	glutKeyboardFunc(OnKeyClick);//�󶨼��̵������
	glutSpecialFunc(OnSpecialKeyClick);//��������̵������
	glutMainLoop();
}
