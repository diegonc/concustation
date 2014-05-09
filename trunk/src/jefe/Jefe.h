#ifndef JEFE_H
#define JEFE_H

#include <estacion/Configuracion.h>
#include <estacion/ListaEntero.h>
#include <estacion/Tarea.h>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>

class Jefe : public EventHandler
{
	SharedVariable<Configuracion> areaConfiguracion;
	SharedArray<Tarea> areaTareas;
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
