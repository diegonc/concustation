#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <utils/NonCopyable.h>

class ArgParser : private NonCopyable
{
	private:
		bool _debug;
		unsigned _id;

		ArgParser();
		~ArgParser();

		friend int parserFunc (int key, char *arg, struct argp_state *state);

	public:
		static ArgParser& getInstance();

		void parse (int argc, char **argv);

		bool debug () const { return _debug; }
		unsigned id () const { return _id; }
};

#endif
