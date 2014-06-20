#ifndef JEFE_H
#define JEFE_H

#include <estacion/Auto.h>
#include <estacion/Configuracion.h>
#include <estacion/ListaEntero.h>
#include <estacion/Tarea.h>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>
#include <system/Semaphore.h>
#include <system/SharedArray.h>
#include <system/SharedVariable.h>

class Jefe : public EventHandler
{
	SharedVariable<Configuracion> areaConfiguracion;
	SharedArray<pid_t> areaNomina;
	MessageQueue<Tarea> msgEmpleados;
	Semaphore semListaEmpleados;
	ListaEntero listaEmpleados;
	volatile sig_atomic_t interrumpido;

	int aceptarAutos ();
	void despacharAuto (const Auto& elAuto);

	public:
		Jefe ();
		~Jefe ();
		void run ();

		void handleSignal (int signum);
};

#endif // JEFE_H
