#include <ArgParser.h>
#include <Empleado.h>
#include <estacion/constantes.h>
#include <logging/LoggerRegistry.h>

int main (int argc, char **argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);
	
	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.filename (estacion::LOG_FILE);
	registry.application ("empleado");
	registry.quiet (!args.debug ());

	Logger& logger = LoggerRegistry::getLogger ("main");
	logger << "Se procesaron los argumentos del programa:" << Logger::endl;
	logger << "Debug: " << args.debug () << Logger::endl;
	logger << "Id: " << args.id () << Logger::endl;

	Empleado empleado (args.id ());
	empleado.run ();
	return 0;
}
