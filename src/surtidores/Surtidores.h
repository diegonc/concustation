#ifndef SURTIDORES_H
#define SURTIDORES_H

#include <estacion/Configuracion.h>
#include <estacion/OpSurtidores.h>
#include <list>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>
#include <system/SharedVariable.h>

class Surtidores : public EventHandler
{
	SharedVariable<Configuracion> areaConfiguracion;
	MessageQueue<OpSurtidores> msgSurtidores;
	std::list<int> listaSurtidores;
	volatile sig_atomic_t interrumpido;

	int aceptarSolicitudes ();
	void despacharSurtidor (const OpSurtidores& opSurtidores);

	public:
		Surtidores ();
		~Surtidores ();
		void run ();

		void handleSignal (int signum);
};

#endif // SURTIDORES_H
