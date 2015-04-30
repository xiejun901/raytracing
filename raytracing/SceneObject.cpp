#include"Ray3.h"
#include"vector3D.h"
#include"Material.h"
#include"Color.h"
#include"SceneObject.h"
Color<float> rayTraceRecursive(UnionObject &scene, const Ray3 ray, int maxReflect)
{
	auto result = scene.intersect(ray);
	if (result.geometry)
	{
		auto reflectiveness = result.m_material->reflectiveness;
		auto color = result.m_material->sample(ray, result.position, result.normal);
		color = color*(1 - reflectiveness);
		if (reflectiveness > 0 && maxReflect > 0)
		{
			auto r = result.normal*(-2 * dot(result.normal, ray.direction)) + ray.direction;
			Ray3 newray(result.position, r);
			auto refcolor = rayTraceRecursive(scene, newray, maxReflect - 1);
			color = color + (refcolor*reflectiveness);
		}
		return color;
	}
	else
		return Color<float>(0.0f, 0.0f, 0.0f);
}