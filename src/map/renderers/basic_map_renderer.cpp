#include <tr1/memory>
#include <ostream>
#include <iostream>

#include "scene/frustum.h"
#include "scene/iscenenode.h"
#include "scene/icamerascenenode.h"

#include "resources/iresource_manager.h"
#include "resources/itexture.h"
#include "basic_map_renderer.h"

using std::tr1::dynamic_pointer_cast;

basic_map_renderer::basic_map_renderer(shared_ptr<resource_manager_interface> rmgr):
m_resource_manager(rmgr) {

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

	vector<map_texture>::iterator texture = m_map_pointer->get_textures().begin();

	for (; texture != m_map_pointer->get_textures().end(); ++texture) {
		resource_id id = (*texture).get_resource_id();
		//We wait until the texture has been loaded
		while (!get_resource_manager()->has_resource_loading_finished(id)) {
			sleep(0);
		}

		//Get a pointer to the resource
		shared_ptr<texture_interface> tex;
		tex = dynamic_pointer_cast<texture_interface>(get_resource_manager()->get_resource(id));

		if (!tex) {
#ifndef NDEBUG
			std::cout << "Texture could not be loaded: " << id << std::endl;
#endif
			(*texture).set_opengl_texture_id(0);
			continue;
		}

		//generate the texture
		tex->generate();

		//Store the opengl texture ID for the newly generated texture
		(*texture).set_opengl_texture_id(tex->get_ogl_texture_id());
	}
}

void basic_map_renderer::pre_render(shared_ptr<camera_scene_node_interface> camera) {
	//Build visible list by texture, sort the opaque and transparent surfaces
	Vec3 camera_position = dynamic_pointer_cast<scene_node_interface>(camera)->get_position();
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
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	face_list::iterator face;
	for(face = m_opaque_surfaces.surfaces.begin(); face != m_opaque_surfaces.surfaces.end(); ++face) {
		glBindTexture(GL_TEXTURE_2D, m_map_pointer->get_textures()[(*face)->get_texture_index()].get_opengl_texture_id());
		int index_count = (*face)->get_triangle_count() * 3;
		glVertexPointer(3, GL_FLOAT, sizeof(map_vertex), &(*face)->get_vertices()[0].position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(map_vertex), &(*face)->get_vertices()[0].texture_coordinate);
		glDrawArrays(GL_TRIANGLES, 0, index_count);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
