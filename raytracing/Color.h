#ifndef _COLOR_H_
#define _COLOR_H_
#pragma once
template<typename T>
class Color{
	friend Color<T> operator+<T >(const Color<T> &, const Color<T> &);
	friend Color<T> operator-<T>(const Color<T> &lhs, const Color<T> &rhs);
	friend Color<T> operator*<T>(const Color<T> &lhs, const Color<T> &rhs);
	friend Color<T> operator*<T>(const Color<T> &lhs, T var);
	friend Color<T> operator*<T>(T var, const Color<T> &rhs);
public:
	Color() = default;
	Color(T r, T g, T b) :red(r), green(g), blue(b){}
private:
	T red, green, blue;
};

template<typename T>
Color<T> operator+(const Color<T> &lhs, const Color<T> &rhs)
{
	return Color<T>(lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue);
}
template<typename T>
Color<T> operator-(const Color<T> &lhs, const Color<T> &rhs)
{
	return Color(lhs.red - rhs.red, lhs.green - rhs.green, lhs.blue - rhs.blue);
}
template<typename T>
Color<T> operator*(const Color<T> &lhs, const Color<T> &rhs)
{
	return Color(lhs.red * rhs.red, lhs.green * rhs.green, lhs.blue * rhs.blue);
}
template<typename T>
Color<T> operator*(const Color<T> &lhs, T var)
{
	return Color(lhs.red * var, lhs.green * var, lhs.blue * var);
}
template<typename T>
Color<T> operator*(T var, const Color<T> &rhs)
{
	return Color(var * rhs.red, var * rhs.green, var * rhs.blue);
}
#endif