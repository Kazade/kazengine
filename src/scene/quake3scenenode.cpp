#include "quake3scenenode.h"

quake3_scene_node::quake3_scene_node() {
	m_node_type = SNT_QUAKE3_BSP;
}

void quake3_scene_node::on_register_scene_node() {
	if (m_is_visible) {
		get_scene_manager()->register_node_for_rendering(this);
	}

	scene_node::on_register_scene_node();
}
