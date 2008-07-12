#include <fstream>
#include <iostream>
#include <sstream>

#include "resource_manager.h"

using std::istringstream;
using std::ifstream;

bool resource_manager::s_was_initialized = false;

resource_manager::resource_manager() {

}

/**
	guarentee: basic
*/
resource_id resource_manager::queue_file_for_loading(const string& filename,
					shared_ptr<resource_interface> new_res, shared_ptr<boost::mutex> res_mutex) {

	queued_resource queued_res;
	queued_res.filename = filename;
	queued_res.res = new_res;
	queued_res.mutex = res_mutex;
	queued_res.id = generate_next_id();

	{
		boost::mutex::scoped_lock lock(m_load_queue_mutex);
		m_load_queue.push_back(queued_res);
	}

	return queued_res.id;
}

auto_ptr<resource_manager::queued_resource> resource_manager::get_next_resource_to_load() {
	auto_ptr<queued_resource> result;

	//We lock (the user might be adding stuff to the queue
	boost::mutex::scoped_lock lock(m_load_queue_mutex);
	//Get a pointer to the front of the queue
	if (!m_load_queue.empty()) {
		//If there are some items in the queue, get the first one
		//and remove it from the queue.

		//If this throws, the program will be in the same state as before (strong guarantee)
		result = auto_ptr<queued_resource>(new queued_resource(m_load_queue.front()));

		m_load_queue.pop_front();
	}
	//We can now unlock the mutex ready for more load requests

	return result;
}

void resource_manager::load_resource_from_stream(
																	auto_ptr<queued_resource>& currently_loading,
																	shared_ptr<istream> stream) {
	//Lock ready for load
	boost::mutex::scoped_lock current_lock(*currently_loading->mutex);

	resource_id current_id = currently_loading->id;
	m_load_status[current_id] = FILE_LOAD_IN_PROGRESS;

	//Attempt the load
	if (!currently_loading->res->load(*stream)) {
		//If it failed then we mark as invalid
		m_load_status[current_id] = FILE_LOAD_FAILED;
	} else {
		//Lock the resources list
		boost::mutex::scoped_lock resources_lock(m_resources_mutex);

		//Insert the new resource into the list
		// I can find no evidence anywhere that insert can throw a bad_alloc,
		// yet, it's  logical that it must do if it can't allocate the memory
		try {
			m_resources.insert(std::make_pair(currently_loading->id,
																										currently_loading->res));
			//Set the load status to successful
			m_load_status[current_id] = FILE_LOAD_SUCCESS;

			//Associate the ID of the new resource with its filename
			m_file_resource_lookup[currently_loading->filename] = current_id;

		} catch (std::bad_alloc& e) {
			//If we couldn't insert into the map, we mark the load as failed
			m_load_status[current_id] = FILE_LOAD_FAILED;
		}

	} //unlock m_resources
}

void resource_manager::do_run() {
	//Create a variable to store the resource we are currently loading
	auto_ptr<queued_resource> currently_loading;
	currently_loading = get_next_resource_to_load();

	//If we found a resource to load
	if (currently_loading.get()) {
		resource_id current_id = currently_loading->id;

		//create a file stream
		shared_ptr<istream> stream = get_stream_from_file(currently_loading->filename);

		if (!stream) {
			//The file didnt exist or could not be opened for reading
			//so we destroy the currently_loaded resource and mark it as
			//invalid (we can't throw an exception because of the whole thread thing)
			m_load_status[current_id] = FILE_NOT_FOUND;
		} else {
			load_resource_from_stream(currently_loading, stream);
		}
	}

	//If we have just loaded a file, mark it as finished loading
	if (currently_loading.get()) {
		boost::mutex::scoped_lock finished_lock(m_finished_loading_mutex);
		m_resources_finished_loading.push_back(currently_loading->id);
	}

	sleep(0);
	//We now continue around the loop
}

bool resource_manager::has_resource_loading_finished(const resource_id id) const {
	boost::mutex::scoped_lock finished_lock(m_finished_loading_mutex);
	bool result = (std::find(m_resources_finished_loading.begin(),
																m_resources_finished_loading.end(),id) != m_resources_finished_loading.end());
	return result;
}

bool resource_manager::is_resource_loaded(const resource_id& id) const {
	unordered_map<resource_id, file_load_status>::const_iterator i;
	i = m_load_status.find(id);
	if (i == m_load_status.end()) {
		return false;
	}

	return ((*i).second == FILE_LOAD_SUCCESS);
}

file_load_status resource_manager::get_resource_load_status(const resource_id id) const {
	unordered_map<resource_id, file_load_status>::const_iterator i;

	i = m_load_status.find(id);

	if (i == m_load_status.end()) {
		throw std::logic_error("Load status not available for resource id");
	}

	return (*i).second;
}

shared_ptr<resource_interface> resource_manager::get_resource(const resource_id& id) {
	shared_ptr<resource_interface> result;

	if (m_load_status[id] == FILE_LOAD_SUCCESS) {
		unordered_map< resource_id, shared_ptr<resource_interface> >::iterator i;
		//We lock here, we dont know if m_resources is being altered in
		//another thread
		boost::mutex::scoped_lock lock(m_resources_mutex);

		i = m_resources.find(id);
		if (i != m_resources.end()) {
			result = (*i).second;
		}
	}

	//Return a shared pointer to the resource
	return result;
}


/**
 * Reads a file using PhysFS and returns a pointer to an inputstream
 * to it
 */
shared_ptr<istream> resource_manager::get_stream_from_file(const string& filename) {
		if (!PHYSFS_exists(filename.c_str())) {
			return shared_ptr<istream>();
		}

		PHYSFS_file* myfile = PHYSFS_openRead(filename.c_str());
		PHYSFS_sint64 file_size = PHYSFS_fileLength(myfile);

		vector<char> buffer(file_size);
		PHYSFS_read (myfile, &buffer[0], 1, file_size);
		string stringBuf(buffer.begin(), buffer.begin() + file_size);
		PHYSFS_close(myfile);

		shared_ptr<istringstream> iss(new istringstream(stringBuf));
		return iss;
}
