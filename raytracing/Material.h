#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#pragma once
#include"Ray3.h"
#include"Color.h"
class Ray3;
class Material{
public:
	Material() = default;
	Material(float ref) :reflectiveness(ref){}
	virtual Color<float> sample(Ray3 ray, vector3D<float> position, vector3D<float> normal)=0;
//private:
	float reflectiveness;
};

class CheckerMaterial:public Material{
public:
	CheckerMaterial(float scale,float reflect=0.5) :Material(reflect),scale(scale){}
	Color<float> sample(Ray3 ray, vector3D<float> position, vector3D<float> normal);
private:
	float scale;
};

class PhongMaterial :public Material{
public:
	PhongMaterial(Color<float> diffuse, Color<float>specular, float shininess, float reflect = 0.5) :Material(reflect), diffuse(diffuse), specular(specular), shininess(shininess){}
	Color<float> sample(Ray3 ray, vector3D<float> position, vector3D<float> normal);
private:
	//static vector3D<float> lightDir;
	//static Color<float> lightColor;
	Color<float> diffuse,specular;
	float shininess;
};
#endif