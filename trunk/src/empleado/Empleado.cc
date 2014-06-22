#include <iostream>
#include <stdlib.h>
#include <Empleado.h>
#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <system/SignalHandler.h>
#include <system/SemaphoreLocker.h>

Empleado::Empleado(int id)
	: id (id)
	, semSurtidoresLibres (
		  IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES_LIBRES)
		, 1, 0666)
	, semListaSurtidores (
		  IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES)
		, 1, 0666)
	, semCaja (
		  IPCName (estacion::PATH_NAME, estacion::SEM_CAJA)
		, 1, 0666)
	, areaConfiguracion (
		  IPCName (estacion::PATH_NAME, estacion::AREA_CONFIGURACION)
		, 0666)
	, msgEmpleados (
		  IPCName (estacion::PATH_NAME, estacion::MSG_EMPLEADOS)
		, 0666)
	, msgJefe (
		  IPCName (estacion::PATH_NAME, estacion::MSG_JEFE)
		, 0666)
	, areaCaja (
		  IPCName (estacion::PATH_NAME, estacion::AREA_CAJA)
		, 0666)
	, listaSurtidores (
		  IPCName (estacion::PATH_NAME, estacion::AREA_SURTIDORES)
		, 0666
		, areaConfiguracion.get ().surtidores
		, semListaSurtidores)
	, interrumpido (0)
{
	// No borrar mecanismos de IPC
	semSurtidoresLibres.persist ();
	semListaSurtidores.persist ();
	semCaja.persist ();
	areaConfiguracion.persist ();
	msgEmpleados.persist ();
	msgJefe.persist ();
	areaCaja.persist ();
	listaSurtidores.persist ();
}

void Empleado::handleSignal (int signum)
{
	if (signum == SIGINT) {
		interrumpido = 1;
	}
}

void Empleado::inicializarSeniales ()
{
	sigset_t bloqueadasDuranteSenial;
	sigfillset (&bloqueadasDuranteSenial);

	SignalHandler& handler = SignalHandler::getInstance ();
	handler.registrarHandler (SIGINT, this, bloqueadasDuranteSenial);
}

void Empleado::finalizarSeniales ()
{
	SignalHandler& handler = SignalHandler::getInstance ();
	handler.removerHandler (SIGINT);
	handler.removerHandler (SIGUSR1);
}

void Empleado::run ()
{
	Logger& logger = LoggerRegistry::getLogger ("Empleado");

	logger << "empleado corriendo con pid " << getpid ()
	       << Logger::endl;

	inicializarSeniales ();

	while (interrumpido == 0) {
		procesarAuto ();
	}

	logger << "Finalizando por señal de interrupción." << Logger::endl;

	finalizarSeniales ();
}

void Empleado::procesarAuto ()
{
	Logger& logger = LoggerRegistry::getLogger ("Empleado");
	try {
		logger << "esperando asignación..." << Logger::endl;

		Tarea tarea = msgEmpleados.receive (getpid ());
		logger << "Comenzando el procesamiento del auto {"
		       << "litros: " << tarea.litros << "}" << Logger::endl;

		// Se espera a que haya un surtidor libre, luego se toma
		// el primero de la lista
		logger << "Bloqueando el semáforo de surtidores libres."
		       << Logger::endl;
		SemaphoreLocker slLocker (semSurtidoresLibres);

		logger << "Hay surtidores libres, tomando uno de la lista..."
		       << Logger::endl;

		int surtidor = listaSurtidores.take ();
		logger << "El empleado " << id << " esta utilizando el surtidor "
		       << surtidor << Logger::endl;

		// Simulación del retardo por carga de combustible
		int tiempoEspera = tarea.litros * estacion::TIEMPO_LITRO;
		logger << "Esperando recarga (" << tiempoEspera
		       << " segundos)" << Logger::endl;

		int restante = sleep (tiempoEspera);
		if (restante > 0) {
			// Interrumpido por señal
			throw SystemErrorException (EINTR);
		}

		logger << "Recarga finalizada. "
		       << "Se acreditará el monto en la caja."
		       << Logger::endl;

		// Se actualiza el monto de la caja en forma atómica
		// tomando el semáforo de la caja.
		{
			logger << "Bloqueando el semáforo de caja."
			       << Logger::endl;
			SemaphoreLocker locker (semCaja);
			float montoCaja = areaCaja.get ();
			float nuevoMonto = montoCaja + (tarea.litros * estacion::PRECIO_LITRO);
			areaCaja.set (nuevoMonto);
			logger << "Monto inicial: " << montoCaja << Logger::endl;
			logger << "Nuevo monto: " << nuevoMonto << Logger::endl;
			logger << "Desbloqueado el semáforo de caja."
			       << Logger::endl;
		}

		logger << "Devolviendo el surtidor " << surtidor
		       << Logger::endl;
		listaSurtidores.put (surtidor);

		// Luego de terminar el procesamiento del auto, el empleado se
		// agrega a la lista de empleados libres.
		logger << "Devolviendo el empleado " << id
		       << Logger::endl;
		OpJefe operacion;
		operacion.mtype = MSG_EMPLEADO_LIBRE;
		operacion.idEmp = id;
		msgJefe.send (operacion);

		logger << "Desbloqueando el semáforo de surtidores libres..."
		       << Logger::endl;
	} catch (SystemErrorException& e) {
		logger << "Se atrapo una excepción al procesar el auto: "
		       << e.number () << "("
		       << e.what () << ")" << Logger::endl;
		logger << "Saliendo del método..." << Logger::endl;
	}
}
