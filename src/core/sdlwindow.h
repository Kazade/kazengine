#ifndef SDLWINDOW_H_INCLUDED
#define SDLWINDOW_H_INCLUDED

#include <boost/noncopyable.hpp>
#include "iwindow.h"

class SDLWindow : public IWindow {
	public:
		SDLWindow();
		~SDLWindow();

		virtual bool create(const Vec2& size, const BitDepth& depth, bool fullscreen);
		virtual void setWindowCaption(const string& title);
		virtual void destroy();
		virtual bool isFullscreen() { return m_Fullscreen; }
		virtual bool isWindowActive() { return m_WindowActive; }

		virtual bool on_event_received(const SDL_Event& e);

	private:
		Vec2 m_Size;
		string m_Title;
		bool m_Fullscreen;
		bool m_WindowActive;
		SDL_Surface* m_Screen;

		SDLWindow(const SDLWindow&); //Noncopyable
		SDLWindow operator=(const SDLWindow&); //Noncopyable
};

#endif // SDLWINDOW_H_INCLUDED
