#include <errno.h>
#include <iostream>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <system/Semaphore.h>
#include <system/System.h>

namespace {
	union semun {
		int val;
		struct semid_ds *buf;
		unsigned short  *array;
	};
}

Semaphore::Semaphore (IPCName name, int nsems, int flags) : nsems (nsems)
{
	Logger& logger = LoggerRegistry::getLogger ("Semaphore");

	logger << "Creando semáforo ["
	       << name.path << "::" << name.index << "]" << Logger::endl;

	key_t token = ftok (name.path, name.index);
	System::check (token);
	id = semget (token, nsems, flags);
	System::check (id);
	persistent = false;
}

Semaphore::Semaphore (key_t key, int nsems, int flags) : nsems (nsems)
{
	id = semget (key, nsems, flags);
	System::check (id);
	persistent = false;
}

Semaphore::~Semaphore ()
{
	if (!persistent) {
		union semun arg = {0};
		semctl (id, 0, IPC_RMID, arg);
	}
}

void Semaphore::initialize ()
{
	for (unsigned short i=0; i < nsems; i++)
		set (i, 0);
}

void Semaphore::persist ()
{
	persistent = true;
}

void Semaphore::set (unsigned short idx, short value)
{
	union semun arg;
	arg.val = value;
	semctl (id, idx, SETVAL, arg);
}

void Semaphore::wait (unsigned short idx, short value)
{
	if (value < 0)
		throw std::invalid_argument ("Semaphore::wait: value must be positive.");

	struct sembuf ops;

	ops.sem_num = idx;
	ops.sem_op = static_cast<short>(-value);
	ops.sem_flg = 0;
	
	bool retry;
	do {
		retry = false;

		int ret = semop (id, &ops, 1);
		if (ret == -1) {
			if (errno == EINTR) {
				retry = true;
			} else
				throw SystemErrorException ();
		}
	} while (retry);
}

void Semaphore::signal (unsigned short idx, short value)
{
	if (value < 0)
		throw std::invalid_argument ("Semaphore::signal: value must be positive.");

	struct sembuf ops;

	ops.sem_num = idx;
	ops.sem_op = value;
	ops.sem_flg = 0;
	System::check (semop (id, &ops, 1));
}
