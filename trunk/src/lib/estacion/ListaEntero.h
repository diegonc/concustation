#ifndef ESTACION_LISTAENTERO_H
#define ESTACION_LISTAENTERO_H

#include <system/IPCName.h>
#include <system/Semaphore.h>
#include <system/SharedArray.h>
#include <utils/NonCopyable.h>

class ListaEntero : private NonCopyable
{
	Semaphore& lock;
	SharedArray<int> lista;

	public:
		static const int EMPTY = -1;

		ListaEntero (IPCName name, int flags, int capacity, Semaphore& lock);
		~ListaEntero ();

		void persist () { lista.persist (); }

		// Agrega el entero i en la cabeza de la lista
		void put (int i);
		// Remueve un entero de la cabeza de la lista.
		int take ();
		// Lee el entero en la cabeza de la lista
		int peek () const;
};

#endif // ESTACION_LISTAENTERO_H
