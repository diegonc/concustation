#ifndef ESTACION_TAREA_H
#define ESTACION_TAREA_H

#include <estacion/Auto.h>
#include <sys/types.h>

struct Tarea
{
	// PID del proceso 'empleado' asociado a esta tarea.
	pid_t owner;
	// Auto que debe atender el proceso 'empleado' correspondiente.
	Auto asignacion;
};

#endif // ESTACION_TAREA_H
