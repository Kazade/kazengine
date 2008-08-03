#ifndef BSP_MAP_RENDERER_H_INCLUDED
#define BSP_MAP_RENDERER_H_INCLUDED

#include "map/imap_renderer.h"

class bsp_map_renderer : public map_renderer_interface {
	public:
		bool initialize(shared_ptr<base_map> map);
		void generate_resources();
		void pre_render(shared_ptr<frustum> frustum);
		void post_render();
		void render_map();

	private:
		bool is_cluster_visible(int cluster_1_id, int cluster_2_id);
		void calc_visible_faces(shared_ptr<frustum> frustum);
		int get_camera_leaf(const Vec3& camera_position);

		shared_ptr<bsp_map> m_map;
};

#endif // BSP_MAP_RENDERER_H_INCLUDED
