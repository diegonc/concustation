#ifndef JEFE_H
#define JEFE_H

#include <estacion/Configuracion.h>
#include <estacion/OpJefe.h>
#include <estacion/Tarea.h>
#include <list>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>
#include <system/SharedArray.h>
#include <system/SharedVariable.h>

class Jefe : public EventHandler
{
	SharedVariable<Configuracion> areaConfiguracion;
	SharedArray<pid_t> areaNomina;
	MessageQueue<Tarea> msgEmpleados;
	MessageQueue<OpJefe> msgJefe;
	std::list<int> listaEmpleados;
	volatile sig_atomic_t interrumpido;

	int aceptarAutos ();
	void despacharAuto (const OpJefe& elAuto);

	public:
		Jefe ();
		~Jefe ();
		void run ();

		void handleSignal (int signum);
};

#endif // JEFE_H
