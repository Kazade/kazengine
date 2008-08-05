#include <tr1/memory>
#include <boost/thread/thread.hpp>
#include <tr1/functional>

using std::tr1::shared_ptr;
using std::tr1::bind;

/**
 *	A class that will spawn a thread and call the overridden do_run() method in a loop.
 *	it uses boost::thread */
class threaded_class {
	public:
		threaded_class():
		m_stop_requested(false),
		m_thread(shared_ptr<boost::thread>()) {

		}

		virtual ~threaded_class() {

		}

		void start_thread() {
			assert(!m_thread);
			m_thread = shared_ptr<boost::thread> (new boost::thread(bind(&threaded_class::run, this)));
		}

		void stop_thread() {
			assert(m_thread);
			m_stop_requested = true;
			m_thread->join();
		}

	protected:
		virtual void do_run() = 0;

	private:
		volatile bool m_stop_requested;
		shared_ptr<boost::thread> m_thread;

		void run() {
			while(!m_stop_requested) {
				do_run();
			}
		}
};
