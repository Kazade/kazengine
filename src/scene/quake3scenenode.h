#ifndef QUAKE3SCENENODE_H_INCLUDED
#define QUAKE3SCENENODE_H_INCLUDED

#include "scenenode.h"
#include "imapscenenode.h"

class quake3_scene_node :  public scene_node, public map_scene_node_interface {
	public:
		quake3_scene_node(scene_node_interface* parent, scene_manager_interface* mgr);

		void on_register_scene_node();

		void set_map_renderer(shared_ptr<map_renderer_interface> renderer);

	private:
		shared_ptr<base_map> m_map;
		shared_ptr<map_renderer_interface> m_map_renderer;
};

#endif // QUAKE3SCENENODE_H_INCLUDED
