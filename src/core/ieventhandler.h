#ifndef IEVENTHANDLER_H_INCLUDED
#define IEVENTHANDLER_H_INCLUDED

#include <SDL/SDL.h>
#include "ilogholder.h"

class IEventHandler : public ILogHolder {
	public:
		virtual ~IEventHandler() {}

		///Returns true if the event was handled (no further processing will be done)
		virtual bool onEvent(const SDL_Event& e) = 0;
};

#endif // IEVENTHANDLER_H_INCLUDED
