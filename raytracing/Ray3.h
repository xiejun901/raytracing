#ifndef _RAY3_H_
#define _RAY3_H_
#pragma once
#include"vector3D.h"
#include<memory>
#include"Material.h"


class IntersectResult{
public:
	IntersectResult() :geometry(0), distance(0), position(vector3D<float>(0, 0, 0)), normal(vector3D<float>(0, 0, 0)){}
	int geometry = 0;
//	std::shared_ptr<Material> m_material;
	float distance = 0;
	vector3D<float> position = vector3D<float>(0, 0, 0);
	vector3D<float> normal = vector3D<float>(0, 0, 0);
};

class Ray3{
	friend class Sphere;
public:
	Ray3(const vector3D<float> &o, const vector3D<float> &d) :origin(o), direction(d){}
	vector3D<float> getPoint(float t)
	{
		return origin + t*direction;
	}
//private:
	vector3D<float> origin;
	vector3D<float> direction;
};


class Sphere{
	typedef vector3D<float> v3d;
public:
	Sphere() = default;
	Sphere(v3d c, float r) :centor(c), radius(r),sqrradius(r*r){}
	IntersectResult intersect(Ray3 ray)
	{
		IntersectResult result;
		auto v = ray.origin - centor;
		auto a0 = v.sqrLength() - sqrradius;
		auto DdotV = dot(ray.direction,v);
		if (DdotV <= 0)
		{
			auto discr = DdotV*DdotV - a0;
			result.geometry = 1;
			result.distance = -DdotV - sqrt(discr);
			result.position = ray.getPoint(result.distance);
			result.normal = (result.position - centor).normalize();
		}
		return result;
	}
private:
	v3d centor;
	float radius;
	float sqrradius;
};

class PerspectiveCamera{
	typedef vector3D<float> v3d;
public:

	PerspectiveCamera(v3d e,v3d f,v3d u,float fov) :eye(e),front(f),refUp(u),fov(fov){}
	void initialize()
	{
		right = cross(front,refUp);
		up = cross(right, front);
		fovScale = static_cast<float>(tan(fov*0.5*3.1415 / 180) * 2);
	}
	Ray3 generateRay(float x, float y)
	{
		auto r = right*((x - 0.5f)*fovScale);
		auto u = up*((y - 0.5f)*fovScale);
		return Ray3(eye, (front + r + u).normalize());
	}
	v3d eye, front, refUp;
	float fov;

	v3d right, up;
	float fovScale;
};


#endif