#ifndef FIFOFILESYSTEMOBJECT_H
#define FIFOFILESYSTEMOBJECT_H

#include <string>
#include <sys/stat.h>
#include <utils/NonCopyable.h>

class FifoFilesystemObject : private NonCopyable
{
	std::string path;
	bool _persist;

	public:
		FifoFilesystemObject (const std::string& path, int flags);
		~FifoFilesystemObject ();

		/* El objeto permanecera en el sistema de ficheros
		 * luego de que esta instancia sea destruida.
		 */
		void persist () { _persist = true; }

		const std::string& getPath () const { return path; }
};

#endif
