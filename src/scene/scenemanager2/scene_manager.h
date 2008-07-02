#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class scene_manager {
	public:
		shared_ptr<camera_scene_node_interface> add_camera_scene_node_fps(scene_node_interface* parent=0);
		shared_ptr<map_scene_node_interface> add_map_scene_node(scene_node_interface* parent=0);
};

#endif
