#include <assert.h>
#include <Jefe.h>
#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <system/File.h>
#include <system/SignalHandler.h>
#include <system/System.h>

Jefe::Jefe ()
	: areaConfiguracion (
		  IPCName (estacion::PATH_NAME, estacion::AREA_CONFIGURACION)
		, 0666)
	, areaNomina (
		IPCName (estacion::PATH_NAME, estacion::AREA_NOMINA)
		, 0666
		, areaConfiguracion.get ().empleados)
	, msgEmpleados (
		  IPCName (estacion::PATH_NAME, estacion::MSG_EMPLEADOS)
		, 0666)
	, msgJefe (
		  IPCName (estacion::PATH_NAME, estacion::MSG_JEFE)
		, 0666)
	, interrumpido (0)
{
	areaConfiguracion.persist ();
	areaNomina.persist ();
	msgEmpleados.persist ();
	msgJefe.persist ();

	int M = areaConfiguracion.get ().empleados;
	for (int i = 1; i <= M; i++) {
		listaEmpleados.push_back (i);
	}
}

Jefe::~Jefe ()
{
}

void Jefe::run ()
{
	SignalHandler& sh = SignalHandler::getInstance ();
	sh.registrarHandler (SIGINT, this);

	Logger& logger = LoggerRegistry::getLogger ("Jefe");

	logger << "jefe corriendo con pid " << getpid ()
	       << Logger::endl;

	while (interrumpido == 0) {
		logger << "Se llama a aceptarAutos." << Logger::endl;
		int err = aceptarAutos ();
		logger << "aceptarAutos devolvió "
		       << err << Logger::endl;
	}
	logger << "Finalizando el proceso" << Logger::endl;
}

int Jefe::aceptarAutos ()
{

	Logger& logger = LoggerRegistry::getLogger ("Jefe");
	try {
		logger << "Esperando la llegada de algún auto..." << Logger::endl;
		OpJefe operacion;

		logger << "Leyendo una operacion de la cola..." << Logger::endl;
		operacion = msgJefe.receive (-MSG_AUTO_REGULAR);
		logger << "se obtuvo: { mtype: " << operacion.mtype
		       << ", idEmp: " << operacion.idEmp
		       << ", litros: " << operacion.litros << "}" << Logger::endl;

		// Recibimos una operacion de la cola, se la procesa dependiendo de su mtype
		switch (operacion.mtype)
		{
			case MSG_EMPLEADO_LIBRE:
				listaEmpleados.push_front (operacion.idEmp);
				break;
			case MSG_AUTO_VIP:
			case MSG_AUTO_REGULAR:
				despacharAuto (operacion);
				break;
		};
		return 0;
	} catch (SystemErrorException& e) {
		logger << "Se atrapo una excepción: "
		       << e.what ()
		       << Logger::endl;
		return e.number ();
	}
}

void Jefe::despacharAuto (const OpJefe& elAuto)
{
	Logger& logger = LoggerRegistry::getLogger ("Jefe");

	logger << "Despachando auto: {litros: " << elAuto.litros << "}"
	       << Logger::endl;

	if (listaEmpleados.empty ()) {
		logger << "Lista de empleados vacía, descartando auto."
		       << Logger::endl;
	} else {
		logger << "Tomando un empleado de la lista." << Logger::endl;
		int idEmp = listaEmpleados.front ();
		listaEmpleados.pop_front ();
		logger << "Se tomo el empleado " << idEmp << Logger::endl;

		// Enviar el auto por la cola 
		logger << "Notificando al empleado de su tarea." << Logger::endl;
		Tarea tarea;
		tarea.mtype = areaNomina[idEmp - 1];
		tarea.litros = elAuto.litros;
		msgEmpleados.send (tarea);
	}
}

void Jefe::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}
