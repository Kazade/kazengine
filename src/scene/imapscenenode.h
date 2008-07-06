#ifndef IMAPSCENENODE_H_INCLUDED
#define IMAPSCENENODE_H_INCLUDED

class map_scene_node_interface {
	public:
		virtual ~map_scene_node_interface() {}

		virtual void set_map_loader(shared_ptr<base_map> loader) = 0;
		virtual void set_map_renderer(shared_ptr<map_renderer_interface> renderer) = 0;
};

#endif // IMAPSCENENODE_H_INCLUDED
