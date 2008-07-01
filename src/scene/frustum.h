#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED

#include <kazmathxx/plane.h>
#include <kazmathxx/aabb.h>

class frustum {
	public:
		bool point_in_frustum(const float* point);
		bool cube_in_frustum(const float* point, const float size);
		bool aabb_in_frustum(const kmAABB& aabb);

		void update_frustum();
		void update_frustum(const float* model, const float* proj);

	private:
		Plane m_planes[6]; ///< 6 planes to make up the frustum

		void calculate_frustum(const float* model, const float* proj);
		void normalize_frustum();
};

#endif // FRUSTUM_H_INCLUDED
