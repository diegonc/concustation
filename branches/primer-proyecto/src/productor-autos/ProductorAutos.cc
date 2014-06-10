#include <ArgParser.h>
#include <assert.h>
#include <ProductorAutos.h>
#include <estacion/constantes.h>
#include <estacion/Auto.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <stdlib.h>
#include <system/File.h>
#include <system/SignalHandler.h>
#include <system/System.h>

ProductorAutos::ProductorAutos ()
	: interrumpido (0)
{
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
		File fifo (estacion::FIFO_NAME, O_WRONLY);
		Auto elAuto;
		elAuto.litros = (rand() % (2 * estacion::MEDIA_LITROS_AUTO)) + 1;
		ssize_t err;

		logger << "Agregando un auto al FIFO..." << Logger::endl;
		err = fifo.write (sizeof (elAuto), reinterpret_cast<char*> (&elAuto));
		logger << "write devolvió " << err << Logger::endl;

		if (err == -1) {
			// Verificar si se recibió una señal
			// que haya interrumpido el write.
			if (errno == EINTR) {
				logger << "write fue interrumpido."
					   << Logger::endl;
				return errno;
			}
			throw SystemErrorException();

		} else if (err != 0) {
			if (interrumpido == 1) {
				return EINTR;
			}
			logger << "Se agregó el auto: {"
				   << "litros: " << elAuto.litros
				   << "}" << Logger::endl;
		}

		return 0;

	} catch (SystemErrorException& e) {
		logger << "Se atrapo una excepción: "
		       << e.what ()
		       << Logger::endl;
		return e.number ();
	}
}

void ProductorAutos::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}
