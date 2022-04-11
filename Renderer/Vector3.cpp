#include "Vector3.h"

Vector3::Vector3() : x(0), y(0), z(0) { }
Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
Vector3::Vector3(float value) : x(value), y(value), z(value) { }
Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) { }
Vector3::~Vector3() { }

Vector3 Vector3::operator + (const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}
Vector3 Vector3::operator - (const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator - () const 
{
	return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator * (const float value) const
{
	return Vector3(x * value, y * value, z * value);
}
float Vector3::operator * (const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}
Vector3 Vector3::operator / (const float value) const
{
	if (value != 0)
	{
		return Vector3(x / value, y / value, z / value);
	}
	return Vector3();
}
Vector3& Vector3::operator += (const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}
Vector3& Vector3::operator -= (const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}
Vector3& Vector3::operator *= (const float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}
Vector3& Vector3::operator /= (const float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}
Vector3& Vector3::operator = (const Vector3 other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}
bool Vector3::operator == (const Vector3& other) const
{
	return (x == other.x) && (y == other.y) && (z == other.z);
}
bool Vector3::operator != (const Vector3& other) const
{
	return (x != other.x) && (y != other.y) && (z != other.z);
}

Vector3 operator * (const float value, const Vector3& other)
{
	return other * value;
}