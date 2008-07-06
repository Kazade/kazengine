#include "iscenemanager.h"
#include "quake3scenenode.h"

quake3_scene_node::quake3_scene_node(scene_node_interface* parent, scene_manager_interface* mgr):
scene_node(parent, mgr) {
	m_node_type = SNT_QUAKE3_BSP;
}

void quake3_scene_node::on_register_scene_node() {
	if (is_visible()) {
		get_scene_manager()->register_node_for_rendering(this);
	}

	scene_node::on_register_scene_node();
}

void quake3_scene_node::set_map_renderer(shared_ptr<map_renderer_interface> renderer) {
	m_map_renderer = renderer;
}
