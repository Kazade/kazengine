#ifndef BASIC_MAP_RENDERER_H_INCLUDED
#define BASIC_MAP_RENDERER_H_INCLUDED

#include <kazmathxx/aabb.h>

#include "map/surface_list.h"
#include "map/imap_renderer.h"

class basic_map_renderer : public map_renderer_interface {
	public:
		basic_map_renderer(shared_ptr<resource_manager_interface>);
		virtual ~basic_map_renderer();

		bool initialize(shared_ptr<base_map> map);
		void generate_resources();
		void pre_render(shared_ptr<frustum> frustum, const float* camera_position);
		void post_render();
		void render_map();

		virtual shared_ptr<resource_manager_interface> get_resource_manager() {
			return m_resource_manager;
		}

	private:
		shared_ptr<base_map> m_map_pointer;

		vector<AABB> m_surface_bounds;
		surface_list m_opaque_surfaces;
		surface_list m_translucent_surfaces;

		shared_ptr<resource_manager_interface> m_resource_manager;
};


#endif // BASIC_MAP_RENDERER_H_INCLUDE
