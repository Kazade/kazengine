#ifndef ILOGHOLDER_H_INCLUDED
#define ILOGHOLDER_H_INCLUDED

#include <tr1/memory>
#include "utilities/logger.h"

using std::tr1::shared_ptr;

class ILogHolder {
	public:
		virtual ~ILogHolder() {}

		void setLog(shared_ptr<Logger> log) { m_Logger = log; }
	private:
		shared_ptr<Logger> m_Logger;

	protected:
		shared_ptr<Logger> getLog() { return m_Logger; }
};

#endif // ILOGHOLDER_H_INCLUDED
