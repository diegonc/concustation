#ifndef PRODUCTOR_AUTOS_H
#define PRODUCTOR_AUTOS_H

#include <signal.h>
#include <system/EventHandler.h>

class ProductorAutos : public EventHandler
{
	volatile sig_atomic_t interrumpido;

	int producirAutos ();

	public:
		ProductorAutos ();
		~ProductorAutos ();
		void run ();

		void handleSignal (int signum);
};

#endif // PRODUCTOR_AUTOS_H
