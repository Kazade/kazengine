#include "bsp_map_renderer.h"

bool bsp_map_renderer::initialize(shared_ptr<base_map> map) {

	//We can only use the bsp renderer with a bsp map
	if (!dynamic_pointer_cast<bsp_map> (map) ) {
		throw std::logic_error("Attempt to use a BSP renderer on a non-BSP map");
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
