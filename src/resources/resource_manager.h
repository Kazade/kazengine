#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <queue>
#include <vector>
#include <map>
#include <tr1/unordered_map>
#include <physfs.h>
#include "iresource_manager.h"
#include "utilities/threaded_class.h"

using std::vector;
using std::queue;
using std::map;
using std::tr1::unordered_map;

/**
 * A resource manager that uses integers as the resource_id
 */
class resource_manager : public resource_manager_interface, public threaded_class {
	public:
		static void initialize(int argc, char** argv) {
			if (s_was_initialized) return;

			PHYSFS_init(argv[0]);
			s_was_initialized = true;
			return;
		}

		static void deinitialize() {
			if (!s_was_initialized) return;

			PHYSFS_deinit();
		}

		template <typename T>
		resource_id load_resource(const string& filename);

		bool is_resource_loaded(const resource_id& id) const;

		resource_id queue_file_for_loading(const string& filename, shared_ptr<resource_interface>* new_res, shared_ptr<boost::mutex> res_mutex);

		file_load_status get_resource_load_status(const resource_id id) const;
	protected:

		/** Actions to do each iteration */
		virtual void do_run();

	private:
		struct queued_resource {
			//This is a pointer to a shared_ptr!!!!
			shared_ptr<resource_interface>* res;
			shared_ptr<boost::mutex> mutex;
			const string& filename;
			resource_id id;
		};

		/** Pointer to the resource we are currently loading */
		shared_ptr<queued_resource> m_currently_loading;

		/** Queue of resources to load */
		boost::mutex m_load_queue_mutex;
		queue< shared_ptr< queued_resource > > m_load_queue;

		boost::mutex m_blocked_resource_mutex;
		shared_ptr< queued_resource > m_blocked_resource;

		unordered_map<resource_id, file_load_status> m_load_status;

		/** ID => resource lookup */
		boost::mutex m_resources_mutex;
		unordered_map< resource_id, shared_ptr<resource_interface> > m_resources;

		static bool s_was_initialized;

		resource_id generate_next_id() {
			static resource_id s_id = 0;
			return s_id++;
		}
};

template <typename T>
resource_id resource_manager::load_resource(const string& filename) {
	/**
		What we do here is lock the blocked_resource,
		and fill it with the right data ready for the main
		resource process to load it.
	*/
	//Create the resource
	shared_ptr<resource_interface> new_resource(new T());
	resource_id id = generate_next_id();

	//Lock the blocked resource
	{
		boost::mutex::scoped_lock(m_blocked_resource_lock);
		//assert(!m_blocked_resource);
		m_blocked_resource.reset(new queued_resource());
		m_blocked_resource->id = id;
		m_blocked_resource->filename = filename;
		m_blocked_resource->mutex = shared_ptr<boost::mutex>(new boost::mutex());
		//Associate the pointer to pointer (remember, this is a pointer to a shared_ptr)
		//As soon as we leave this function then this pointer will be invalid but
		//that doesnt matter
		m_blocked_resource->res = &new_resource;
	}//unlock

	//Wait until the resource manager loads the resource
	while (!is_resource_loaded(m_blocked_resource->id)) {
		sleep(0);
	}

	m_blocked_resource.reset();

	return id;
}

#endif
