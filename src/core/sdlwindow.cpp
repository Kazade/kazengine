#include "sdlwindow.h"

SDLWindow::SDLWindow():
m_Screen(0) {

}

SDLWindow::~SDLWindow() {
	destroy();
}

bool SDLWindow::create(const Vec2& size, const BitDepth& depth, bool fullscreen) {
	getLog()->setLevel(CURRENT_LEVEL);

	getLog()->debug("Creating the window");

	//all values are "at least"!
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Flags tell SDL about the type of window we are creating.
	int flags = SDL_OPENGL;

	if(fullscreen == true) {
		flags |= SDL_FULLSCREEN;
	}

	// Create the window
	m_Screen = SDL_SetVideoMode(size.x, size.y, depth.bits, flags);

	if(m_Screen == 0) {
		return false;
	}

	//SDL doesn't trigger off a ResizeEvent at startup, but as we need this for OpenGL, we do this ourself
	SDL_Event resizeEvent;
	resizeEvent.type = SDL_VIDEORESIZE;
	resizeEvent.resize.w = size.x;
	resizeEvent.resize.h = size.y;

	SDL_PushEvent(&resizeEvent);

	getLog()->debug("Window creation complete");

	return true;
}

void SDLWindow::setWindowCaption(const string& title) {
	m_Title = title;
	// Set the title.
	SDL_WM_SetCaption(title.c_str(), title.c_str());
}

void SDLWindow::destroy() {
	if (m_Screen) {
		SDL_FreeSurface(m_Screen);
		m_Screen = 0;
	}
}

bool SDLWindow::on_event_received(const SDL_Event& e) {
	switch(e.type) {
		case SDL_ACTIVEEVENT:
			if (e.active.state == SDL_APPMOUSEFOCUS) {
				m_WindowActive = (e.active.gain == 1);

				getLog()->debug("The window gained focus");
			}
			return true;
		break;
	}

	return false;
}
