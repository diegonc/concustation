#include <estacion/ListaEntero.h>
#include <system/SemaphoreLocker.h>

ListaEntero::ListaEntero (IPCName name, int flags, int capacity, Semaphore& lock)
	: lock (lock)
	, lista (name, flags, capacity + 1)
{
	for (int i=0; i <= capacity; i++) {
		lista[i] = EMPTY;
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
	int head = peek ();
	lista[0] = lista[head];
	return head;
}

void ListaEntero::put (int i)
{
	SemaphoreLocker locker (lock);
	int head = peek ();
	lista[i] = head;
	lista[0] = i;
}
