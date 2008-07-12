#include "basic_face.h"

void basic_face::add_triangle(const map_vertex& v1, const map_vertex& v2, const map_vertex& v3) {
	if (m_triangle_count == -1) m_triangle_count = 0;

	get_vertices().push_back(v1);
	get_vertices().push_back(v2);
	get_vertices().push_back(v3);

	map_vertex* test = &get_vertices()[0];
	m_triangle_count++;
}
