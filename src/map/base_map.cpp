#include <iostream>

#include "base_map.h"

base_map::base_map(resource_manager_interface* owning_manager):
m_owning_resource_manager(owning_manager) {
    //ctor
}

base_map::~base_map() {
    m_faces.clear();
}

void base_map::print_statistics() {
	std::cout << "Vertices: " << m_vertices.size() << std::endl;
	std::cout << "Faces: " << m_faces.size() << std::endl;
	std::cout << "Planes: " << m_planes.size() << std::endl;
	std::cout << "Textures: " << m_textures.size() << std::endl;
}
