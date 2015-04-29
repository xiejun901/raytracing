#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_
#pragma once
#include<math.h>
template<typename T>
class vector3D{
	template<typename T>
	friend vector3D<T> operator+(const vector3D<T> &lhs, const vector3D<T> &rhs);
	template<typename T>
	friend vector3D<T> operator-(const vector3D<T> &lhs, const vector3D<T> &rhs);
	template<typename T>
	friend vector3D<T> operator*(const vector3D<T> &lhs, const vector3D<T> &rhs);
	template<typename T>
	friend vector3D<T> operator*(const vector3D<T> &lhs, T var);
	template<typename T>
	friend vector3D<T> operator*(T var, const vector3D<T> &rhs);
	template<typename T>
	friend vector3D<T> operator/(const vector3D<T> &lhs, T var);
	template<typename T>
	friend vector3D<T> cross(const vector3D<T> &lhs, const vector3D<T> &rhs);
	template<typename T>
	friend T dot(const vector3D<T> &lhs, const vector3D<T> &rhs);
public:
	using value_type = T;
	vector3D() = default;
	vector3D(value_type x, value_type y, value_type z) :x(x), y(y), z(z){}
	vector3D(const vector3D& rhs) :x(rhs.x), y(rhs.y), z(rhs.z){}
	value_type length(){ return sqrt(x*x + y*y + z*z); }
	value_type sqrLength(){ return x*x + y*y + z*z; }
	vector3D normalize(){ value_type temp = 1 / length(); return vector3D( x*temp, y*temp, z*temp); }
	vector3D negte(){ return vector3D(-x, -y, -z); }
	vector3D &operator=(const vector3D rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

//private:
	value_type x, y, z;
};

template<typename T>
vector3D<T> operator+(const vector3D<T> &lhs, const vector3D<T> &rhs)
{
	return vector3D<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template<typename T>
vector3D<T> operator-(const vector3D<T> &lhs, const vector3D<T> &rhs)
{
	return vector3D<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template<typename T>
vector3D<T> operator*(const vector3D<T> &lhs, const vector3D<T> &rhs)
{
	return vector3D<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

template<typename T>
vector3D<T> operator*(const vector3D<T> &lhs, T var)
{
	return vector3D<T>(lhs.x * var, lhs.y * var, lhs.z * var);
}

template<typename T>
vector3D<T> operator*(T var, const vector3D<T> &rhs)
{
	return vector3D<T>(var * rhs.x, var * rhs.y, var * rhs.z);
}

template<typename T>
vector3D<T> operator/(const vector3D<T> &lhs, T var)
{
	return vector3D<T>(lhs.x / var, lhs.y / var, lhs.z / var);
}

template<typename T>
vector3D<T> cross(const vector3D<T> &lhs, const vector3D<T> &rhs)
{
	return vector3D<T>(-lhs.z*rhs.y + lhs.y*rhs.z, lhs.z*rhs.x - lhs.x*rhs.z, -lhs.y*rhs.x + lhs.x*rhs.y);
}
template<typename T>
T dot(const vector3D<T> &lhs, const vector3D<T> &rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}


#endif