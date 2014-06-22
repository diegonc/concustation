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
	, msgSurtidores (
		  IPCName (estacion::PATH_NAME, estacion::MSG_SURTIDORES)
		, 0666)
	, areaCaja (
		  IPCName (estacion::PATH_NAME, estacion::AREA_CAJA)
		, 0666)
	, interrumpido (0)
{
	// No borrar mecanismos de IPC
	semCaja.persist ();
	areaConfiguracion.persist ();
	msgEmpleados.persist ();
	msgJefe.persist ();
	msgSurtidores.persist ();
	areaCaja.persist ();
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

		logger << "Solicitando un surtidor {"
		       << "rtype: " << getpid () << "}" << Logger::endl;
		OpSurtidores opSurtidores;
		opSurtidores.mtype = MSG_SOLICITAR_SURTIDOR;
		opSurtidores.rtype = getpid ();
		opSurtidores.idSurtidor = 0;
		msgSurtidores.send(opSurtidores);

		logger << "Esperando un surtidor disponible." << Logger::endl;
		opSurtidores = msgSurtidores.receive (getpid ());

		logger << "Surtidor disponible {"
		       << "idSurtidor: " << opSurtidores.idSurtidor << "}" << Logger::endl;

		int surtidor = opSurtidores.idSurtidor;
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
		opSurtidores.mtype = MSG_DEVOLVER_SURTIDOR;
		opSurtidores.rtype = 0;
		msgSurtidores.send(opSurtidores);

		// Luego de terminar el procesamiento del auto, el empleado se
		// agrega a la lista de empleados libres.
		logger << "Devolviendo el empleado " << id
		       << Logger::endl;
		OpJefe operacion;
		operacion.mtype = MSG_EMPLEADO_LIBRE;
		operacion.idEmp = id;
		msgJefe.send (operacion);

	} catch (SystemErrorException& e) {
		logger << "Se atrapo una excepción al procesar el auto: "
		       << e.number () << "("
		       << e.what () << ")" << Logger::endl;
		logger << "Saliendo del método..." << Logger::endl;
	}
}
