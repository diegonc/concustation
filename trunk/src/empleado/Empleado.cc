#include <iostream>
#include <Empleado.h>
#include <estacion/constantes.h>
#include <system/SignalHandler.h>

Empleado::Empleado(int id)
	: id (id)
	, semSurtidoresLibres (
		  IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES_LIBRES)
		, 1, 0666)
	, semListaSurtidores (
		  IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES)
		, 1, 0666)
	, semListaEmpleados (
		  IPCName (estacion::PATH_NAME, estacion::SEM_EMPLEADOS)
		, 1, 0666)
	, semCaja (
		  IPCName (estacion::PATH_NAME, estacion::SEM_CAJA)
		, 1, 0666)
	, interrumpido (0)
	, tareaAsignada (0)
{
	sigemptyset (&unblocked);
	sigemptyset (&oldset);
}

void Empleado::handleSignal (int signum)
{
	if (signum == SIGINT) {
		interrumpido = 1;
	} else if (signum == SIGUSR1) {
		tareaAsignada = 1;
	}
}

void Empleado::inicializarSeniales ()
{
	sigset_t bloqueadasDuranteSenial;
	sigfillset (&bloqueadasDuranteSenial);

	SignalHandler& handler = SignalHandler::getInstance ();
	handler.registrarHandler (SIGINT, this, bloqueadasDuranteSenial);
	handler.registrarHandler (SIGUSR1, this, bloqueadasDuranteSenial);

	// Preparar la mascara de bloqueo para poder utilizar sigsuspend.
	sigset_t blocked;
	oldset = blocked = unblocked = SignalHandler::getProcMask ();
	sigaddset (&blocked, SIGINT);
	sigaddset (&blocked, SIGUSR1);
	sigdelset (&unblocked, SIGINT);
	sigdelset (&unblocked, SIGUSR1);
	SignalHandler::setProcMask (blocked);
}

void Empleado::finalizarSeniales ()
{
	SignalHandler& handler = SignalHandler::getInstance ();
	handler.removerHandler (SIGINT);
	handler.removerHandler (SIGUSR1);
	SignalHandler::setProcMask (oldset);
}

void Empleado::esperarSenial ()
{
	sigsuspend (&unblocked);
}

void Empleado::run ()
{
	inicializarSeniales ();

	while (interrumpido == 0) {
		if (tareaAsignada == 1) {
			// TODO: procesar auto
			std::cout << "Procesando auto..." << std::endl;
			tareaAsignada = 0;
		}

		std::cout << "Esperando señal..." << std::endl;
		esperarSenial ();
	}

	finalizarSeniales ();
}
