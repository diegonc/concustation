#ifndef EMPLEADO_H
#define EMPLEADO_H

#include <estacion/OpJefe.h>
#include <estacion/OpSurtidores.h>
#include <estacion/OpCaja.h>
#include <estacion/Tarea.h>
#include <signal.h>
#include <system/EventHandler.h>
#include <system/MessageQueue.h>

class Empleado : public EventHandler
{
	// Numero de empleado. Se utiliza para identificar el slot en 
	// el area de tareas y para identificar al empleado en la lista
	// de libres.
	int id;

	MessageQueue<Tarea> msgEmpleados;
	MessageQueue<OpJefe> msgJefe;
	MessageQueue<OpSurtidores> msgSurtidores;
	MessageQueue<OpCaja> msgCaja;

	volatile sig_atomic_t interrumpido;

	void inicializarSeniales ();
	void finalizarSeniales ();

	// Realiza las tareas para procesar un Auto asignado por el jefe
	// de estacion
	void procesarAuto ();

	public:
		Empleado (int id);

		// Corre el ciclo principal en el que se espera por una
		// tarea y luego se la procesa.
		void run ();

		// Manejador de señales que marcará si se debe interrumpir
		// el ciclo o si el jefe asignó una tarea a este empleado.
		void handleSignal (int signum);
};

#endif // EMPLEADO_H
