#include <stdexcept>
#include <vector>
#include <sstream>

#include "resourcemanager.h"

using std::vector;
using std::logic_error;
using std::istringstream;

resource_id resource_manager::s_next_resource_id = 0;
bool resource_manager::s_was_initialized = false;

resource_id resource_manager::get_next_resource_id() {
	return s_next_resource_id++;
}

bool resource_manager::has_resource(const string& name) {
	return (m_name_lookup.find(name) != m_name_lookup.end());
}

bool resource_manager::has_resource(resource_id ID) {
	return (m_resources.find(ID) != m_resources.end());
}

bool resource_manager::exists(const string& filename) const {
	return PHYSFS_exists(filename.c_str());
}

resource_id resource_manager::get_resource_id(const string& name) {
	if (!has_resource(name)) {
		throw logic_error("Resource does not exist");
	}

	return m_name_lookup[name];
}

void resource_manager::destroy_resource(const resource_id r) {
	if (m_resources.find(r) == m_resources.end()) {
		throw logic_error("Resource does not exist");
	}

	m_resources.erase(r);
}

void resource_manager::add_to_search_path(const string& path) {
	PHYSFS_addToSearchPath(path.c_str(), 1);
}

/**
 * Reads a file using PhysFS and returns a pointer to an inputstream
 * to it
 */
shared_ptr<istream> resource_manager::get_stream_from_filename(const string& filename) {
		if (!PHYSFS_exists(filename.c_str())) {
			return shared_ptr<istream>();
		}

		PHYSFS_file* myfile = PHYSFS_openRead(filename.c_str());
		PHYSFS_sint64 fileSize = PHYSFS_fileLength(myfile);

		vector<char> buffer(fileSize);
		PHYSFS_read (myfile, &buffer[0], 1, fileSize);
		string stringBuf(buffer.begin(), buffer.begin() + fileSize);
		PHYSFS_close(myfile);

		shared_ptr<istringstream> iss(new istringstream(stringBuf));
		return iss;
}
