#include "scene/frustum.h"
#include "basic_map_renderer.h"

basic_map_renderer::basic_map_renderer() {

}

basic_map_renderer::~basic_map_renderer() {

}

bool basic_map_renderer::initialize(shared_ptr<base_map> map) {
	//TODO: Calculate bounding boxes for each polygon
	//Make an AABB implementation in Kazmath/Kazmathxx to integrate
	//with the frustum stuff

	m_map_pointer = map; //Assign the map to the internal pointer
/*
	face_array faces = map->get_faces(); //get the list of faces in this map

	m_surface_bounds.resize(faces.size());

	int j = 0;
	for (face_array::iterator i = faces.begin();
		i != faces.end(); ++i) {

		Vec3 min(10000, 10000, 10000);
		Vec3 max(-10000, -10000, -10000);

		vector<map_vertex>::iterator start = (*i)->get_vertices().begin();
		vector<map_vertex>::iterator end = (*i)->get_vertices().end();
		vector<map_vertex>::iterator vertex;

		for (vertex = start; vertex != end; ++vertex) {
			if ((*vertex).position.x < min.x) min.x = (*vertex).position.x;
			if ((*vertex).position.y < min.y) min.y = (*vertex).position.y;
			if ((*vertex).position.z < min.z) min.z = (*vertex).position.z;

			if ((*vertex).position.x > max.x) max.x = (*vertex).position.x;
			if ((*vertex).position.y > max.y) max.y = (*vertex).position.y;
			if ((*vertex).position.z > max.z) max.z = (*vertex).position.z;
		}

		//m_surface_bounds[j] = AABB(min, max);
		j++;
	}*/

	return true;
}

void basic_map_renderer::generate_resources() {
	//Generate the textures for OpenGL
}

void basic_map_renderer::pre_render(shared_ptr<frustum> frustum, const float* camera_position) {
	//Build visible list by texture, sort the opaque and transparent surfaces
	face_array faces = m_map_pointer->get_faces();

	m_translucent_surfaces.surfaces.clear();
	m_opaque_surfaces.surfaces.clear();

	int i = faces.size();
	while(i--) {
	//	if (frustum->aabb_in_frustum(m_surface_bounds[i])) {
			if (faces[i]->is_translucent()) {
				m_translucent_surfaces.surfaces.push_back(faces[i]);
			} else {
				m_opaque_surfaces.surfaces.push_back(faces[i]);
			}
	//	}
	}

	m_opaque_surfaces.sort_by_texture(); //Sort the opaque surfaces by texture
	m_translucent_surfaces.sort_by_distance(BACK_TO_FRONT, camera_position); //Sort the transparent ones by depth
}

void basic_map_renderer::post_render() {
	//Clear the lists
}

void basic_map_renderer::render_map() {
	//Render the polygons
	glEnableClientState(GL_VERTEX_ARRAY);

	face_list::iterator face;
	for(face = m_opaque_surfaces.surfaces.begin(); face != m_opaque_surfaces.surfaces.end(); ++face) {
		glVertexPointer(3, GL_FLOAT, sizeof(map_vertex), &(*face)->get_vertices()[0]);
		glDrawArrays(GL_TRIANGLES, 0, (*face)->get_triangle_count());
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}
