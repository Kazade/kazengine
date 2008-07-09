#include <fstream>
#include <iostream>

#include "resource_manager.h"

using std::ifstream;

bool resource_manager::s_was_initialized = false;

resource_manager::resource_manager():
m_currently_loading(NULL) {

}

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

void resource_manager::do_run() {
	//If there wasn't a blocked resource load waiting, then we attempt to load
	//the next from the queue
	queued_resource* currently_loading = NULL;

	if (!currently_loading) {
		{ //We lock again (the user might be adding stuff to the queue
			boost::mutex::scoped_lock lock(m_load_queue_mutex);
			//Get a pointer to the front of the queue
			if (!m_load_queue.empty()) {
				currently_loading = &m_load_queue.front();
			}
			//We can now unlock the mutex ready for more load requests
		}
	}

	if (currently_loading) {
		//By here we have a pointer in m_currently_loading which cannot be accessed
		//from another thread so no more locks are needed except the one
		//associated with the resource itself

		//create a file stream

		ifstream stream(currently_loading->filename.c_str(), std::ios::binary);
		resource_id current_id = currently_loading->id;

		if (!stream.is_open()) {
			//The file didnt exist or could not be opened for reading
			//so we destroy the currently_loaded resource and mark it as
			//invalid
			m_load_status[current_id] = FILE_NOT_FOUND;
		} else {
			//Lock ready for load
			boost::mutex::scoped_lock current_lock(*currently_loading->mutex);
			m_load_status[current_id] = FILE_LOAD_IN_PROGRESS;

			//Attempt the load
			if (!currently_loading->res->load(stream)) {
				//If it failed then we mark as invalid
				m_load_status[current_id] = FILE_LOAD_FAILED;
			} else {
				//Lock the resources list
				boost::mutex::scoped_lock resources_lock(m_resources_mutex);
				//Insert a pointer to the resource (this increments the reference count
				//so the resource is no longer destroyed at the end of load_resource)
				m_resources.insert(std::make_pair(currently_loading->id, currently_loading->res));
				m_load_status[current_id] = FILE_LOAD_SUCCESS;
				m_file_resource_lookup[currently_loading->filename] = current_id;
			} //unlock m_resources
		} //Unlock m_currently_loading
	}

	boost::mutex::scoped_lock lock(m_load_queue_mutex);

	if (!m_load_queue.empty()) {
		m_load_queue.pop_front();
	}

	if (currently_loading) {
		boost::mutex::scoped_lock finished_lock(m_finished_loading_mutex);
		m_resources_finished_loading.push_back(currently_loading->id);
	}

	currently_loading = NULL;
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

resource_interface* resource_manager::get_resource(const resource_id& id) {
	if (m_load_status[id] != FILE_LOAD_SUCCESS) {
		return NULL;
	}

	unordered_map< resource_id, shared_ptr<resource_interface> >::iterator i;

	//We lock here, we dont know if m_resources is being altered in
	//another thread
	boost::mutex::scoped_lock lock(m_resources_mutex);

	i = m_resources.find(id);
	if (i == m_resources.end()) {
		return NULL;
	}

	//Return a raw pointer to the resource
	return (*i).second.get();
}
