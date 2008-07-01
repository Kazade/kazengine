#ifndef IRESOURCE_H_INCLUDED
#define IRESOURCE_H_INCLUDED

#include <istream>
#include <string>

using std::istream;
using std::string;

enum file_load_status {
	FILE_NOT_FOUND,
	FILE_LOAD_FAILED,
	FILE_LOAD_SUCCESS
};

/**
Describes an interface that all resources should implement
*/
class resource_interface {
	public:
		virtual ~resource_interface() {} ///< virtual destructor

		virtual file_load_status load(istream& stream) = 0;
		virtual void unload() = 0;
		virtual string get_last_error() = 0;

};


#endif // IRESOURCE_H_INCLUDED
