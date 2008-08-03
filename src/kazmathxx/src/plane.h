#ifndef KMXX_PLANE_H
#define KMXX_PLANE_H

#include <kazmath/plane.h>
#include <kazmath/vec3.h>

/**
Class to represent a 3D plane
*/

struct Plane : public kmPlane {
	Plane(); ///Default constructor
	Plane(const kmVec3& v1, const kmVec3& v2, const kmVec3& v3); ///Builds a plane from 3 points (i.e. a triangle)
	Plane(const Plane& p); ///Copy constructor
};

#endif
