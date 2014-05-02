#include <fcntl.h>
#include <logging/Logger.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <system/System.h>

class Locker
{
	const Logger& logger;

	public:
		Locker (const Logger& logger)
			: logger (logger)
		{
			logger.lock ();
		}
		~Locker ()
		{
			logger.unlock ();
		}
};

Logger::Logger (const std::string& filename, const std::string& module)
	: module (module)
	, printedModule(false)
{
	fd = open (filename.c_str ()
		, O_CREAT | O_CLOEXEC | O_WRONLY | O_APPEND
		, S_IRUSR | S_IWUSR);
	System::check (fd);
}

Logger::~Logger ()
{
	close (fd);
}

void Logger::lock () const
{
	// TODO: implementar bloqueo
}

void Logger::unlock () const
{
	// TODO: implementar bloqueo
}

Logger& Logger::endl (Logger& logger)
{
	logger.buffer << std::endl;

	std::string line = logger.buffer.str();
	const char* data = line.c_str ();
	size_t remaining = line.size ();
	ssize_t written;

	Locker grabLock (logger);
	do {
		written = write (logger.fd, data, remaining);
		System::check (written);
		remaining -= written;
	} while (remaining > 0);

	logger.buffer.str ("");
	logger.printedModule = false;
	return logger;
}

Logger& operator<< (Logger& logger, Logger& (*pf)(Logger&))
{
	return pf (logger);
}
