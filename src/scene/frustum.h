#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED

#include <kazmathxx/plane.h>
#include <kazmathxx/aabb.h>

class frustum {
	public:
		bool is_point_in_frustum(const float* point) const;
		bool is_cube_in_frustum(const float* point, const float size) const;
		bool is_aabb_in_frustum(const kmAABB& aabb) const;
		bool is_aabb_in_frustum(float minX, float minY, float minZ,
                              float maxX, float maxY, float maxZ) const;

		void update_frustum();
		void update_frustum(const float* model, const float* proj);

	private:
		Plane m_planes[6]; ///< 6 planes to make up the frustum

		void calculate_frustum(const float* model, const float* proj);
		void normalize_frustum();
};

#endif // FRUSTUM_H_INCLUDED
