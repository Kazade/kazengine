#ifndef IEVENTHANDLER_H_INCLUDED
#define IEVENTHANDLER_H_INCLUDED

#include <SDL/SDL.h>
#include "ilogholder.h"

class event_handler_interface : public ILogHolder {
	public:
		virtual ~event_handler_interface() {}

		///Returns true if the event was handled (no further processing will be done)
		virtual bool on_event_received(const SDL_Event& e) = 0;
};

#endif // IEVENTHANDLER_H_INCLUDED
