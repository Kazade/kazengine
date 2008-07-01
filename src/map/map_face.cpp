#include "map_face.h"

map_face::map_face() {
	//ctor
	m_triangle_count = -1;
	m_is_translucent = false;
}

map_face::~map_face() {
	//dtor
}

void map_face::clear() {
	m_vertices.clear();
}

