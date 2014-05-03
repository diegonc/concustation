#include <Empleado.h>
#include <estacion/constantes.h>
#include <logging/LoggerRegistry.h>

int main (int argc, char **argv)
{
	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.filename (estacion::LOG_FILE);
	registry.application ("empleado");
	// TODO: parsear argv y obtener el flag de depuración
	registry.quiet (false);

	// TODO: parsear argv y obtener el id del empleado
	Empleado empleado (1);
	empleado.run ();
	return 0;
}
