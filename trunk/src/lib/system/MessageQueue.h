#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <system/IPCName.h>
#include <system/System.h>
#include <utils/NonCopyable.h>

template <typename T>
class MessageQueue : private NonCopyable
{
	private:
		int id;
		bool _persistent;

	public:
		MessageQueue (IPCName name, int flags);
		~MessageQueue ();

		void persist () { _persistent = true; }

		void send (const T& msg);
		T receive (long msgtyp);
};

template <typename T>
MessageQueue<T>::MessageQueue (IPCName name, int flags)
	: _persistent (false)
{
	Logger& logger = LoggerRegistry::getLogger ("MessageQueue");
	logger << "Creando cola de mensajes ["
	       << name.path << ":" << name.index << "]" << Logger::endl;

	key_t token = ftok (name.path, name.index);
	logger << "ftok devolvió " << token << Logger::endl;
	System::check (token);

	id = msgget (token, flags);
	logger << "msgget devolvió " << id << Logger::endl;
	System::check (id);
}

template <typename T>
MessageQueue<T>::~MessageQueue ()
{
	if (!_persistent) {
		msgctl (id, IPC_RMID, NULL);
	}
}

template <typename T>
void MessageQueue<T>::send (const T& msg)
{
	int err;
	err = msgsnd (id, (const void*) &msg, sizeof (T) - sizeof (long), 0);
	System::check (err);
}

template <typename T>
T MessageQueue<T>::receive (long msgtyp)
{
	T msg;
	ssize_t err;
	err = msgrcv (id, (void *) &msg, sizeof (T) - sizeof (long), msgtyp, 0);
	System::check (err);
	return msg;
}

#endif /* MESSAGEQUEUE_H */
