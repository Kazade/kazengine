#include <cassert>
#include <iostream>
#include "iresource.h"

template <typename T>
resource_id resource_manager::load_resource(const string& filename, const string& name) {
	return load_resource<T, T>(filename, name);
}

template <typename T, typename U>
resource_id resource_manager::load_resource(const string& filename, const string& name) {
	shared_ptr<T> new_resource(new U); //Create the new resource

	//new_resource->set_resource_manager(this);

	resource_id new_id = get_next_resource_id(); //Get an ID for this resource

	//See if we can get a stream to the file
	shared_ptr<istream> stream = get_stream_from_filename(filename);

	file_load_status status = FILE_LOAD_SUCCESS;

	if (!stream) {
		//The file was not available for loading
		status = FILE_NOT_FOUND;
		return -1;
	}

	//Load using the stream
	status = new_resource->load(*stream.get());
	stream.reset(); //Destroy the stream

	if (status != FILE_LOAD_SUCCESS) {
		return -1;
	}

	m_resources[new_id] = new_resource;
	m_name_lookup[name] = new_id;
	return new_id;
}

template <typename T>
T* resource_manager::get_resource(resource_id ID) {
	if (ID == -1) {
		return NULL;
	}

	//Get the resource from the hashmap
	shared_ptr<T> resource = any_cast<shared_ptr<T> > (m_resources[ID]);

	//Return a pointer, this is so we can destroy the resource when we want
	return resource.get();
}

template <typename T>
T* resource_manager::get_resource_by_name(const string& name) {
	return get_resource<T>(get_resource_id(name));
}
