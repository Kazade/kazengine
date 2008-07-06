#ifndef QUAKE3SCENENODE_H_INCLUDED
#define QUAKE3SCENENODE_H_INCLUDED

class quake3_scene_node :  public scene_node, public map_scene_node_interface {
	public:
		quake3_scene_node();

		void on_register_scene_node();

	private:
		shared_ptr<base_map> m_map;
		shared_ptr<map_renderer_interface> m_map_renderer;
};

#endif // QUAKE3SCENENODE_H_INCLUDED
