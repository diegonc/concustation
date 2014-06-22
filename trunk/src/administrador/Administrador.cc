#include <ArgParser.h>
#include <assert.h>
#include <Administrador.h>
#include <estacion/constantes.h>
#include <iostream>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <stdlib.h>
#include <system/SignalHandler.h>
#include <system/System.h>

Administrador::Administrador ()
	: msgCaja (
		  IPCName (estacion::PATH_NAME, estacion::MSG_CAJA)
		, 0666)
	, interrumpido (0)
{
	msgCaja.persist ();
}

Administrador::~Administrador ()
{
}

void Administrador::run ()
{
	SignalHandler& sh = SignalHandler::getInstance ();
	sh.registrarHandler (SIGINT, this);

	Logger& logger = LoggerRegistry::getLogger ("Administrador");

	logger << "administrador corriendo con pid " << getpid ()
	       << Logger::endl;

	ArgParser& args = ArgParser::getInstance ();
	if (args.once ()) {
		imprimirCaja ();
		return;
	}

	srand ( time(NULL) );

	while (interrumpido == 0) {
		// simulación del tiempo para la próxima consulta del administrador
		int tiempo = (rand() % (2 * estacion::MEDIA_ENTRE_CONSULTAS_ADM)) + 1;
		logger << "Faltan " << tiempo << " segundos para la próxima consulta del administrador." << Logger::endl;
		int restante = sleep(tiempo);
		if (restante > 0) {
			// Sleep interrumpido - verificar flag
			continue;
		}

		logger << "Se llama a consultarCaja." << Logger::endl;
		int err = consultarCaja ();
		logger << "consultarCaja devolvió " << err << Logger::endl;
	}
	logger << "Finalizando el proceso" << Logger::endl;
}

void Administrador::imprimirCaja ()
{
	Logger& logger = LoggerRegistry::getLogger ("Administrador");
	
	OpCaja opCaja;
	opCaja.mtype = MSG_CONSULTAR_CAJA;
	opCaja.rtype = getpid ();
	opCaja.monto = 0;
	msgCaja.send (opCaja);

	opCaja = msgCaja.receive (getpid ());
	logger << "Monto en caja: " << opCaja.monto << Logger::endl;
}

int Administrador::consultarCaja ()
{
	Logger& logger = LoggerRegistry::getLogger ("Administrador");
	try {
		logger << "Consultando la caja..." << Logger::endl;
		OpCaja opCaja;
		opCaja.mtype = MSG_CONSULTAR_CAJA;
		opCaja.rtype = getpid ();
		opCaja.monto = 0;
		msgCaja.send (opCaja);

		opCaja = msgCaja.receive (getpid ());

		logger << "Monto en caja: " << opCaja.monto << Logger::endl;
		return 0;

	} catch (SystemErrorException& e) {
		logger << "Se atrapo una excepción al consultar la caja: "
		       << e.number () << "("
		       << e.what () << ")" << Logger::endl;
		logger << "Saliendo del método..." << Logger::endl;
		return e.number ();
	}
}

void Administrador::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}
