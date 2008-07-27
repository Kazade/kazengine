#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "itimer.h"

class timer : public timer_interface {
		public:
			virtual float get_elapsed_time() {
				m_current_time = static_cast<float>(SDL_GetTicks());

				float result = m_current_time - m_last_time;

				m_last_time = m_current_time;

				return result;
			}

		private:
			float m_last_time;
			float m_current_time;

};

#endif // TIMER_H_INCLUDED
