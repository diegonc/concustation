#include <logging/LoggerRegistry.h>
#include <sstream>
#include <stdexcept>

LoggerRegistry::LoggerRegistry ()
	: _quiet (true)
{
}

LoggerRegistry::~LoggerRegistry ()
{
	std::map<std::string, Logger*>::iterator it = registry.begin ();
	while (it != registry.end ()) {
		delete it->second;
		it++;
	}
}

LoggerRegistry& LoggerRegistry::getInstance ()
{
	static LoggerRegistry instance;
	return instance;
}

Logger& LoggerRegistry::getLogger (const std::string& name)
{
	LoggerRegistry& registry = LoggerRegistry::getInstance ();

	if (registry._filename.size () == 0) {
		throw std::logic_error ("LoggerRegistry is not initialized: missing filename parameter"); 
	}

	if (registry._application.size () == 0) {
		throw std::logic_error ("LoggerRegistry is not initialized: missing application parameter"); 
	}

	std::ostringstream loggerName;
	loggerName << registry._application << " - " << name;

	try {
		return *registry.registry.at (loggerName.str ());
	} catch (std::out_of_range& e) {
		Logger* logger = new Logger (
			  registry._filename
			, loggerName.str ()
			, registry._quiet);
		registry.registry[loggerName.str ()] = logger;
		return *logger;
	}
}
