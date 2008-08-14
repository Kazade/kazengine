#include <GL/gl.h>
#include <SDL/SDL.h>

#include "opengl2driver.h"

void opengl_2_driver::draw_3d_triangle(const vector<Vec3>& vertices, const Colour& colour) {
	glColor4ub(colour.red, colour.green, colour.blue, colour.alpha);
}

void opengl_2_driver::begin_scene() {
	m_timer->update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void opengl_2_driver::end_scene() {
	SDL_GL_SwapBuffers();
}

bool opengl_2_driver::do_initialize() {
	load_supported_extensions();

	m_timer->initialize();

	m_current_matrix_mode = MODELVIEW;

	return true;
}

bool opengl_2_driver::load_supported_extensions() {
	return true;
}
