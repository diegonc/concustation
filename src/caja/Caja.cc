#include <assert.h>
#include <Caja.h>
#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <system/File.h>
#include <system/SignalHandler.h>
#include <system/System.h>

Caja::Caja ()
	: msgCaja (
		  IPCName (estacion::PATH_NAME, estacion::MSG_CAJA)
		, 0666)
	, interrumpido (0)
{
	msgCaja.persist ();
	monto = 0;
}

Caja::~Caja ()
{
}

void Caja::run ()
{
	SignalHandler& sh = SignalHandler::getInstance ();
	sh.registrarHandler (SIGINT, this);

	Logger& logger = LoggerRegistry::getLogger ("Caja");

	logger << "caja corriendo con pid " << getpid ()
	       << Logger::endl;

	while (interrumpido == 0) {
		logger << "Se llama a aceptarConsultas." << Logger::endl;
		int err = aceptarConsultas ();
		logger << "aceptarConsultas devolvió "
		       << err << Logger::endl;
	}
	logger << "Finalizando el proceso" << Logger::endl;
}

int Caja::aceptarConsultas ()
{

	Logger& logger = LoggerRegistry::getLogger ("Caja");
	try {
		logger << "Esperando la llegada de alguna consulta..." << Logger::endl;

		OpCaja operacion = msgCaja.receive (-MSG_ACREDITAR_MONTO);
		logger << "Leyendo una operacion de la cola..." << Logger::endl;
		logger << "se obtuvo: { mtype: " << operacion.mtype
		       << ", rtype: " << operacion.rtype
		       << ", monto: " << operacion.monto << "}" << Logger::endl;

		// Recibimos una operacion de la cola, se la procesa dependiendo de su mtype
		switch (operacion.mtype)
		{
			case MSG_CONSULTAR_CAJA:
				consultarMonto (operacion);
				break;
			case MSG_ACREDITAR_MONTO:
				acreditarMonto (operacion);
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

void Caja::consultarMonto (const OpCaja& opCaja)
{
	Logger& logger = LoggerRegistry::getLogger ("Caja");

	logger << "Procesando consulta de monto: {rtype: " << opCaja.rtype << "}"
	       << Logger::endl;

	logger << "Respondiendo consulta." << Logger::endl;
	OpCaja operacion;
	operacion.mtype = opCaja.rtype;
	operacion.rtype = 0;
	operacion.monto = monto;
	msgCaja.send (operacion);
}

void Caja::acreditarMonto (const OpCaja& opCaja)
{
	Logger& logger = LoggerRegistry::getLogger ("Caja");

	logger << "Acreditando monto en caja." << Logger::endl;
	logger << "Monto actual: " << monto << Logger::endl;
	logger << "Monto a acreditar: " << opCaja.monto << Logger::endl;

	monto += opCaja.monto;

	logger << "Monto acreditado." << Logger::endl;
	logger << "Nuevo monto: " << monto << Logger::endl;
}

void Caja::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}
