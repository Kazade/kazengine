#include "vec3.h"

Vec3::Vec3() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vec3::Vec3(const kmVec3& v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

Vec3::Vec3(float X, float Y, float Z) {
	this->x = X;
	this->y = Y;
	this->z = Z;	
}

Vec3::Vec3(const kmVec2& v, float z) {
	x = v.x;
	y = v.y;
	this->z = z;
}

float Vec3::dot(const Vec3& v) {
	return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3& v2) {
	Vec3 ret;
	kmVec3Cross(&ret, this, &v2);
	return ret;
}

const Vec3& Vec3::normalize() {
	kmVec3Normalize(this, this);
	return *this;
}

