#include <iostream>
#include <string>

#include <estacion/Configuracion.h>
#include <estacion/constantes.h>
#include <estacion/ListaEntero.h>
#include <estacion/Tarea.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <system/FifoFilesystemObject.h>
#include <system/Semaphore.h>
#include <system/SharedArray.h>
#include <system/SharedVariable.h>

#include <ArgParser.h>

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

	Semaphore semCaja (
		IPCName (estacion::PATH_NAME, estacion::SEM_CAJA)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);
	Semaphore semListaSurtidores (
		IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);
	Semaphore semListaEmpleados (
		IPCName (estacion::PATH_NAME, estacion::SEM_EMPLEADOS)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);
	Semaphore semSurtidoresLibres (
		IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES_LIBRES)
		, 1, 0666 | IPC_CREAT | IPC_EXCL);

	SharedVariable<Configuracion> areaConfiguracion (
		IPCName (estacion::PATH_NAME, estacion::AREA_CONFIGURACION)
		, 0666 | IPC_CREAT | IPC_EXCL);
	SharedArray<Tarea> areaTareas (
		IPCName (estacion::PATH_NAME, estacion::AREA_TAREAS)
		, 0666 | IPC_CREAT | IPC_EXCL, args.empleados ());
	SharedVariable<float> areaCaja (
		IPCName (estacion::PATH_NAME, estacion::AREA_CAJA)
		, 0666 | IPC_CREAT | IPC_EXCL);
	ListaEntero listaEmpleados (
		IPCName (estacion::PATH_NAME, estacion::AREA_EMPLEADOS)
		, 0666 | IPC_CREAT | IPC_EXCL
		, args.empleados ()
		, semListaEmpleados);
	ListaEntero listaSurtidores (
		IPCName (estacion::PATH_NAME, estacion::AREA_SURTIDORES)
		, 0666 | IPC_CREAT | IPC_EXCL
		, args.surtidores ()
		, semListaSurtidores);
	FifoFilesystemObject fifo (estacion::FIFO_NAME, 0666);

	semCaja.set(0, 1);
	semListaSurtidores.set(0, 1);
	semListaEmpleados.set(0, 1);
	semSurtidoresLibres.set(0, args.surtidores ());

	areaConfiguracion.get ().empleados = args.empleados ();
	areaConfiguracion.get ().surtidores = args.surtidores ();

	areaCaja.set (0);

	listaEmpleados.debug ();
	for (int i=args.empleados (); i > 0; i--) {
		listaEmpleados.put (i);
	}
	listaEmpleados.debug ();

	listaSurtidores.debug ();
	for (int i=args.surtidores (); i > 0; i--) {
		listaSurtidores.put (i);
	}
	listaSurtidores.debug ();

	std::cout << "Ingrese un texto y presione ENTER para continuar." << std::endl;
	
	std::string tmp;
	std::cin >> tmp;

	return 0;
}
