#ifndef LOGGING_LOGGERREGISTRY_H
#define LOGGING_LOGGERREGISTRY_H

#include <logging/Logger.h>
#include <map>
#include <string>
#include <utils/NonCopyable.h>

class LoggerRegistry : private NonCopyable
{
	std::map<std::string, Logger*> registry;
	std::string _filename;
	std::string _application;
	bool _quiet;

	LoggerRegistry ();
	~LoggerRegistry ();

	public:
		static LoggerRegistry& getInstance ();
		static Logger& getLogger (const std::string& name);

		void filename (const std::string& theFileName)
		{
			_filename = theFileName;
		}

		void application (const std::string& theApplication)
		{
			_application = theApplication;
		}

		void quiet (bool isQuiet)
		{
			_quiet = isQuiet;
		}
};

#endif // LOGGING_LOGGERREGISTRY_H
