#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <tr1/unordered_map>
#include <tr1/memory>
#include <string>
#include <boost/any.hpp>
#include <istream>
#include <physfs.h>


using std::string;
using std::tr1::unordered_map;
using std::tr1::shared_ptr;
using boost::any;
using boost::any_cast;
using std::istream;

typedef int resource_id;

/**
 * A resource managing class that can store arbitrary resource types
 * which are managed by shared pointers.
 */
class resource_manager {
	public:
		template <typename T>
		resource_id load_resource(const string& filename, const string& name);

		template <typename T, typename U>
		resource_id load_resource(const string& filename, const string& name);

		template <typename T>
		T* get_resource(resource_id ID);

		template <typename T>
		T* get_resource_by_name(const string& name);

		bool exists(const string& filename) const; //Returns true if the file is available for loading

		bool has_resource(const string& name); //Do we have a resource with this handle
		bool has_resource(resource_id ID); //Do we have a resource with this ID

		resource_id get_resource_id(const string& name); //Get a resource ID by name

		void add_to_search_path(const string&); //Add a folder or zip file to search

		void destroy_resource(const resource_id r); //Deletes a loaded resource

		static resource_id get_next_resource_id();

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

	private:
		unordered_map<string, resource_id> m_name_lookup;
		unordered_map<resource_id, any> m_resources;
		static resource_id s_next_resource_id;
		static bool s_was_initialized;

		shared_ptr<istream> get_stream_from_filename(const string& filename);
};

#include "resourcemanager-tpl.h"

#endif
