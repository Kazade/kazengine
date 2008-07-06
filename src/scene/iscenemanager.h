#ifndef ISCENEMANAGER_H_INCLUDED
#define ISCENEMANAGER_H_INCLUDED

#include "iscenenode.h"

class scene_node_factory_interface;
class camera_scene_node_interface;

class scene_manager_interface {
	public:
		virtual ~scene_manager_interface() {}

		//Register a factory for creating custom scene nodes
		virtual bool register_scene_node_factory(shared_ptr<scene_node_factory_interface> factory) = 0;

		//Create a custom scene node using a typename (factory must be registered)
		virtual scene_node_interface* add_custom_scene_node(const string& type_name, scene_node_interface* parent = 0) = 0;

		//Create one of the built in scene nodes
		virtual scene_node_interface* add_built_in_scene_node(scene_node_type type, scene_node_interface* parent = 0) = 0;

		//Render all of the scene nodes
		virtual void render_all() = 0;

		virtual camera_scene_node_interface* get_active_camera() = 0;
		virtual void set_active_camera(camera_scene_node_interface* camera) = 0;

		//Remove any dead nodes
		virtual void flush() = 0;

		virtual void kill_scene_node(scene_node_interface* node) = 0;

		virtual void register_node_for_rendering(scene_node_interface* node) = 0;
};

#endif // ISCENEMANAGER_H_INCLUDED
