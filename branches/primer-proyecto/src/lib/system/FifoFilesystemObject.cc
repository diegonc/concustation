#include <system/FifoFilesystemObject.h>
#include <system/System.h>
#include <unistd.h>

FifoFilesystemObject::FifoFilesystemObject (const std::string& path, int flags)
	: path (path)
	, _persist (false)
{
	System::check (mkfifo (path.c_str (), flags));
}

FifoFilesystemObject::~FifoFilesystemObject ()
{
	if (!_persist) {
		unlink (path.c_str ());
	}
}
