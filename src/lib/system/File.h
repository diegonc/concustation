#ifndef FILE_H
#define FILE_H

#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <utils/NonCopyable.h>

class File : private NonCopyable
{
	private:
		int fd;

		void inicializar (const char *path, int flags, mode_t mode);

	public:
		File (const std::string &path, int flags);
		File (const std::string &path, int flags, mode_t mode);
		File (const char *path, int flags);
		File (const char *path, int flags, mode_t mode);
		~File ();

		ssize_t write (size_t n, const char *data);
		ssize_t read (size_t n, char *data);
};

#endif
