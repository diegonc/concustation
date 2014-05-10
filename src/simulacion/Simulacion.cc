#include <ArgParser.h>
#include <cassert>
#include <estacion/constantes.h>
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
	, areaTareas (
		IPCName (estacion::PATH_NAME, estacion::AREA_TAREAS)
		, 0666 | IPC_CREAT | IPC_EXCL
		, ArgParser::getInstance ().empleados ())
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
		areaTareas[i - 1].owner = pid;
	}

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

	pid_t hijoTerminado = wait (NULL);
	logger << "Esperando que se detengan los hijos..." << Logger::endl;
	if (hijoTerminado != -1) {
		logger << "pid " << hijoTerminado
		       << " ya terminó"
		       << Logger::endl;
		hijos.erase (hijoTerminado);
	}

	esperarHijos (hijos);

	logger << "Todos los hijos terminaron. "
	       << "Finalizando simulación." << Logger::endl;
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
		}
	}
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
