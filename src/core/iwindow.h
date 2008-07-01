#ifndef IWINDOW_H_INCLUDED
#define IWINDOW_H_INCLUDED

#include <cassert>
#include <kazmathxx/vec2.h>
#include <string>

#include "ieventhandler.h"

using std::string;

struct BitDepth {
	explicit BitDepth(uint bpp):
	bits(bpp) {
		assert(bpp == 0 || bpp == 16 || bpp == 24 | bpp == 32);
	}

	uint bits;
};

class IWindow : public IEventHandler {
	public:
		virtual ~IWindow() {}

		virtual bool create(const Vec2& size, const BitDepth& depth, bool fullscreen) = 0;
		virtual void setWindowCaption(const string& title) = 0;
		virtual void destroy() = 0;

		virtual bool isFullscreen() = 0;
		virtual bool isWindowActive() = 0;
};

#endif // IWINDOW_H_INCLUDED
