#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include "Point.hpp"
#include "MovingBall.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Board.hpp"
#include "Static.hpp"
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
Board Boards[5]; //�߽�
//С��
MovingBall BallA; 
MovingBall BallB;
//��̬������
Static Statics[4];



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
	//8����
	Point DownA(-XRange, 0, -ZRange);
	Point DownB(-XRange, 0, ZRange);
	Point DownC(XRange, 0, -ZRange);
	Point DownD(XRange, 0, ZRange);
	Point UpA(-XRange, Height, -ZRange);
	Point UpB(-XRange, Height, ZRange);
	Point UpC(XRange, Height, -ZRange);
	Point UpD(XRange, Height, ZRange);

	//���õذ�͵���λ��
	Boards[0].InitPlace(DownA, DownB, DownD, DownC);
	Boards[1].InitPlace(DownA, DownB, UpB, UpA);
	Boards[2].InitPlace(DownC, DownD, UpD, UpC);
	Boards[3].InitPlace(DownA, DownC, UpC, UpA);
	Boards[4].InitPlace(DownB, DownD, UpD, UpB);

	//�ذ����
	GLfloat color_floor[3] = { 1.0, 1.0, 1.0 };
	GLfloat ambient_floor[3] = { 0.4, 0.4, 0.4};
	GLfloat diffuse_floor[3] = { 0.2, 0.2, 0.2};
	GLfloat specular_floor[3] = { 0.4, 0.4, 0.4};
	GLfloat shininess_floor = 90;
	Boards[0].InitColor(color_floor, ambient_floor, diffuse_floor, specular_floor, shininess_floor);

	//�������ܵ������
	GLfloat color_border[3] = { 1.0, 1.0, 1.0 };
	GLfloat ambient_border[3] = { 0.2, 0.2, 0.2};
	GLfloat diffuse_border[3] = { 0.2, 0.2, 0.2};
	GLfloat specular_border[3] = { 0.2, 0.2, 0.2};
	GLfloat shininess_border = 40;
	for (int i = 1; i < 5; i++)
	{
		Boards[i].InitColor(color_border, ambient_border, diffuse_border, specular_border, shininess_border);
	}
}

//��ʼ����̬����
void InitStatics()
{
	float LargeRadius = 2;
	float SmallRadius = 1;
	float DownHeight = 2;
	float UpHeight = 4;
	Statics[0].InitPlace(LargeRadius, SmallRadius, DownHeight, UpHeight, -XRange + LargeRadius, 0, -ZRange + LargeRadius);
	Statics[1].InitPlace(LargeRadius, SmallRadius, DownHeight, UpHeight, -XRange + LargeRadius, 0, ZRange - LargeRadius);
	Statics[2].InitPlace(LargeRadius, SmallRadius, DownHeight, UpHeight, XRange - LargeRadius, 0, ZRange - LargeRadius);
	Statics[3].InitPlace(LargeRadius, SmallRadius, DownHeight, UpHeight, XRange - LargeRadius, 0, -ZRange + LargeRadius);
	
	
	//С��A�����������ʣ���ɫ
	GLfloat color[3] = { 1.0, 1.0, 0.0 };
	GLfloat ambient[3] = { 0.4, 0.4, 0.2 };
	GLfloat diffuse[3] = { 1, 0, 0.8 };
	GLfloat specular[3] = { 0.5, 0.5, 0.3 };
	GLfloat shininess = 60;
	for (int i = 0; i < 4; i++)
	{
		Statics[i].InitColor(color, ambient, diffuse, specular, shininess);
	}
}

//��ʼ��С��
void InitMovingBalls()
{
	//С��A��λ�ã��ٶ�
	float radius_a = 1;
	Point place_a = Point(3, 0, -3);
	Point speed_a = Point(10, 0, -6);

	//С��A�����������ʣ���ɫ
	GLfloat color_a[3] = { 1.0, 0.0, 0.0 };
	GLfloat ambient_a[3] = { 0.4, 0.2, 0.2 };
	GLfloat diffuse_a[3] = { 1, 0.8, 0.8 };
	GLfloat specular_a[3] = { 0.5, 0.3, 0.3 };
	GLfloat shininess_a = 10;

	//��ʼ��С��A
	BallA.InitPlace(place_a.x, place_a.z, radius_a, speed_a.x, speed_a.z);
	BallA.InitColor(color_a, ambient_a, diffuse_a, specular_a, shininess_a);

	//С��B��λ�ã��ٶ�
	float radius_b = 1;
	Point place_b = Point(-3, 0, -3);
	Point speed_b = Point(7, 0, 10);

	//С��B�����������ʣ���ɫ
	GLfloat color_b[3] = { 0.0, 0.0, 1.0 };
	GLfloat ambient_b[3] = { 0.2, 0.2, 0.4 };
	GLfloat diffuse_b[3] = { 0.3, 0.3, 0.6 };
	GLfloat specular_b[3] = { 0.8, 0.8, 1.0 };
	GLfloat shininess_b = 80;

	//��ʼ��С��B
	BallB.InitPlace(place_b.x, place_b.z, radius_b, speed_b.x, speed_b.z);
	BallB.InitColor(color_b, ambient_b, diffuse_b, specular_b, shininess_b);
}

//��ʼ����������
void InitScene()
{

	InitLight();
	InitCamera();
	InitBoards();
	InitStatics();
	InitMovingBalls();
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

//���Ʊ߽�͵ذ�
void DrawBoards()
{
	for (int i = 0; i < 5; i++)
	{
		glColor3f(Boards[i].Color[0], Boards[i].Color[1], Boards[i].Color[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Boards[i].Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Boards[i].Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Boards[i].Specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Boards[i].Shininess);


		glBegin(GL_POLYGON);
		glVertex3f(Boards[i].PointList[0].x, Boards[i].PointList[0].y, Boards[i].PointList[0].z);
		glVertex3f(Boards[i].PointList[1].x, Boards[i].PointList[1].y, Boards[i].PointList[1].z);
		glVertex3f(Boards[i].PointList[2].x, Boards[i].PointList[2].y, Boards[i].PointList[2].z);
		glVertex3f(Boards[i].PointList[3].x, Boards[i].PointList[3].y, Boards[i].PointList[3].z);
		glEnd();
		glFlush();
	}
	
}


void DrawPolygon(Point a, Point b, Point c, Point d)
{
	glBegin(GL_POLYGON);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glVertex3f(d.x, d.y, d.z);
	glEnd();
}

//���ƾ�̬����
void DrawStatics()
{
	
	for (int i = 0; i < 4; i++)
	{
		glColor3f(Statics[i].Color[0], Statics[i].Color[1], Statics[i].Color[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Statics[i].Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Statics[i].Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Statics[i].Specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Statics[i].Shininess);


		DrawPolygon(Statics[i].BottomPoints[0], Statics[i].BottomPoints[1], Statics[i].MiddlePoints[1], Statics[i].MiddlePoints[0]);
		DrawPolygon(Statics[i].BottomPoints[1], Statics[i].BottomPoints[2], Statics[i].MiddlePoints[2], Statics[i].MiddlePoints[1]);
		DrawPolygon(Statics[i].BottomPoints[2], Statics[i].BottomPoints[3], Statics[i].MiddlePoints[3], Statics[i].MiddlePoints[2]);
		DrawPolygon(Statics[i].BottomPoints[3], Statics[i].BottomPoints[0], Statics[i].MiddlePoints[0], Statics[i].MiddlePoints[3]);

		DrawPolygon(Statics[i].UpPoints[0], Statics[i].UpPoints[1], Statics[i].MiddlePoints[1], Statics[i].MiddlePoints[0]);
		DrawPolygon(Statics[i].UpPoints[1], Statics[i].UpPoints[2], Statics[i].MiddlePoints[2], Statics[i].MiddlePoints[1]);
		DrawPolygon(Statics[i].UpPoints[2], Statics[i].UpPoints[3], Statics[i].MiddlePoints[3], Statics[i].MiddlePoints[2]);
		DrawPolygon(Statics[i].UpPoints[3], Statics[i].UpPoints[0], Statics[i].MiddlePoints[0], Statics[i].MiddlePoints[3]);
	
		DrawPolygon(Statics[i].UpPoints[0], Statics[i].UpPoints[1], Statics[i].UpPoints[2], Statics[i].UpPoints[3]);
		DrawPolygon(Statics[i].BottomPoints[0], Statics[i].BottomPoints[1], Statics[i].BottomPoints[2], Statics[i].BottomPoints[3]);

		glPushMatrix();
		glTranslatef(Statics[i].CircleCenter.x, Statics[i].CircleCenter.y, Statics[i].CircleCenter.z);
		glutSolidSphere(Statics[i].CircleRadius, BallComplexity, BallComplexity);
		glPopMatrix();

		glFlush();
	}
	
}

//����С��λ�ø��º���ײ��⣬����
void UpdateBalls()
{
	BallA.Move(TimeOnce);
	BallA.HandleCollisionBoard(XRange, ZRange);
	BallB.Move(TimeOnce);
	BallB.HandleCollisionBoard(XRange, ZRange);
	for (int i = 0; i < 4; i++)
	{
		BallA.HandleCollisionStatic(Statics[i]);
		BallB.HandleCollisionStatic(Statics[i]);
	}
	BallA.HandleCollisionBall(BallB);
	//TODO ��ײ��ʾ
	
}

//����һ��С��
void DrawOneBall(MovingBall& ball)
{
	//�������������ʵ���Ϣ
	glMaterialfv(GL_FRONT, GL_AMBIENT, ball.Ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ball.Diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ball.Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, ball.Shininess);

	//ƽ�Ƶ�����ԭ�㣬���ƣ��ָ�����
	glPushMatrix();
	glTranslatef(ball.CurrentPlace.x, ball.CurrentPlace.y, ball.CurrentPlace.z);
	glutSolidSphere(ball.Radius, BallComplexity, BallComplexity);
	glPopMatrix();
}

//����С��
void DrawBalls()
{
	//����С��λ��
	UpdateBalls();
	
	//����С��
	DrawOneBall(BallA);
	DrawOneBall(BallB);
}

//���Ƶ�������
void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//�����ɫ����
	SetCamera();//�������
	DrawStatics();//���ƾ�̬����
	DrawBoards();//���Ƶذ�ͱ߿�
	DrawBalls();//���ºͻ���С��
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

//�������̵����WASD��
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

//�������̵����ǰ�����ң�
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