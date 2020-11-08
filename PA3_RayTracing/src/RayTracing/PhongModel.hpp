/*
������phongģ����
���ڣ�2020/11/6
*/
#pragma once
#include<GL/glut.h>
#include"Point.hpp"
#include"Board.hpp"
#include"MeshModel.hpp"
#include"GLLight.hpp"
#include"Camera.hpp"

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