#ifndef BASIC_FACE_H_INCLUDED
#define BASIC_FACE_H_INCLUDED

#include "map_face.h"

class basic_face : public map_face {
	public:
		virtual void add_triangle(const map_vertex& v1, const map_vertex& v2, const map_vertex& v3);
		virtual ~basic_face() {}
};

#endif // BASIC_FACE_H_INCLUDED
