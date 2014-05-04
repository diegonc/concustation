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

	pid = fork ();
	if (pid == 0) {
		int err = execvp (file, argv);
		/* TODO: log error */
		(void) err;
		_exit (1);
	}
	return pid;
}
