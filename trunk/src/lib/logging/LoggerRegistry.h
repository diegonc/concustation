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
		const std::string& filename () const
		{
			return _filename;
		}

		void application (const std::string& theApplication)
		{
			_application = theApplication;
		}

		const std::string& application () const
		{
			return _application;
		}

		void quiet (bool isQuiet)
		{
			_quiet = isQuiet;
		}

		bool quiet () const
		{
			return _quiet;
		}
};

#endif // LOGGING_LOGGERREGISTRY_H
