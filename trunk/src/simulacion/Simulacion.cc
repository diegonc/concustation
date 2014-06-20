#include <ArgParser.h>
#include <cassert>
#include <estacion/constantes.h>
#include <iostream>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <set>
#include <Simulacion.h>
#include <sys/wait.h>
#include <system/SignalHandler.h>
#include <system/System.h>
#include <vector>

Simulacion::Simulacion ()
	: semCaja (
		IPCName (estacion::PATH_NAME, estacion::SEM_CAJA)
		, 1, 0666 | IPC_CREAT | IPC_EXCL)
	, semListaSurtidores (
		IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES)
		, 1, 0666 | IPC_CREAT | IPC_EXCL)
	, semListaEmpleados (
		IPCName (estacion::PATH_NAME, estacion::SEM_EMPLEADOS)
		, 1, 0666 | IPC_CREAT | IPC_EXCL)
	, semSurtidoresLibres (
		IPCName (estacion::PATH_NAME, estacion::SEM_SURTIDORES_LIBRES)
		, 1, 0666 | IPC_CREAT | IPC_EXCL)
	, areaConfiguracion (
		IPCName (estacion::PATH_NAME, estacion::AREA_CONFIGURACION)
		, 0666 | IPC_CREAT | IPC_EXCL)
	, areaNomina (
		IPCName (estacion::PATH_NAME, estacion::AREA_NOMINA)
		, 0666 | IPC_CREAT | IPC_EXCL
		, ArgParser::getInstance ().empleados ())
	, msgEmpleados (
		IPCName (estacion::PATH_NAME, estacion::MSG_EMPLEADOS)
		, 0666 | IPC_CREAT | IPC_EXCL)
	, areaCaja (
		IPCName (estacion::PATH_NAME, estacion::AREA_CAJA)
		, 0666 | IPC_CREAT | IPC_EXCL)
	, listaEmpleados (
		IPCName (estacion::PATH_NAME, estacion::AREA_EMPLEADOS)
		, 0666 | IPC_CREAT | IPC_EXCL
		, ArgParser::getInstance ().empleados ()
		, semListaEmpleados)
	, listaSurtidores (
		IPCName (estacion::PATH_NAME, estacion::AREA_SURTIDORES)
		, 0666 | IPC_CREAT | IPC_EXCL
		, ArgParser::getInstance ().surtidores ()
		, semListaSurtidores)
	, fifo (estacion::FIFO_NAME, 0666)
{
	ArgParser& args = ArgParser::getInstance ();

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
}

Simulacion::~Simulacion ()
{
}

void Simulacion::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}

void Simulacion::run ()
{
	SignalHandler& sh = SignalHandler::getInstance ();
	sh.registrarHandler (SIGINT, this);

	// Conjunto de PIDs correspondiente a los hijos del proceso
	// simulación.
	std::set<pid_t> hijos;
	
	ArgParser& args = ArgParser::getInstance ();
	Logger& logger = LoggerRegistry::getLogger ("Simulacion");

	logger << "simulacion corriendo con pid " << getpid ()
	       << Logger::endl;

	std::cout << "Iniciando simulación..." << std::endl;
	std::cout << "Lanzando empleados " << std::flush;

	for (unsigned i=1; i <= args.empleados (); i++) {
		std::ostringstream oss;
		oss << i;
		std::string empid = oss.str ();

		std::vector<char*> argumentos;
		argumentos.push_back (const_cast<char*> ("./empleado"));

		if (args.debug ()) {
			argumentos.push_back (const_cast<char*> ("-d"));
		}

		argumentos.push_back (&empid[0]);
		argumentos.push_back (NULL);

		logger << "Ejecutando:";
		for (size_t arg=0; arg < argumentos.size () - 1; arg++) {
			logger << " " << argumentos[arg];
		}
		logger << Logger::endl;

		pid_t pid = System::spawn ("./empleado", &argumentos[0]);
		if (pid == -1) {
			SystemErrorException e;
			logger << "Fallo al crear empleado: "
			       << e.what () << Logger::endl;
			detenerHijos (hijos);
			return;
		}
		logger << "Proceso hijo: " << pid << Logger::endl;
		hijos.insert (pid);
		areaNomina[i - 1] = pid;

		std::cout << "." << std::flush;
	}
	std::cout << " [completado]" << std::endl;

	std::cout << "Lanzando jefe " << std::flush;
	// Luego de inicializar por completo el area de tareas
	// se lanza el proceso jefe.
	{
		std::vector<char*> argumentos;
		argumentos.push_back (const_cast<char*> ("./jefe"));
		if (args.debug ()) {
			argumentos.push_back (const_cast<char*> ("-d"));
		}
		argumentos.push_back (NULL);

		logger << "Ejecutando:";
		for (size_t arg=0; arg < argumentos.size () - 1; arg++) {
			logger << " " << argumentos[arg];
		}
		logger << Logger::endl;

		pid_t pid = System::spawn ("./jefe", &argumentos[0]);
		if (pid == -1) {
			SystemErrorException e;
			logger << "Fallo al crear jefe: "
			       << e.what () << Logger::endl;
			detenerHijos (hijos);
			return;
		}
		logger << "Proceso hijo: " << pid << Logger::endl;
		hijos.insert (pid);
	}
	std::cout << " [completado]" << std::endl;

	std::cout << "Lanzando administrador " << std::flush;
	// luego se lanza el proceso administrador
	{
		std::vector<char*> argumentos;
		argumentos.push_back (const_cast<char*> ("./administrador"));
		if (args.debug ()) {
			argumentos.push_back (const_cast<char*> ("-d"));
		}
		argumentos.push_back (NULL);

		logger << "Ejecutando:";
		for (size_t arg=0; arg < argumentos.size () - 1; arg++) {
			logger << " " << argumentos[arg];
		}
		logger << Logger::endl;

		pid_t pid = System::spawn ("./administrador", &argumentos[0]);
		if (pid == -1) {
			SystemErrorException e;
			logger << "Fallo al crear administrador: "
			       << e.what () << Logger::endl;
			detenerHijos (hijos);
			return;
		}
		logger << "Proceso hijo: " << pid << Logger::endl;
		hijos.insert (pid);
	}
	std::cout << " [completado]" << std::endl;

	std::cout << "Lanzando el productor de autos " << std::flush;
	// el ultimo proceso que se lanza es el productor de autos
	{
		std::vector<char*> argumentos;
		argumentos.push_back (const_cast<char*> ("./productor-autos"));
		if (args.debug ()) {
			argumentos.push_back (const_cast<char*> ("-d"));
		}
		argumentos.push_back (NULL);

		logger << "Ejecutando:";
		for (size_t arg=0; arg < argumentos.size () - 1; arg++) {
			logger << " " << argumentos[arg];
		}
		logger << Logger::endl;

		pid_t pid = System::spawn ("./productor-autos", &argumentos[0]);
		if (pid == -1) {
			SystemErrorException e;
			logger << "Fallo al crear productor-autos: "
			       << e.what () << Logger::endl;
			detenerHijos (hijos);
			return;
		}
		logger << "Proceso hijo: " << pid << Logger::endl;
		hijos.insert (pid);
	}
	std::cout << " [completado]" << std::endl;
	std::cout << "...simulación corriendo" << std::endl;

	pid_t hijoTerminado = wait (NULL);
	logger << "Esperando que se detengan los hijos..." << Logger::endl;

	std::cout << std::endl;
	std::cout << "Esperando que se detengan los hijos..." << std::flush;
	if (hijoTerminado != -1) {
		logger << "pid " << hijoTerminado
		       << " ya terminó"
		       << Logger::endl;
		hijos.erase (hijoTerminado);
		std::cout << " " << hijoTerminado << std::flush;
	}

	esperarHijos (hijos);

	logger << "Todos los hijos terminaron. "
	       << "Finalizando simulación." << Logger::endl;

	std::cout << "Simulación finalizada" << std::endl;
}

void Simulacion::esperarHijos (std::set<pid_t>& hijos)
{
	while (!hijos.empty ()) {
		pid_t hijo = wait (NULL);
		if (hijo == -1) {
			Logger& logger = LoggerRegistry::getLogger ("Simulacion");
			SystemErrorException e;
			logger << "Error al esperar hijo: "
			       << e.what () << Logger::endl;
		} else {
			hijos.erase (hijo);
			std::cout << " " << hijo << std::flush;
		}
	}
	std::cout << std::endl;
}

void Simulacion::detenerHijos (std::set<pid_t>& hijos)
{
	Logger& logger = LoggerRegistry::getLogger ("Simulacion");
	logger << "Deteniendo hijos..." << Logger::endl;

	for (std::set<pid_t>::iterator it = hijos.begin ();
		it != hijos.end (); it++) {
		pid_t hijo = *it;
		logger << "Enviando señal SIGINT a hijo " << hijo << Logger::endl;
		kill (hijo, SIGINT);
	}

	esperarHijos (hijos);
}
