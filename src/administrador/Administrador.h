#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include <signal.h>
#include <system/EventHandler.h>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>

class Administrador : public EventHandler
{
	Semaphore semCaja;
	SharedVariable<float> areaCaja;

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
