#include <fstream>

#include "resource_manager.h"

using std::ifstream;

bool resource_manager::s_was_initialized = false;

void resource_manager::do_run() {
	/**
		1. Check to see if there are any blocking resources to load
			a. if so, copy it to m_currently_loading
			b. else
				i. check if there are any resources in the queue
					1. If so, get the next one and assign it to m_currently_loading
		2. If there are no resources: sleep
		3. else
			a. lock the file
			b. create a progress indicator
			c. load the resource
			d. unlock the file
			e. copy resource to m_resources
			f. mark the resource as loaded */


	//First we create a lock so we can check if there are any blocking
	//resources to load (i.e. the user is waiting for load_resource to return)
	{ //Start of lock block
		boost::mutex::scoped_lock lock(m_blocked_resource_mutex);
		if (m_blocked_resource) {
			//Copy the blocked resource to the currently loaded one
			m_currently_loading = m_blocked_resource;
			m_blocked_resource.reset(); //Nullify this ready for another load_resource
		}
	} //End of lock block

	//If there wasn't a blocked resource load waiting, then we attempt to load
	//the next from the queue
	if (!m_currently_loading) {
		{ //We lock again (the user might be adding stuff to the queue
			boost::mutex::scoped_lock lock(m_load_queue_mutex);
			//Get and remove the item at the front of the queue
			m_currently_loading = m_load_queue.front();
			m_load_queue.pop();
			//We can now unlock the mutex ready for more load requests
		}
	}

	//By here we have a pointer in m_currently_loading which cannot be accessed
	//from another thread so no more locks are needed except the one
	//associated with the resource itself

	//create a file stream

	ifstream stream(m_currently_loading->filename.c_str(), std::ios::binary);

	resource_id current_id = m_currently_loading->id;

	if (!stream.is_open()) {
		//The file didnt exist or could not be opened for reading
		//so we destroy the currently_loaded resource and mark it as
		//invalid
		m_load_status[current_id] = FILE_NOT_FOUND;
		m_currently_loading.reset();
	}

	if (m_currently_loading) {
		{
			//Lock ready for load
			boost::mutex::scoped_lock current_lock(*m_currently_loading->mutex);


			m_load_status[current_id] = FILE_LOAD_IN_PROGRESS;

			//Attempt the load
			if (!(*m_currently_loading->res)->load(stream)) {
				//If it failed then we mark as invalid
				m_load_status[current_id] = FILE_LOAD_FAILED;
				m_currently_loading.reset();
				return; //Go to the next iteration
			}

			{ 		//Lock the resources list
				boost::mutex::scoped_lock resources_lock(m_resources_mutex);
				//Insert a pointer to the resource (this increments the reference count
				//so the resource is no longer destroyed at the end of load_resource)
				m_resources.insert(std::make_pair(m_currently_loading->id, *m_currently_loading->res));
				m_load_status[current_id] = FILE_LOAD_SUCCESS;
			} //unlock m_resources

			m_currently_loading.reset();
		} //Unlock m_currently_loading
	}
	//We now continue around the loop
}


file_load_status resource_manager::get_resource_load_status(const resource_id id) const {
	unordered_map<resource_id, file_load_status>::const_iterator i;

	i = m_load_status.find(id);

	if (i == m_load_status.end()) {
		throw std::logic_error("Load status not available for resource id");
	}

	return (*i).second;
}
