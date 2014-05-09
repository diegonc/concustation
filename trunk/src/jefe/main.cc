#include <ArgParser.h>
#include <estacion/constantes.h>
#include <Jefe.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("jefe");
	registry.filename (estacion::LOG_FILE);
	registry.quiet (!args.debug ());

	Jefe jefe;
	jefe.run ();

	return 0;
}
