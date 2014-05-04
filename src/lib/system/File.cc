#include <system/File.h>
#include <system/System.h>

File::File (const std::string &path, int flags)
	: fd (-1)
{
	inicializar (path.c_str(), flags, 0);
}

File::File (const std::string &path, int flags, mode_t mode)
	: fd (-1)
{
	inicializar (path.c_str(), flags, mode);
}

File::File (const char *path, int flags)
	: fd (-1)
{
	inicializar (path, flags, 0);
}

File::File (const char *path, int flags, mode_t mode)
	: fd (-1)
{
	inicializar (path, flags, mode);
}

void File::inicializar (const char *path, int flags, mode_t mode)
{
	fd = open (path, flags, mode);
	System::check (fd);
}

File::~File()
{
	if (fd != -1) {
		close (fd);
	}
}

ssize_t File::write (size_t n, const char *data)
{
	size_t restantes = n;
	ssize_t escritos = 0;
	while (restantes > 0) {
		escritos = ::write (fd, data, restantes);
		if (escritos == 0 || escritos == -1) {
			break;
		}
		restantes -= escritos;
		data += escritos;
	}
	return escritos;
}

ssize_t File::read (size_t n, char *data)
{
	return ::read (fd, data, n);	
}
