#ifndef ESTACION_TAREA_H
#define ESTACION_TAREA_H

#include <sys/types.h>

/**
 * Estructura que se envia a traves de la cola que comunica
 * a jefe y empleados.
 */
struct Tarea
{
	// Tipo de mensaje que identifica al empleado destino.
	long mtype;
	// Litros que el empleado debera cargar.
	int litros;
};

#endif // ESTACION_TAREA_H
