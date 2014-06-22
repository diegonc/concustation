#include <assert.h>
#include <Surtidores.h>
#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <system/File.h>
#include <system/SignalHandler.h>
#include <system/System.h>

Surtidores::Surtidores ()
	: areaConfiguracion (
		  IPCName (estacion::PATH_NAME, estacion::AREA_CONFIGURACION)
		, 0666)
	, msgSurtidores (
		  IPCName (estacion::PATH_NAME, estacion::MSG_SURTIDORES)
		, 0666)
	, interrumpido (0)
{
	areaConfiguracion.persist ();
	msgSurtidores.persist ();

	int M = areaConfiguracion.get ().surtidores;
	for (int i = 1; i <= M; i++) {
		listaSurtidores.push_back (i);
	}
}

Surtidores::~Surtidores ()
{
}

void Surtidores::run ()
{
	SignalHandler& sh = SignalHandler::getInstance ();
	sh.registrarHandler (SIGINT, this);

	Logger& logger = LoggerRegistry::getLogger ("Surtidores");

	logger << "surtidores corriendo con pid " << getpid ()
	       << Logger::endl;

	while (interrumpido == 0) {
		logger << "Se llama a aceptarSolicitudes." << Logger::endl;
		int err = aceptarSolicitudes ();
		logger << "aceptarSolicitudes devolvió "
		       << err << Logger::endl;
	}
	logger << "Finalizando el proceso" << Logger::endl;
}

int Surtidores::aceptarSolicitudes ()
{

	Logger& logger = LoggerRegistry::getLogger ("Surtidores");
	try {
		logger << "Esperando la solicitud algún surtidor..." << Logger::endl;
		OpSurtidores operacion = msgSurtidores.receive (-MSG_SOLICITAR_SURTIDOR);

		logger << "Leyendo una operacion de la cola..." << Logger::endl;
		logger << "se obtuvo: { mtype: " << operacion.mtype
		       << ", rtype: " << operacion.rtype
		       << ", idSurtidor: " << operacion.idSurtidor << " }" << Logger::endl;

		// Recibimos una operacion de la cola, se la procesa dependiendo de su mtype
		switch (operacion.mtype)
		{
			case MSG_DEVOLVER_SURTIDOR:
				listaSurtidores.push_front (operacion.idSurtidor);
				logger << "Se agregó el surtidor " << operacion.idSurtidor
					   << " a la lista de surtidores disponibles."
					   << Logger::endl;
				break;
			case MSG_SOLICITAR_SURTIDOR:
				despacharSurtidor (operacion);
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

void Surtidores::despacharSurtidor (const OpSurtidores& opSurtidores)
{
	Logger& logger = LoggerRegistry::getLogger ("Surtidores");

	logger << "Despachando surtidor: {rtype: " << opSurtidores.rtype << "}"
	       << Logger::endl;

	OpSurtidores operacion;

	if (listaSurtidores.empty ()) {
		logger << "Lista de surtidores vacía, esperando que se devuelva un surtidor..."
		       << Logger::endl;
		operacion = msgSurtidores.receive (MSG_DEVOLVER_SURTIDOR);
		logger << "Se devolvió el surtidor " << operacion.idSurtidor << Logger::endl;

	} else {
		logger << "Tomando un surtidor de la lista." << Logger::endl;
		int idSurtidor = listaSurtidores.front ();
		listaSurtidores.pop_front ();
		logger << "Se tomó el surtidor " << idSurtidor << Logger::endl;
		operacion.idSurtidor = idSurtidor;
	}

	operacion.mtype = opSurtidores.rtype;
	operacion.rtype = 0;
	logger << "Notificando al empleado el surtidor que puede tomar." << Logger::endl;
	logger << "{ mtype: " << operacion.mtype
	       << ", idSurtidor: " << operacion.idSurtidor << "}" << Logger::endl;
	msgSurtidores.send (operacion);
}

void Surtidores::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}
