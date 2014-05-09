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
	: fd (-1)
	, filename (filename)
	, module (module)
	, _quiet (true)
	, printedModule(false)
{
}

Logger::Logger (const std::string& filename, const std::string& module, bool quiet)
	: fd (-1)
	, filename (filename)
	, module (module)
	, _quiet (quiet)
	, printedModule(false)
{
	if (!_quiet) {
		open ();
	}
}

Logger::~Logger ()
{
	if (fd != -1) {
		close (fd);
	}
}

void Logger::open ()
{
	fd = ::open (filename.c_str ()
		, O_CREAT | O_CLOEXEC | O_WRONLY | O_APPEND
		, S_IRUSR | S_IWUSR);
	System::check (fd);
}

void Logger::lock () const
{
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = 0;
	lck.l_len = 0;
	int err = fcntl (fd, F_SETLKW, &lck);
	System::check (err);
}

void Logger::unlock () const
{
	struct flock lck;
	lck.l_type = F_UNLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = 0;
	lck.l_len = 0;
	fcntl (fd, F_SETLK, &lck);
}

Logger& Logger::endl (Logger& logger)
{
	logger.buffer << std::endl;

	std::string line = logger.buffer.str();
	const char* data = line.c_str ();
	size_t remaining = line.size ();
	ssize_t written;

	if (logger.fd == -1) {
		logger.open ();
	}

	{
		Locker grabLock (logger);
		do {
			written = write (logger.fd, data, remaining);
			System::check (written);
			remaining -= written;
		} while (remaining > 0);
	}

	logger.buffer.str ("");
	logger.printedModule = false;
	return logger;
}

Logger& operator<< (Logger& logger, Logger& (*pf)(Logger&))
{
	if (logger.quiet ()) {
		return logger;
	}

	return pf (logger);
}
