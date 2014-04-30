#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <utils/NonCopyable.h>

class ArgParser : private NonCopyable
{
	private:
		bool _debug;
		unsigned _empleados;
		unsigned _surtidores;

		ArgParser();
		~ArgParser();

	public:
		static ArgParser& getInstance();

		void parse (int argc, char **argv);

		bool debug () const { return _debug; }
		unsigned empleados () const { return _empleados; }
		unsigned surtidores () const { return _surtidores; }
};

#endif
