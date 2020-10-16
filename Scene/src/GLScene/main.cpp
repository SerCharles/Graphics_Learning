#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include "Object.hpp"
#include "MovingBall.hpp"
#include "Camera.hpp"
#include "Board.hpp"
using namespace std;


float Stepk = 0.02;

float XRange = 10, ZRange = 10, Height = 20;
MovingBall BallA;
MovingBall BallB;
Camera TheCamera;
Board Boards[5];

//��ʼ������
void InitLight()
{

	//������ɫģʽ
	glShadeModel(GL_SMOOTH);
	//���ó�ʼ����ɫ�������ɫ�������Ȼ���
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// light source attributes
	GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat LightSpecular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat LightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//������ȼ�⣬��ֻ������ǰ���һ��
	glEnable(GL_DEPTH_TEST);
}

//��ʼ�����
void InitCamera()
{
	TheCamera.Init(10.0f, 10.0f);
}

//��ʼ������
void InitObjects()
{
	BallA.Init(7, -5, 1, 10, -6);
	BallB.Init(-5, 4, 1, 7, 10);
}

void InitBoards()
{
	Point DownA(-XRange, 0, -ZRange);
	Point DownB(-XRange, 0, ZRange);
	Point DownC(XRange, 0, -ZRange);
	Point DownD(XRange, 0, ZRange);
	Point UpA(-XRange, Height, -ZRange);
	Point UpB(-XRange, Height, ZRange);
	Point UpC(XRange, Height, -ZRange);
	Point UpD(XRange, Height, ZRange);
	Boards[0].Init(DownA, DownB, DownD, DownC);
	Boards[1].Init(DownA, DownB, UpB, UpA);
	Boards[2].Init(DownC, DownD, UpD, UpC);
	Boards[3].Init(DownA, DownC, UpC, UpA);
	Boards[4].Init(DownB, DownD, UpD, UpB);
}

void BoardDisplay(void)
{


	for (int i = 0; i < 5; i++)
	{
		
		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(Boards[i].PointList[0].x, Boards[i].PointList[0].y, Boards[i].PointList[0].z);
		glVertex3f(Boards[i].PointList[1].x, Boards[i].PointList[1].y, Boards[i].PointList[1].z);
		glVertex3f(Boards[i].PointList[2].x, Boards[i].PointList[2].y, Boards[i].PointList[2].z);
		glVertex3f(Boards[i].PointList[3].x, Boards[i].PointList[3].y, Boards[i].PointList[3].z);
		glEnd();
		glFlush();
	}
	
}

//��ʾ�ĺ���
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//����С��ǰ��λ��

	///////////////////

	glLoadIdentity();
	Point camera_place = TheCamera.CurrentPlace;//������ӵ������  
	gluLookAt(camera_place.x, camera_place.y, camera_place.z, 0, 0, 0, 0, 1, 0); //���ӵ㿴Զ��,y�᷽��(0,1,0)���Ϸ���  

	glColor3f(1.0f, 0.0f, 0.0f);

	//���õذ����
	GLfloat mat_ambient[] = { 0.021500, 0.174500, 0.021500, 0.550000 };
	GLfloat mat_diffuse[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
	GLfloat mat_specular[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
	GLfloat mat_shininess[] = { 76.800003 }; //����RGBA����ָ������ֵ��0��128��Χ��

	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	BoardDisplay();					//���Ƶذ�

	BallA.Move(Stepk);

	BallA.HandleCollisionBoard(XRange, ZRange);
	
	BallB.Move(Stepk);
	BallB.HandleCollisionBoard(XRange, ZRange);
	BallA.HandleCollisionBall(BallB);

	//�ƶ�����ǰС���λ�ã�����С��
	//����С�����
	GLfloat mat_ambient2[] = { 0.250000, 0.207250, 0.207250, 0.922000 };
	GLfloat mat_diffuse2[] = { 1.000000, 0.829000, 0.829000, 0.922000 };
	GLfloat mat_specular2[] = { 0.296648, 0.296648, 0.296648, 0.922000 };
	GLfloat mat_shininess2[] = { 11.264000 }; //����RGBA����ָ������ֵ��0��128��Χ��

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);

	Point place_a = BallA.CurrentPlace;
	float radius_a = BallA.Radius;
	glPushMatrix();
	glTranslatef(place_a.x, place_a.y, place_a.z);
	glutSolidSphere(radius_a, 40, 40);
	glPopMatrix();

	//����С�����
	GLfloat mat_ambient3[] = { 0.174500, 0.011750, 0.011750, 0.550000 };
	GLfloat mat_diffuse3[] = { 0.614240, 0.041360, 0.041360, 0.550000 };
	GLfloat mat_specular3[] = { 0.727811, 0.626959, 0.626959, 0.550000 };
	GLfloat mat_shininess3[] = { 76.800003 }; //����RGBA����ָ������ֵ��0��128��Χ��

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular3);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess3);

	Point place_b = BallB.CurrentPlace;
	float radius_b = BallB.Radius;
	glPushMatrix();
	glTranslatef(place_b.x, place_b.y, place_b.z);
	glutSolidSphere(radius_b, 40, 40);
	glPopMatrix();

	glutSwapBuffers();
	//////////////////
}

void OnTimer(int value)
{
	glutPostRedisplay();//��ǵ�ǰ������Ҫ���»��ƣ�����myDisplay()
	glutTimerFunc(20, OnTimer, 1);
}


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
	//���ڵ���ز����趨����������ʾ����
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("MyScene");

	
	InitLight();
	InitCamera();
	InitBoards();
	InitObjects();
	glutReshapeFunc(reshape); //��reshape����
	glutDisplayFunc(myDisplay); //����ʾ����
	glutTimerFunc(20, OnTimer, 1);  //�󶨼�ʱˢ�º���
	glutMouseFunc(OnMouseClick); //�����������
	glutMotionFunc(OnMouseMove); //������ƶ�����
	glutMainLoop();
}
