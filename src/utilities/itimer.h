#ifndef ITIMER_H_INCLUDED
#define ITIMER_H_INCLUDED

class timer_interface {
	public:
		virtual float get_elapsed_time() = 0;
};

#endif // ITIMER_H_INCLUDED
