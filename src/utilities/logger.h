#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <tr1/memory>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <cassert>

using std::tr1::shared_ptr;
using std::string;
using std::map;
using std::ofstream;
using boost::filesystem::path;

#define CURRENT_LEVEL LL_DEBUG


	enum LOG_LEVELS {
		LL_ERROR = 0,
		LL_WARNING,
		LL_INFO,
		LL_DEBUG
	};


	class Logger : boost::noncopyable {
		private:
			string m_Name;
			string m_Path;
			uint m_Level;

			static std::map<string, shared_ptr<Logger> > s_Loggers;
			static string s_DefaultPath;

			void writeLine(const string& msg, const string& prefix) {
				ofstream fileOut(m_Path.c_str(), std::ios::app);

				assert(fileOut.is_open());

				fileOut << prefix << getPreamble() << msg << "\n";
				fileOut.close();
			}

		public:
			Logger();
			~Logger();

			static shared_ptr<Logger> getLogger(const string& name) {
				if ((!s_Loggers.empty()) && s_Loggers.find(name) != s_Loggers.end()) {
					return s_Loggers[name];
				}
				//else create a new logger
				path filepath(s_DefaultPath);
				filepath = filepath / (name + ".txt");

				std::cout << filepath.string() << std::endl;

				shared_ptr<Logger> newLogger(new Logger());
				newLogger->m_Path = filepath.string();
				newLogger->m_Name = name;
				newLogger->m_Level = LL_ERROR;
				s_Loggers[name] = newLogger;

				return newLogger;
			}

			static void setDefaultPath(const string& path) {
				s_DefaultPath = path;
			}

			void setLevel(LOG_LEVELS level) {
				m_Level = level;
			}

			string getPreamble() {
				time_t rawtime;
				struct tm * timeinfo;

				time(&rawtime);
				timeinfo = localtime(&rawtime);
				string result = string(asctime(timeinfo));
				result.replace(result.find_last_of("\n"), 1, " ");

				return result;
			}

			void error(const string& msg) {
				writeLine(msg, "Error ");
			}

			void warning(const string& msg) {
				if (m_Level < LL_WARNING)
					return;

				writeLine(msg, "Warning ");
			}

			void info(const string& msg) {
				if (m_Level < LL_INFO)
					return;

				writeLine(msg, "Info ");
			}

			void debug(const string& msg) {
				if (m_Level < LL_DEBUG)
					return;

				writeLine(msg, "Debug ");
			}

			void clear() {
				ofstream file(m_Path.c_str());
				file << "Start of log\n";
				file.close();
			}
	};



#endif // LOGGER_H_INCLUDED
