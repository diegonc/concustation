#ifndef PRODUCTOR_AUTOS_H
#define PRODUCTOR_AUTOS_H

#include <estacion/OpJefe.h>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>

class ProductorAutos : public EventHandler
{
	MessageQueue<OpJefe> msgJefe;
	volatile sig_atomic_t interrumpido;

	int producirAutos ();
	void producirConEspera ();
	void producirRafaga (int cantidad);
	long calcularVIP ();

	public:
		ProductorAutos ();
		~ProductorAutos ();
		void run ();

		void handleSignal (int signum);
};

#endif // PRODUCTOR_AUTOS_H
