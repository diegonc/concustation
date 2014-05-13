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
#include <system/SemaphoreLocker.h>

Administrador::Administrador ()
	: semCaja (
		  IPCName (estacion::PATH_NAME, estacion::SEM_CAJA)
		, 1, 0666)
	, areaCaja (
		  IPCName (estacion::PATH_NAME, estacion::AREA_CAJA)
		, 0666)
	, interrumpido (0)
{
	semCaja.persist ();
	areaCaja.persist ();
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
		sleep(tiempo);

		logger << "Se llama a consultarCaja." << Logger::endl;
		int err = consultarCaja ();
		logger << "consultarCaja devolvió " << err << Logger::endl;
	}
	logger << "Finalizando el proceso" << Logger::endl;
}

void Administrador::imprimirCaja ()
{
	Logger& logger = LoggerRegistry::getLogger ("Administrador");
	logger << "Bloqueando el semáforo de caja." << Logger::endl;
	
	SemaphoreLocker locker (semCaja);
	float montoCaja = areaCaja.get ();
	std::cout << "Caja: " << montoCaja << std::endl;	
}

int Administrador::consultarCaja ()
{
	Logger& logger = LoggerRegistry::getLogger ("Administrador");
	try {
		logger << "Bloqueando el semáforo de caja." << Logger::endl;
		SemaphoreLocker locker (semCaja);
		float montoCaja = areaCaja.get ();

		int tiempoConsulta = (rand() % (2 * estacion::MEDIA_CONSULTA_ADM)) + 1;
		logger << "Consultando la caja... (" << tiempoConsulta << " segundos)" << Logger::endl;
		sleep(tiempoConsulta);

		logger << "Monto en caja: " << montoCaja << Logger::endl;
		logger << "Desbloqueado el semáforo de caja."
			   << Logger::endl;
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
