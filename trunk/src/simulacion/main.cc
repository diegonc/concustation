#include <iostream>
#include <string>

#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <system/Semaphore.h>

#include <ArgParser.h>

int main(int argc, char** argv)
{

	ArgParser& args = ArgParser::getInstance();
	args.parse(argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("simulacion");
	registry.filename (estacion::LOG_FILE);
	registry.quiet (!args.debug ());
	
	Logger& logger = LoggerRegistry::getLogger ("main");

	logger << "Se procesaron los argumentos del programa:" << Logger::endl;
	logger << "Debug: " << args.debug () << Logger::endl;
	logger << "Empleados: " << args.empleados () << Logger::endl;
	logger << "Surtidores: " << args.surtidores () << Logger::endl;

	Semaphore semCaja(
		IPCName(estacion::PATH_NAME, estacion::SEM_CAJA)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);
	Semaphore semListaSurtidores(
		IPCName(estacion::PATH_NAME, estacion::SEM_SURTIDORES)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);
	Semaphore semListaEmpleados(
		IPCName(estacion::PATH_NAME, estacion::SEM_EMPLEADOS)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);
	Semaphore semSurtidoresLibres(
		IPCName(estacion::PATH_NAME, estacion::SEM_SURTIDORES_LIBRES)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);

	semCaja.set(0, 1);
	semListaSurtidores.set(0, 1);
	semListaEmpleados.set(0, 1);
	semSurtidoresLibres.set(0, args.surtidores ());

	std::cout << "Ingrese un texto y presione ENTER para continuar." << std::endl;
	
	std::string tmp;
	std::cin >> tmp;

	return 0;
}
