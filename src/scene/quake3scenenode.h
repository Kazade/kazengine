#ifndef QUAKE3SCENENODE_H_INCLUDED
#define QUAKE3SCENENODE_H_INCLUDED

class quake3_scene_node :  public scene_node, public map_scene_node_interface {
	public:
		quake3_scene_node();

		void on_register_scene_node();

};

#endif // QUAKE3SCENENODE_H_INCLUDED
