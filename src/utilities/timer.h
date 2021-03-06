#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "itimer.h"

class timer : public timer_interface {
		public:
            timer():
            m_last_time(0),
            m_current_time(0) {

            }

			virtual void update() {
				m_last_time = m_current_time;
				m_current_time = static_cast<float>(SDL_GetTicks());
			}

			virtual float get_elapsed_time() {
			    update();
				float result = m_current_time - m_last_time;
				return result / 1000.0f;
			}

			virtual bool initialize() {
				update();
				return true;
			}

		private:
			float m_last_time;
			float m_current_time;

};

#endif // TIMER_H_INCLUDED
