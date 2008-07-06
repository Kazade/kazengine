#ifndef CUSTOMEXCEPT_H_INCLUDED
#define CUSTOMEXCEPT_H_INCLUDED

#include <stdexcept>

class not_implemented_error : public std::logic_error {
	public:
		not_implemented_error(const string& m="NotImplementedError"):
		logic_error(m) {}
};

#endif // CUSTOMEXCEPT_H_INCLUDED
