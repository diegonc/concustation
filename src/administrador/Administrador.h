#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include <estacion/OpCaja.h>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>

class Administrador : public EventHandler
{
	MessageQueue<OpCaja> msgCaja;

	volatile sig_atomic_t interrumpido;

	int consultarCaja ();
	void imprimirCaja ();

	public:
		Administrador ();
		~Administrador ();
		void run ();

		void handleSignal (int signum);
};

#endif // ADMINISTRADOR_H
