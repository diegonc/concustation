#include <ArgParser.h>
#include <cassert>
#include <estacion/constantes.h>
#include <Simulacion.h>

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
