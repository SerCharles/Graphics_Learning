/*
描述：phong模型类
日期：2020/11/6
*/
#pragma once
#include<GL/glut.h>
#include"Point.hpp"
#include"Board.hpp"
#include"MeshModel.hpp"
#include"GLLight.hpp"
#include"Camera.hpp"

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