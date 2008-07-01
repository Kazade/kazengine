#include "utilities/logger.h"



	map<string, shared_ptr<Logger> > Logger::s_Loggers;
	string Logger::s_DefaultPath;

	Logger::Logger() {

	}

	Logger::~Logger() {

	}

