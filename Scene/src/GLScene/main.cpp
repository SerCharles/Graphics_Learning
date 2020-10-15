#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include "Object.hpp"
#include "MovingBall.hpp"
#include "Camera.hpp"
using namespace std;


float Stepk = 0.02;

float GroundX = 10;	//ƽ�壨���棩��X��Z�����ϵĳߴ�
float GroundZ = 10;
MovingBall A;
MovingBall B;
Camera c;

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
	c.Init(10.0f, 10.0f);
}

//��ʼ������
void InitObjects()
{
	A.Init(10, -6, 1, 0, 0);
	B.Init(7, 10, 1, 0, 0);
}

void BoardDisplay(void)
{
	glColor3f(0.5, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-GroundX, 0, GroundZ);
	glVertex3f(-GroundX, 0, -GroundZ);
	glVertex3f(GroundX, 0, -GroundZ);
	glVertex3f(GroundX, 0, GroundZ);
	glEnd();
	glFlush();
}

//��ʾ�ĺ���
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//����С��ǰ��λ��

	///////////////////

	glLoadIdentity();
	Point camera_place = c.CurrentPlace;//������ӵ������  
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

	A.Move(Stepk);
	B.Move(Stepk);
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

	Point place_a = A.CurrentPlace;
	float radius_a = A.Radius;
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

	Point place_b = B.CurrentPlace;
	float radius_b = B.Radius;
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
		c.MouseDown(x, y);
	}
}


//��������϶�  
void OnMouseMove(int x, int y) 
{
	c.MouseMove(x, y);
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
	InitObjects();
	glutReshapeFunc(reshape); //��reshape����
	glutDisplayFunc(myDisplay); //����ʾ����
	glutTimerFunc(20, OnTimer, 1);  //�󶨼�ʱˢ�º���
	glutMouseFunc(OnMouseClick); //�����������
	glutMotionFunc(OnMouseMove); //������ƶ�����
	glutMainLoop();
}
