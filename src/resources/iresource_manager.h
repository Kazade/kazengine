#include <tr1/memory>
#include <boost/thread/thread.hpp>
#include "iresource.h"

using std::tr1::shared_ptr;

typedef int resource_id;

class resource_manager_interface {
	public:
		virtual ~resource_manager_interface() {}

		/**	Queue a file for async loading */
		virtual resource_id queue_file_for_loading(const string& filename, shared_ptr<resource_interface>* new_res, shared_ptr<boost::mutex> res_mutex) = 0;

		/** Block until a file is loaded */
//		virtual resource_id load_resource(const string& filename) = 0;

		/** Get a resource by its ID */
		virtual resource_interface* get_resource(const resource_id& res_id) = 0;

		/** Generates the next resource_id */
		virtual resource_id generate_next_id() const = 0;

		/** Add a path to the resource manager where it can
		 * find resources to load */
		virtual bool add_to_search_path(const string& path) = 0;

		/** Delete any search paths so we can start again */
		virtual void clear_search_paths() = 0;

		/** Returns true if the file is on the search path */
		virtual bool is_file_available(const string& file) const = 0;

		/** Returns true if the file has been loaded */
		virtual bool is_file_loaded(const string& file) const = 0;

		/** Returns true if the resource_id exists and the file has been loaded */
		virtual bool is_resource_loaded(const resource_id& id) const = 0;

		/** Returns the status of a file load */
		virtual file_load_status get_resource_load_status(const resource_id id) const = 0;
};
