#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include "Object.hpp"
#include "MovingBall.hpp"
#include "Camera.hpp"
using namespace std;


float Stepk = 0.02;

float GroundX = 10;	//平板（桌面）在X和Z方向上的尺寸
float GroundZ = 10;
MovingBall A;
MovingBall B;
Camera c;

//初始化光照
void InitLight()
{

	//设置着色模式
	glShadeModel(GL_SMOOTH);
	//设置初始背景色，清除颜色缓存和深度缓存
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

	//设置深度检测，即只绘制最前面的一层
	glEnable(GL_DEPTH_TEST);
}

//初始化相机
void InitCamera()
{
	c.Init(10.0f, 10.0f);
}

//初始化物体
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

//显示的函数
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//计算小球当前的位置

	///////////////////

	glLoadIdentity();
	Point camera_place = c.CurrentPlace;//这就是视点的坐标  
	gluLookAt(camera_place.x, camera_place.y, camera_place.z, 0, 0, 0, 0, 1, 0); //从视点看远点,y轴方向(0,1,0)是上方向  

	glColor3f(1.0f, 0.0f, 0.0f);

	//设置地板材质
	GLfloat mat_ambient[] = { 0.021500, 0.174500, 0.021500, 0.550000 };
	GLfloat mat_diffuse[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
	GLfloat mat_specular[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
	GLfloat mat_shininess[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	BoardDisplay();					//绘制地板

	A.Move(Stepk);
	B.Move(Stepk);
	//移动到当前小球的位置，画出小球
	//设置小球材质
	GLfloat mat_ambient2[] = { 0.250000, 0.207250, 0.207250, 0.922000 };
	GLfloat mat_diffuse2[] = { 1.000000, 0.829000, 0.829000, 0.922000 };
	GLfloat mat_specular2[] = { 0.296648, 0.296648, 0.296648, 0.922000 };
	GLfloat mat_shininess2[] = { 11.264000 }; //材质RGBA镜面指数，数值在0～128范围内

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

	//设置小球材质
	GLfloat mat_ambient3[] = { 0.174500, 0.011750, 0.011750, 0.550000 };
	GLfloat mat_diffuse3[] = { 0.614240, 0.041360, 0.041360, 0.550000 };
	GLfloat mat_specular3[] = { 0.727811, 0.626959, 0.626959, 0.550000 };
	GLfloat mat_shininess3[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内

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
	glutPostRedisplay();//标记当前窗口需要重新绘制，调用myDisplay()
	glutTimerFunc(20, OnTimer, 1);
}


//处理鼠标点击 
void OnMouseClick(int button, int state, int x, int y)  
{
	if (state == GLUT_DOWN)
	{
		c.MouseDown(x, y);
	}
}


//处理鼠标拖动  
void OnMouseMove(int x, int y) 
{
	c.MouseMove(x, y);
}

//reshape函数
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
	//窗口的相关参数设定，并创建显示窗口
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("MyScene");

	
	InitLight();
	InitCamera();
	InitObjects();
	glutReshapeFunc(reshape); //绑定reshape函数
	glutDisplayFunc(myDisplay); //绑定显示函数
	glutTimerFunc(20, OnTimer, 1);  //绑定计时刷新函数
	glutMouseFunc(OnMouseClick); //绑定鼠标点击函数
	glutMotionFunc(OnMouseMove); //绑定鼠标移动函数
	glutMainLoop();
}
