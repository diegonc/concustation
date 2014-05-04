#ifndef ESTACION_CONSTANTES_H
#define ESTACION_CONSTANTES_H

namespace estacion {
	/*
	 * nombre del FIFO desde donde el jefe de estación toma los autos.
	 */
	const char* const FIFO_NAME = "simulacion.fifo";

	/*
	 * nombre del archivo de log donde se guardan los mensajes
	 * informativos generados por el programa.
	 */
	const char* const LOG_FILE = "simulacion.log";

	/*
	 * pathname pasado a ftok para genera la key que se utiliza al
	 * adquirir recursos de IPC.
	 */
	const char * const PATH_NAME = "./simulacion";

	/*
	 * proj_id pasado a ftok para generar la key correspondiente a la
	 * memoria compartida utilizada para almacenar el area de configuración.
	 */
	const char AREA_CONFIGURACION = 'A';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente a la
	 * memoria compartida utilizada para almacenar la asignacion de tareas.
	 */
	const char AREA_TAREAS = 'T';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente a la
	 * memoria compartida utilizada para almacenar la caja.
	 */
	const char AREA_CAJA = 'C';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente a la
	 * memoria compartida utilizada para almacenar la lista de empleados
	 * libres.
	 */
	const char AREA_EMPLEADOS = 'E';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente a la
	 * memoria compartida utilizada para almacenar la lista de surtidores
	 * libres.
	 */
	const char AREA_SURTIDORES = 'S';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente al
	 * semaforo que protege la lista de empleados libres.
	 */
	const char SEM_EMPLEADOS = 'e';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente al
	 * semaforo que protege la lista de surtidores libres.
	 */
	const char SEM_SURTIDORES = 's';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente al
	 * semaforo general que indica cuantos surtidores estan libres.
	 */
	const char SEM_SURTIDORES_LIBRES = 't';

	/*
	 * proj_id pasado a ftok para generar la key correspondiente al
	 * semaforo que protege la caja.
	 */
	const char SEM_CAJA = 'c';
}

#endif // ESTACION_CONSTANTES_H