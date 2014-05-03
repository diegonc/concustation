#ifndef SHAREDVARIABLE_H
#define SHAREDVARIABLE_H

#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <system/IPCName.h>
#include <system/System.h>
#include <utils/NonCopyable.h>

template <typename T>
class SharedArray : private NonCopyable
{
	private:
		int id;
		bool _persistent;
		T *ptr;

	public:
		SharedArray (IPCName name, int flags, int numItems);
		~SharedVariable ();

		void persistent () { _persistent = true; }

		T& operator [] (const unsigned i) { return ptr[i]; }
};

template <typename T>
SharedArray<T>::SharedArray (IPCName name, int flags, int numItems)
	: _persistent (false)
{
	Logger& logger = LoggerRegistry::getLogger ("SharedArray");
	logger << "Creando arreglo compartido ["
	       << name.path << ":" << name.index << "]" << Logger::endl;

	key_t token = ftok (name.path, name.index);
	logger << "ftok devolvió " << token << Logger::endl;
	System::check (token);

	id = shmget (token, sizeof(T) * numItems, flags);
	logger << "shmget devolvió " << id << Logger::endl;
	System::check (id);

	ptr = static_cast<T *>(shmat (id, NULL, 0));
	logger << "shmat devolvió " << ptr << Logger::endl;
	if (ptr == (T *)-1) {
		int err = errno;
		shmctl (id, IPC_RMID, NULL);
		throw SystemErrorException (err);
	}
}

template <typename T>
SharedArray<T>::~SharedArray ()
{
	shmdt (ptr);
	if (!_persistent) {
		shmctl (id, IPC_RMID, NULL);
	}
}

#endif
