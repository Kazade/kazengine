#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include <stdexcept>
#include <map>
#include <string>
#include <list>
#include <tr1/memory>
#include <kazmathxx/vec4.h>

#include "iscenemanager.h"
#include "resources/resource_manager.h"


using std::tr1::shared_ptr;
using std::string;
using std::list;

class scene_node_interface;
class graphics_driver_interface;

typedef list<shared_ptr<scene_node_interface> > smart_scene_node_list;
class scene_manager : public scene_manager_interface {
	public:
		scene_manager(shared_ptr<graphics_driver_interface> video_device);
		virtual ~scene_manager();

		bool register_scene_node_factory(shared_ptr<scene_node_factory_interface> factory);

		//Remove any dead nodes
		void flush();
		void kill_scene_node(const scene_node_interface* node);
		void render_all();

		scene_node_interface* add_built_in_scene_node(scene_node_type type, scene_node_interface* parent = 0);
		scene_node_interface* add_custom_scene_node(const string& type_name, scene_node_interface* parent = 0);

		void register_node_for_rendering(const scene_node_interface* node);

		camera_scene_node_interface* get_active_camera();
		void set_active_camera(camera_scene_node_interface* camera);

		shared_ptr<resource_manager> get_resource_manager();

		bool initialize();
		void deinitialize();

		virtual map_scene_node_interface* add_quake3_scene_node(const string& filename);
		virtual bool remove_scene_node(const scene_node_interface* node);

	private:
		shared_ptr<scene_node_interface> m_root_node;
		smart_scene_node_list m_node_list; ///< A list of all nodes
		smart_scene_node_list m_dead_nodes; ///< A list of nodes waiting to be removed

		smart_scene_node_list::iterator get_node_iterator(const scene_node_interface* node);

		smart_scene_node_list m_nodes_for_rendering; ///< A list of nodes that have been registered for rendering

		shared_ptr<camera_scene_node_interface> m_active_camera; ///< The currently active camera

		shared_ptr<resource_manager> m_resource_manager;

		shared_ptr<graphics_driver_interface> m_video_device;

		virtual void update(float dT);
};

#include "icamerascenenode.h"

template <typename T>
T* node_down_cast(scene_node_interface* node) {
	T* res = dynamic_cast<T*>(node);
	if (!res) {
		throw std::runtime_error("Attempted to cast a node to an invalid type");
	}

	return res;
}

#endif // SCENEMANAGER_H_INCLUDED
