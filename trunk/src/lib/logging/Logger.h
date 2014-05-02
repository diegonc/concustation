#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <sstream>
#include <string>
#include <utils/NonCopyable.h>

class Logger : private NonCopyable
{
	int fd;
	std::string module;

	// buffer donde se recolectan los resultados intermedios del
	// operador << hasta que recibe el manipulador endl y se escribe
	// a disco.
	std::ostringstream buffer;
	// indica si se grabó el nombre del módulo en el buffer.
	bool printedModule;

	// bloquea el archivo de log para escribir en el en forma
	// atómica
	void lock () const;
	// desbloquea el archivo para que otros procesos puedan
	// escribir en el log
	void unlock () const;

	// clase que implementa el patron RAII para bloquear
	// y desbloquear un Logger.
	friend class Locker;

	public:
		Logger (const std::string& filename, const std::string& module);
		~Logger ();

		// operador << para poder escribir en el log
		template<typename T>
		friend Logger& operator<< (Logger& logger, const T& o);

		// soporte para manipuladores
		friend Logger& operator<< (Logger& logger, Logger& (*pf)(Logger&));
		// manipulador Logger::endl, similar a std::endl, permite
		// concluir una linea de log y grabar en el archivo el
		// contenido del buffer en forma atómica
		static Logger& endl (Logger& logger);
};

template<typename T>
Logger& operator<< (Logger& logger, const T& o)
{
	if (!logger.printedModule) {
		logger.buffer << "[" << logger.module << "] ";
		logger.printedModule = true;
	}
	logger.buffer << o;
	return logger;
}

#endif // LOGGIGNG_LOGGER_H
