#include <ArgParser.h>
#include <estacion/constantes.h>
#include <Administrador.h>
#include <iostream>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("administrador");
	registry.filename (estacion::LOG_FILE);
	registry.quiet (!args.debug ());

	try {
		Administrador administrador;
		administrador.run ();
	} catch (SystemErrorException& e) {
		// Se atrapa excepcion para correr destructores
		std::cout << "Error al correr el administrador: "
		          << e.what ()
		          << std::endl;
		return 1;
	}

	return 0;
}
