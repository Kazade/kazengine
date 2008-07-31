#include <iostream>
#include <tr1/memory>

#include "../bsp_map.h"
#include "bsp_map_renderer.h"

using std::tr1::dynamic_pointer_cast;
bool bsp_map_renderer::initialize(shared_ptr<base_map> map) {

	shared_ptr<bsp_map> bsp_map_instance = dynamic_pointer_cast<bsp_map>(map);
	if (!bsp_map_instance) {
#ifndef NDEBUG
			std::cout << "This is not a bsp map!" << std::endl;
#endif
			return false;
	}


	return true;
}

/** Generate any resources that require a valid OGL context */
void bsp_map_renderer::generate_resources() {

}

/** Perform any prerendering steps using the frustum for any culling operations */
void bsp_map_renderer::pre_render(shared_ptr<frustum> frustum) {

}

void bsp_map_renderer::post_render() {

}

void bsp_map_renderer::render_map() {

}
