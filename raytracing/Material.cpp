#include"Material.h"
#include<math.h>
#include"Color.h"

Color<float> CheckerMaterial::sample(Ray3 ray, vector3D<float> position, vector3D<float> normal)
{
	return (static_cast<int>(abs(floor(position.x*0.1) + floor(position.z*scale))) % 2) < 1 ? Color<float>(0, 0, 0) : Color<float>(1, 1, 1);
}


Color<float> PhongMaterial::sample(Ray3 ray, vector3D<float> position, vector3D<float> normal)
{
	vector3D<float> lightDir = vector3D<float>(1.0f, 1.0f, 1.0f).normalize();
	Color<float> lightColor(1.0f, 1.0f, 1.0f);
	float NdotL = dot(normal, lightDir);
	vector3D<float> H = (lightDir - ray.direction).normalize();
	float NdotH = dot(normal, H);
	Color<float> diffuseTerm = diffuse*(NdotL> 0?NdotL:0);
	auto temp = (NdotH > 0 ? NdotH : 0);
	Color<float> specularTerm = specular*(pow(temp, shininess));
	return lightColor*(diffuseTerm + specularTerm);
}