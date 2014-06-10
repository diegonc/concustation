#ifndef IPCNAME_H
#define IPCNAME_H

struct IPCName
{
	const char* path;
	const char  index;

	IPCName (const char *path, const char index)
		: path (path), index (index) { }
};

#endif
