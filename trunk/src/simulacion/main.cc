
#include <ArgParser.h>
#include <Simulacion.h>
#include <estacion/constantes.h>
#include <iostream>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <string>

int main(int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("simulacion");
	registry.filename (estacion::LOG_FILE);
	registry.quiet (!args.debug ());
	
	Logger& logger = LoggerRegistry::getLogger ("main");

	logger << "Se procesaron los argumentos del programa:" << Logger::endl;
	logger << "Debug: " << args.debug () << Logger::endl;
	logger << "Empleados: " << args.empleados () << Logger::endl;
	logger << "Surtidores: " << args.surtidores () << Logger::endl;

	Simulacion simulacion;
	simulacion.run ();

	std::cout << "Ingrese un texto y presione ENTER para continuar." << std::endl;
	
	std::string tmp;
	std::cin >> tmp;

	return 0;
}
