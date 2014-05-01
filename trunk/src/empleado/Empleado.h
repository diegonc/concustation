#ifndef EMPLEADO_H
#define EMPLEADO_H

#include <signal.h>

#include <system/EventHandler.h>
#include <system/Semaphore.h>

class Empleado : public EventHandler
{
	// Numero de empleado. Se utiliza para identificar el slot en 
	// el area de tareas y para identificar al empleado en la lista
	// de libres.
	int id;
	Semaphore semSurtidoresLibres;
	Semaphore semListaSurtidores;
	volatile sig_atomic_t interrumpido;
	volatile sig_atomic_t tareaAsignada;
	// Señales que se desbloquean durante la suspensión
	sigset_t unblocked;
	// Estado inicial de la mascara de señales
	sigset_t oldset;

	void inicializarSeniales ();
	void finalizarSeniales ();
	void esperarSenial ();

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
