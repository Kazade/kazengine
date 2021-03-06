#ifndef IGRAPHICSDRIVER_H_INCLUDED
#define IGRAPHICSDRIVER_H_INCLUDED

#include <tr1/memory>
#include <kazmathxx/vec3.h>
#include <kazmathxx/mat4.h>

#include <vector>
#include "utilities/colour.h"

using std::vector;
using std::tr1::shared_ptr;

class timer_interface;

class graphics_driver_interface {
	public:
		virtual void begin_scene() = 0;
		virtual void end_scene() = 0;

		virtual void draw_3d_triangle(const vector<Vec3>& vertices, const Colour& colour) = 0;
		virtual ~graphics_driver_interface() {}

		bool initialize() {
			bool status = do_initialize();
			return status;
		}

		virtual shared_ptr<timer_interface> get_timer() = 0;

		virtual void load_identity() = 0;
		virtual void load_matrix(const kmMat4& mat) = 0;
		virtual void push_matrix() = 0;
		virtual void pop_matrix() = 0;
		virtual void translate(float x, float y, float z) = 0;
		virtual void mult_matrix(const kmMat4& mat) = 0;
		virtual Mat4 get_projection_matrix() const = 0;
		virtual Mat4 get_modelview_matrix() const = 0;
	protected:
		virtual bool do_initialize() = 0;
};


#endif // IGRAPHICSDRIVER_H_INCLUDED
