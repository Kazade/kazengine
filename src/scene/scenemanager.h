#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include <map>
#include <string>
#include <list>
#include <tr1/memory>
#include <kazmathxx/vec4.h>

#include "iscenemanager.h"
#include "resources/resourcemanager.h"

using std::tr1::shared_ptr;
using std::string;
using std::list;

class scene_node_interface;

typedef list<shared_ptr<scene_node_interface> > smart_scene_node_list;
class scene_manager : public scene_manager_interface {
	public:
		scene_manager();
		virtual ~scene_manager();

		bool register_scene_node_factory(shared_ptr<scene_node_factory_interface> factory);

		//Remove any dead nodes
		void flush();
		void kill_scene_node(scene_node_interface* node);
		void render_all();

		scene_node_interface* add_built_in_scene_node(scene_node_type type, scene_node_interface* parent = 0);
		scene_node_interface* add_custom_scene_node(const string& type_name, scene_node_interface* parent = 0);

		void register_node_for_rendering(scene_node_interface* node);
	private:
		shared_ptr<scene_node_interface> m_root_node;
		smart_scene_node_list m_node_list; ///< A list of all nodes
		smart_scene_node_list m_dead_nodes; ///< A list of nodes waiting to be removed

		scene_node_list::iterator get_node_iterator(scene_node_interface* node);

		smart_scene_node_list m_nodes_for_rendering; ///< A list of nodes that have been registered for rendering
};

#endif // SCENEMANAGER_H_INCLUDED
