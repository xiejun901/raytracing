#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_
#pragma once
#include"Ray3.h"
#include"vector3D.h"
#include"Material.h"
#include"set"
#include<memory>


class MyIntersectResult{
public:
	MyIntersectResult() :geometry(0), distance(0), position(vector3D<float>(0, 0, 0)), normal(vector3D<float>(0, 0, 0)), m_material(nullptr){}
	int geometry = 0;
	std::shared_ptr<Material> m_material;
	float distance = 0;
	vector3D<float> position = vector3D<float>(0, 0, 0);
	vector3D<float> normal = vector3D<float>(0, 0, 0);
};
class BaseObject{
public:
	virtual MyIntersectResult intersect(Ray3 ray) = 0;
};
class SphereObject:public BaseObject{
	typedef vector3D<float> v3d;
public:
	SphereObject() = default;
	SphereObject(v3d c, float r) :centor(c), radius(r), sqrradius(r*r),m_material(new PhongMaterial(Color<float>(0.55f,0.20f,0.35f),Color<float>(1,1,1),6)){}
	MyIntersectResult intersect(Ray3 ray)
	{
		MyIntersectResult result;
		auto v = ray.origin - centor;
		auto a0 = v.sqrLength() - sqrradius;
		auto DdotV = dot(ray.direction, v);
		if (DdotV <= 0)
		{
			auto discr = DdotV*DdotV - a0;
			result.geometry = 1;
			result.m_material = m_material;
			result.distance = -DdotV - sqrt(discr);
			result.position = ray.getPoint(result.distance);
			result.normal = (result.position - centor).normalize();
		}
		return result;
	}
//private:
	v3d centor;
	float radius;
	float sqrradius;
	std::shared_ptr<Material> m_material;

};

class PlaneObject :public BaseObject{
public:
	typedef vector3D<float> v3d;
	PlaneObject(v3d n, float d) :normal(n), d(d), position(n*d), m_material(new CheckerMaterial(0.1F)){}
	MyIntersectResult intersect(Ray3 ray)
	{
		MyIntersectResult result;
		auto a = dot(ray.direction, normal);
		if (a >= 0)
			return result;
		else
		{
			auto b = dot(normal, ray.origin - position);
			result.geometry = 1;
			result.m_material = m_material;
			result.distance = -b / a;
			result.position = ray.getPoint(result.distance);
			result.normal = normal;
			return result;
		}
		return result;
	}
//private:
	v3d normal,position;
	float d;
	std::shared_ptr<Material> m_material;
};

class UnionObject{
public:
	void insert(std::shared_ptr<BaseObject> o)
	{
		objects.insert(o);
	}
	MyIntersectResult intersect(Ray3 ray)
	{
		float minDistance = FLT_MAX;
		MyIntersectResult minResult;
		for (auto po : objects)
		{
			auto result = po->intersect(ray);
			if (result.geometry&&result.distance < minDistance)
			{
				minDistance = result.distance;
				minResult = result;
			}
		}
		return minResult;
	}
private:
	std::set<std::shared_ptr<BaseObject>> objects;

};


Color<float> rayTraceRecursive(UnionObject &scene, const Ray3 ray, int maxReflect);
#endif