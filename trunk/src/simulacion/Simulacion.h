#ifndef SIMULACION_H
#define SIMULACION_H

#include <estacion/Configuracion.h>
#include <estacion/ListaEntero.h>
#include <estacion/OpJefe.h>
#include <estacion/Tarea.h>
#include <set>
#include <signal.h>
#include <sys/types.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>
#include <system/Semaphore.h>
#include <system/SharedArray.h>
#include <system/SharedVariable.h>

class Simulacion : public EventHandler
{
	// Numero de empleado. Se utiliza para identificar el slot en 
	// el area de tareas y para identificar al empleado en la lista
	// de libres.
	int id;

	Semaphore semCaja;
	Semaphore semListaSurtidores;
	Semaphore semSurtidoresLibres;

	SharedVariable<Configuracion> areaConfiguracion;
	SharedArray<pid_t> areaNomina;
	MessageQueue<Tarea> msgEmpleados;
	MessageQueue<OpJefe> msgJefe;
	SharedVariable<float> areaCaja;
	ListaEntero listaSurtidores;

	volatile sig_atomic_t interrumpido;

	void detenerHijos (std::set<pid_t>& hijos);
	void esperarHijos (std::set<pid_t>& hijos);

	public:
		Simulacion ();
		~Simulacion ();

		// Corre el ciclo principal en el que se espera por una
		// tarea y luego se la procesa.
		void run ();

		// Manejador de señales que marcará si se debe interrumpir
		// el ciclo.
		void handleSignal (int signum);
};

#endif // EMPLEADO_H
