#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED

class Mouse {
	public:
		virtual ~Mouse() {}

		void showCursor(bool value);

		virtual bool isCursorVisible() = 0;

		void update();

		const Vec2& getRelativePos();
		const Vec2& getAbsolutePos();

	protected:
		virtual void doHideCursor() = 0;
		virtual void doShowCursor() = 0;

		virtual void doUpdate() = 0;

		virtual Vec2 getCursorPos() = 0;
		virtual void warpMouse(int x, int y) = 0;
};


class InputSystem : public IEventHandler {
	public:
		void resize(int w, int h); //Notify the input system of the window size
};

#endif // INPUTSYSTEM_H_INCLUDED
