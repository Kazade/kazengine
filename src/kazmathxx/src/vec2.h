#ifndef KMXX_VEC2_H
#define KMXX_VEC2_H

#include <kazmath/vec2.h>

typedef struct Vec2 : public kmVec2 {
	Vec2();
	Vec2(const kmVec2& v);
	Vec2(float X, float Y);
	
	Vec2 operator+ (const Vec2& v) {
		return Vec2(x + v.x, y + v.y);	
	}
	
	Vec2 operator* (const float s) {
		return Vec2(x * s, y * s);
	}
	
} Vec2;

#endif
