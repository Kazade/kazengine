#include "vec2.h"

Vec2::Vec2() {
	this->x = 0.0f;
	this->y = 0.0f;
}

Vec2::Vec2(const kmVec2& v) {
	this->x = v.x;
	this->y = v.y;
}

Vec2::Vec2(float X, float Y) {
	this->x = X;
	this->y = Y;
}
