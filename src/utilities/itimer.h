#ifndef ITIMER_H_INCLUDED
#define ITIMER_H_INCLUDED

class timer_interface {
	public:
		virtual ~timer_interface() {}

		virtual float get_elapsed_time() = 0;
		virtual bool initialize() = 0;
		virtual void update() = 0;
};

#endif // ITIMER_H_INCLUDED
