#ifndef CUSTOMEXCEPT_H_INCLUDED
#define CUSTOMEXCEPT_H_INCLUDED

class not_implemented_error : public std::logic_error {
	public:
		not_implemented_error(const string& m="NotImplementedError"):
		m_message(m) {}

		~not_implemented_error() throw() {}

		const char* what() const throw() {
			return m_message.c_str()
		}
	private:
		string m_message;
};

#endif // CUSTOMEXCEPT_H_INCLUDED
