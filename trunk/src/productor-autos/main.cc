#include <ArgParser.h>
#include <estacion/constantes.h>
#include <ProductorAutos.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("productor-autos");
	registry.filename (estacion::LOG_FILE);
	registry.quiet (!args.debug ());

	ProductorAutos productorAutos;
	productorAutos.run ();

	return 0;
}
