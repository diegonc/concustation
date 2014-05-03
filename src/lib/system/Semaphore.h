#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#include <system/IPCName.h>
#include <utils/NonCopyable.h>

class Semaphore : private NonCopyable
{
	private:
		bool persistent;
		int id;
		const int nsems;

	public:
		Semaphore (key_t key, int nsems, int flags);
		Semaphore (IPCName name, int nsems, int flags);
		~Semaphore ();

		void persist ();
		void initialize ();

		void set (unsigned short idx, short value);
		int wait (unsigned short idx = 0, short value = 1);
		void signal (unsigned short idx = 0, short value = 1);
};

#endif
