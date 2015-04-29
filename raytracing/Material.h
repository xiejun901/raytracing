#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#pragma once
#include"Ray3.h"
#include"Color.h"
class Ray3;
class Material{
public:
	virtual Color<float> sample(Ray3 ray, vector3D<float> position, vector3D<float> normal)=0;
};

class CheckerMaterial:public Material{
public:
	CheckerMaterial(float scale) :scale(scale){}
	Color<float> sample(Ray3 ray, vector3D<float> position, vector3D<float> normal);
private:
	float scale;
};

class PhongMaterial :public Material{
public:
	PhongMaterial(Color<float> diffuse, Color<float>specular, float shininess) :diffuse(diffuse), specular(specular), shininess(shininess){}
	Color<float> sample(Ray3 ray, vector3D<float> position, vector3D<float> normal);
private:
	//static vector3D<float> lightDir;
	//static Color<float> lightColor;
	Color<float> diffuse,specular;
	float shininess;
};
#endif