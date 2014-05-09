#include <estacion/ListaEntero.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <system/SemaphoreLocker.h>

ListaEntero::ListaEntero (IPCName name, int flags, int capacity, Semaphore& lock)
	: lock (lock)
	, lista (name, flags, capacity + 1)
	, capacity (capacity)
{
	if ((flags & IPC_CREAT) != 0) {
		for (int i=0; i <= capacity; i++) {
			lista[i] = EMPTY;
		}
	}
}

ListaEntero::~ListaEntero ()
{
}

int ListaEntero::peek () const
{
	SemaphoreLocker locker (lock);
	return lista[0];
}

int ListaEntero::take ()
{
	SemaphoreLocker locker (lock);
	int head = lista[0];
	if (head != EMPTY) {
		lista[0] = lista[head];
	}
	return head;
}

void ListaEntero::put (int i)
{
	SemaphoreLocker locker (lock);
	int head = lista[0];
	lista[i] = head;
	lista[0] = i;
}

void ListaEntero::debug ()
{
	Logger& logger = LoggerRegistry::getLogger ("ListaEntero");
	SemaphoreLocker locker (lock);

	logger << "Arreglo: ";
	for (int i=0; i < capacity; i++) {
		logger << lista[i];
		logger << ", ";
	}
	logger << lista[capacity] << Logger::endl;

	logger << "Lista: ";

	int iter = lista[0];
	logger << iter;
	while (iter != -1) {
		logger << " -> " << lista[iter];
		iter = lista[iter];
	}
	logger << Logger::endl;
}
