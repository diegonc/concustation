#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <system/System.h>
#include <unistd.h>

SystemErrorException::SystemErrorException () throw ()
	: number (errno)
{
}

SystemErrorException::SystemErrorException (int err) throw ()
	: number (err)
{
}

pid_t System::spawn (const char *file, char* const argv[])
{
	pid_t pid;
	bool quiet = LoggerRegistry::getInstance ().quiet ();

	pid = fork ();
	if (pid == 0) {
		int err = execvp (file, argv);
		SystemErrorException e;
		LoggerRegistry& r = LoggerRegistry::getInstance ();
		r.filename (estacion::LOG_FILE);
		r.application ("spawned");
		r.quiet (quiet);
		Logger& logger = LoggerRegistry::getLogger ("System");
		logger << "Error ejecutando " << file << " : (" << err << ") "
		       << e.what () << Logger::endl;
		_exit (1);
	}
	return pid;
}
