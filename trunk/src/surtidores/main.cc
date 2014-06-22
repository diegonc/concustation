#include <ArgParser.h>
#include <estacion/constantes.h>
#include <Surtidores.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("surtidores");
	registry.filename (estacion::LOG_FILE);
	registry.quiet (!args.debug ());

	Surtidores surtidores;
	surtidores.run ();

	return 0;
}
