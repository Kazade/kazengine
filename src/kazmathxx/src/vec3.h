#ifndef KMXX_VEC3_H
#define KMXX_VEC3_H

#include <kazmath/vec3.h>
#include <kazmath/vec2.h>

typedef struct Vec3 : public kmVec3 {
	Vec3();
	Vec3(const kmVec3& v);
	Vec3(float X, float Y, float Z);
	Vec3(const kmVec2& v, float z);

	operator float* () {
		return (float*) this;
	}

	operator const float* () {
		return (const float*) this;
	}

	Vec3 operator+ (const Vec3& v) const {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3& operator+= (const Vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3& operator-= (const Vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vec3 operator* (const float s) const {
		return Vec3(x * s, y * s, z * s);
	}

	Vec3& operator*= (const float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vec3& operator/= (const float s) {
		kmVec3Scale(this, this, 1.0 / s);
		return *this;
	}

	Vec3 operator-(const Vec3& v2) const {
		return Vec3(x - v2.x, y - v2.y, z - v2.z);
	}

	const Vec3 operator- () const {
			return Vec3(-x, -y, -z);
	}

	bool operator== (const Vec3& vec) const {
		return kmVec3AreEqual(this, &vec);
	}

	bool operator!= (const Vec3& vec) const {
		return !kmVec3AreEqual(this, &vec);
	}

	float dot(const Vec3& v);
	Vec3 cross(const Vec3& v2);
	const Vec3& normalize();

} Vec3;

#endif
