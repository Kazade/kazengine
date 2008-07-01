#include "bsp_map_renderer.h"

bool bsp_map_renderer::initialize(shared_ptr<base_map> map) {

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
