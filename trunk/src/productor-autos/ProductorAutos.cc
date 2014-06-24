#include <ArgParser.h>
#include <assert.h>
#include <ProductorAutos.h>
#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <stdlib.h>
#include <system/File.h>
#include <system/SignalHandler.h>
#include <system/System.h>

ProductorAutos::ProductorAutos ()
	: msgJefe (
		IPCName (estacion::PATH_NAME, estacion::MSG_JEFE)
		, 0666)
	, interrumpido (0)
{
	msgJefe.persist ();
}

ProductorAutos::~ProductorAutos ()
{
}

void ProductorAutos::run ()
{
	ArgParser& args = ArgParser::getInstance ();
	SignalHandler& sh = SignalHandler::getInstance ();
	sh.registrarHandler (SIGINT, this);

	Logger& logger = LoggerRegistry::getLogger ("ProductorAutos");

	logger << "productor-autos corriendo con pid " << getpid ()
	       << Logger::endl;

	srand ( time(NULL) );

	if (args.burstSize () == 0) {
		producirConEspera ();
	} else {
		producirRafaga (args.burstSize ());
	}

	logger << "Finalizando el proceso" << Logger::endl;
}

void ProductorAutos::producirRafaga (int cantidad)
{
	assert (cantidad > 0);
	Logger& logger = LoggerRegistry::getLogger ("ProductorAutos");

	for (int i=1; i <= cantidad; i++) {
		logger << "Produciendo auto " << i << Logger::endl;
		int err = producirAutos ();
		logger << "producirAutos devolvió " << err
		       << Logger::endl;
	}
}

void ProductorAutos::producirConEspera ()
{
	Logger& logger = LoggerRegistry::getLogger ("ProductorAutos");

	while (interrumpido == 0) {
		// simulación del tiempo de llegada de 1 auto
		int tiempo = (rand() % (2 * estacion::MEDIA_LLEGADA_AUTOS)) + 1;
		logger << "Faltan " << tiempo << " segundos para la llegada de un auto." << Logger::endl;
		int faltante = sleep(tiempo);
		if (faltante > 0) {
			// se interrumpio la llamada a sleep.
			// vuelvo a verificar la bandera de interrupción
			if (interrumpido == 1) {
				break;
			}
		}

		logger << "Se llama a producirAutos." << Logger::endl;
		int err = producirAutos ();
		logger << "producirAutos devolvió " << err << Logger::endl;
		if (err != 0) {
			break;		
		}
	}
}

int ProductorAutos::producirAutos ()
{
	Logger& logger = LoggerRegistry::getLogger ("ProductorAutos");
	try {
		logger << "Produciendo un auto..." << Logger::endl;
		OpJefe elAuto;
		elAuto.litros = (rand() % (2 * estacion::MEDIA_LITROS_AUTO)) + 1;
		elAuto.idEmp = 0;
		elAuto.mtype = calcularVIP ();

		logger << "Agregando un auto a la cola del jefe..." << Logger::endl;
		logger << "Se agregará el auto: {"
		       << "mtype: " << elAuto.mtype
		       << ", litros: " << elAuto.litros
		       << "}" << Logger::endl;

		msgJefe.send (elAuto);
		return 0;
	} catch (SystemErrorException& e) {
		logger << "Se atrapo una excepción: "
		       << e.what ()
		       << Logger::endl;
		return e.number ();
	}
}

long ProductorAutos::calcularVIP ()
{
	int i = rand () % 100;
	if (i <= estacion::PROBABILIDAD_VIP) {
		return MSG_AUTO_VIP;
	} else {
		return MSG_AUTO_REGULAR;
	}
}

void ProductorAutos::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}
