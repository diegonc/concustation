#include <ArgParser.h>
#include <estacion/constantes.h>
#include <Caja.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("caja");
	registry.filename (estacion::LOG_FILE);
	registry.quiet (!args.debug ());

	Caja caja;
	caja.run ();

	return 0;
}
