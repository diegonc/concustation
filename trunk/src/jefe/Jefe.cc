#include <assert.h>
#include <Jefe.h>
#include <estacion/constantes.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <system/File.h>
#include <system/SignalHandler.h>
#include <system/System.h>

Jefe::Jefe ()
	: areaConfiguracion (
		  IPCName (estacion::PATH_NAME, estacion::AREA_CONFIGURACION)
		, 0666)
	, areaTareas (
		  IPCName (estacion::PATH_NAME, estacion::AREA_TAREAS)
		, 0666
		, areaConfiguracion.get ().empleados)
	, semListaEmpleados (
		  IPCName (estacion::PATH_NAME, estacion::SEM_EMPLEADOS)
		, 1
		, 0666)
	, listaEmpleados (
		  IPCName (estacion::PATH_NAME, estacion::AREA_EMPLEADOS)
		, 0666
		, areaConfiguracion.get ().empleados
		, semListaEmpleados)
	, interrumpido (0)
{
	areaConfiguracion.persist ();
	areaTareas.persist ();
	semListaEmpleados.persist ();
	listaEmpleados.persist ();
}

Jefe::~Jefe ()
{
}

void Jefe::run ()
{
	SignalHandler& sh = SignalHandler::getInstance ();
	sh.registrarHandler (SIGINT, this);

	Logger& logger = LoggerRegistry::getLogger ("Jefe");
	while (interrumpido == 0) {
		logger << "Se llama a aceptarAutos." << Logger::endl;
		int err = aceptarAutos ();
		logger << "aceptarAutos devolvió "
		       << err << Logger::endl;
	}
	logger << "Finalizando el proceso" << Logger::endl;
}

int Jefe::aceptarAutos ()
{

	Logger& logger = LoggerRegistry::getLogger ("Jefe");
	try {
		File fifo (estacion::FIFO_NAME, O_RDONLY);
		Auto elAuto;
		ssize_t err;

		do {
			logger << "Leyendo un auto del FIFO..." << Logger::endl;
			err = fifo.read (sizeof (elAuto),
			             reinterpret_cast<char*> (&elAuto));
			logger << "read devolvió " << err << Logger::endl;

			if (err == -1) {
				// Verificar si se recibió una señal
				// que haya interrumpido el read.
				if (errno == EINTR) {
					logger << "read fue interrumpido."
					       << Logger::endl;
					return errno;
				}
				throw SystemErrorException();
			} else if (err != 0) {
				// Si se interrumpió un read por la mitad se
				// descarta lo leido volviendo al ciclo
				// principal.
				if (interrumpido == 1) {
					return 0;
				}
				// TODO: hay que acumular datos, read puede
				// devolver menos de lo pedido.
				assert (err == sizeof (elAuto));
				// Recibimos un Auto desde el fifo
				// Se lo despacha
				logger << "Se recibio el auto: {"
				       << "monto: " << elAuto.monto
				       << ", tiempoEspera: " << elAuto.tiempoEspera
				       << "}" << Logger::endl;
				despacharAuto (elAuto);
			}
		} while (err != 0);
		// El productor cerro el otro extremo del fifo
		// Se nos vuelve a llamar desde el ciclo principal
		// en caso que no se haya interrumpido.
		return 0;
	} catch (SystemErrorException& e) {
		logger << "Se atrapo una excepción: "
		       << e.what ()
		       << Logger::endl;
		return e.number ();
	}
}

void Jefe::despacharAuto (const Auto& elAuto)
{
	Logger& logger = LoggerRegistry::getLogger ("Jefe");

	logger << "Despachando auto: { monto: " << elAuto.monto
	       << ", tiempoEspera: " << elAuto.tiempoEspera << "}"
	       << Logger::endl;

	logger << "Tomando un empleado de la lista." << Logger::endl;
	int idEmp = listaEmpleados.take ();

	if (idEmp == ListaEntero::EMPTY) {
		logger << "Lista de empleados vacía, descartando auto."
		       << Logger::endl;
		return;
	} else {
		logger << "Se tomo el empleado " << idEmp << Logger::endl;
	}
	assert (idEmp > 0);

	// Grabar el auto en el area de tareas
	logger << "Grabando el auto en el area de tareas." << Logger::endl;
	Tarea& tarea = areaTareas[idEmp - 1];
	tarea.asignacion = elAuto;

	// Finalmente se despierta al empleado
	logger << "Despertando al empleado " << idEmp
	       << " (pid: " << tarea.owner << ")"
	       << Logger::endl;
	int err = kill (tarea.owner, SIGUSR1);
	if (err == -1) {
		SystemErrorException e;
		logger << "kill devolvio: " << err
		       << " (" << e.what () << ")" << Logger::endl;
	}
}

void Jefe::handleSignal (int signum)
{
	assert (signum == SIGINT);
	interrumpido = 1;
}
