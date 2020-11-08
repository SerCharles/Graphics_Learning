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
		GLfloat shininess = 100;

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
		GLfloat shininess = 100;

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
		GLfloat shininess = 100;

		Happy.InitPlace("happy.ply", size, center);
		Happy.InitColor(color, ambient, diffuse, specular, shininess);
	}
}

//��ʼ����������
void InitScene()
{

	InitGLLight();
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

/*
���������㻷������ɫ
��������Դ�������������Ƭ���������
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
�������������������ɫ
��������Դ�������������Ƭ��������������յķ���0,1,0������Ƭ������
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
���������㾵�淴�����ɫ
��������Դ���淴���������Ƭ���淴����������յķ���0,1,0�������߷�����Ƭ������
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
���������������Ƭ��phongģ��
��������ǰ��Դ����ǰ�����������Ƭ
���أ���ɫ
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
����������ı�����Ƭ��phongģ��
��������ǰ��Դ����ǰ�����������Ƭ
���أ���ɫ
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

//ѡ��ȫ��ģʽ
void ChooseMode()
{
	while (1)
	{
		cout << "������Ҫʹ�õ�ģʽ" << endl;
		cout << "1��OpenGL����ģ��" << endl;
		cout << "2���ҵ�phong����ģ��" << endl;
		int mode;
		cin >> mode;
		if (mode == OPENGL_LIGHT || mode == MY_LIGHT)
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
	else if (CurrentMode == MY_LIGHT)
	{
		cout << "2���ҵ�phong����ģ��" << endl;
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

	//TODO:KS
	Color result = color;
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
