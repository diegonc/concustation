#ifndef CAJA_H
#define CAJA_H

#include <estacion/OpCaja.h>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>

class Caja : public EventHandler
{
	MessageQueue<OpCaja> msgCaja;
	float monto;
	volatile sig_atomic_t interrumpido;

	int aceptarConsultas ();
	void acreditarMonto (const OpCaja& opCaja);
	void consultarMonto (const OpCaja& opCaja);

	public:
		Caja ();
		~Caja ();
		void run ();

		void handleSignal (int signum);
};

#endif // CAJA_H
