#include <GL/gl.h>
#include <SDL/SDL.h>

#include "opengl2driver.h"

void OpenGL2Driver::draw3DTriangle(const vector<Vec3>& vertices, const Colour& colour) {
	glColor4ub(colour.red, colour.green, colour.blue, colour.alpha);
}

void OpenGL2Driver::beginScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void OpenGL2Driver::endScene() {
	SDL_GL_SwapBuffers();
}

bool OpenGL2Driver::doInitialize() {
	loadSupportedExtensions();

	//Set some sane defaults
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	return true;
}

bool OpenGL2Driver::loadSupportedExtensions() {
	return true;
}
