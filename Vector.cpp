#include "Vector.h"
#include <cmath>

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector2::length() const { return std::sqrt(x * x + y * y + z * z); }

Vector2& Vector2::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vector2::dot(const Vector2& v) const { return x * v.x + y * v.y + z * v.z; }

float Vector2::cross(const Vector2& v) const {return (y * v.z - z * v.y) + (z * v.x - x * v.z) + (x * v.y - y * v.x);}

Vector2 Vector2::operator+() const { return *this; }

Vector2 Vector2::operator-() const { return Vector2(-x, -y, -z); }

Vector2& Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector2& Vector2::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

// Vector2 ÉNÉâÉXÇ…ëÆÇ≥Ç»Ç¢ä÷êîåQ
//ìÒçÄââéZéq
const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s) {
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v) { return v * s; }

const Vector2 operator/(const Vector2& v, float s) {
	Vector2 temp(v);
	return temp /= s;
}