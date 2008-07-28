#ifndef OPENGL2DRIVER_H_INCLUDED
#define OPENGL2DRIVER_H_INCLUDED

#include "utilities/colour.h"
#include "utilities/timer.h"
#include "igraphicsdriver.h"

class opengl_2_driver : public graphics_driver_interface {
	public:
		opengl_2_driver() {
			m_Ext.vertexBufferObjects = false;
			m_timer = shared_ptr<timer_interface>(new timer());
		}

		struct ExtensionsEnabled {
			bool vertexBufferObjects;
		} m_Ext;


		void begin_scene();
		void end_scene();
		void draw_3d_triangle(const vector<Vec3>& vertices, const Colour& colour);

		shared_ptr<timer_interface> get_timer() {
			return m_timer;
		}

	protected:
		bool do_initialize();

	private:
		bool load_supported_extensions();

		shared_ptr<timer_interface> m_timer;
};


#endif // OPENGL2DRIVER_H_INCLUDED
