#pragma once
class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float _x, float _y, float _z);
	explicit Vector3(float value);
	Vector3(const Vector3& other);
	~Vector3();
	Vector3 operator + (const Vector3& other) const;
	Vector3 operator - (const Vector3& other) const;
	Vector3 operator - () const;
	Vector3 operator * (const float value) const;
	float operator * (const Vector3& other) const;
	Vector3 operator / (const float value) const;
	Vector3& operator += (const Vector3& other);
	Vector3& operator -= (const Vector3& other);
	Vector3& operator *= (const float value);
	Vector3& operator /= (const float value);
	Vector3& operator = (const Vector3 other);
	bool operator == (const Vector3& other) const;
	bool operator != (const Vector3& other) const;

	friend Vector3 operator * (const float value, const Vector3& other);
};

