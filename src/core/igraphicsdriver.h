#ifndef IGRAPHICSDRIVER_H_INCLUDED
#define IGRAPHICSDRIVER_H_INCLUDED

#include <tr1/memory>
#include <kazmathxx/vec3.h>
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

	protected:
		virtual bool do_initialize() = 0;
};


#endif // IGRAPHICSDRIVER_H_INCLUDED
