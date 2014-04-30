#include <argp.h>
#include <cstdlib>

#include <ArgParser.h>

int parserFunc (int key, char *arg, struct argp_state *state)
{
	ArgParser* argParser = static_cast<ArgParser*> (state->input);
	switch (key) {
		case 'd':
			argParser->_debug = true;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				unsigned long emps = strtoul(arg, NULL, 10);
				if (emps == 0) {
					argp_failure (state, 1, 0, "the EMPLEADOS argument must be a number greater than 0");
				} else {
					argParser->_empleados = static_cast<unsigned> (emps);
				}
			} else if (state->arg_num == 1) {
				unsigned long surtidores = strtoul(arg, NULL, 10);
				if (surtidores == 0) {
					argp_failure (state, 1, 0, "the SURTIDORES argument must be a number greater than 0");
				} else {
					argParser->_surtidores = static_cast<unsigned> (surtidores);
				}
			}
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 2) {
				argp_failure (state, 1, 0, "too few arguments");
			} else if (state->arg_num > 2) {
				argp_failure(state, 1, 0, "too many arguments");
			}
			break;
	}
	return 0;
}

namespace {
	struct argp_option options[] = {
		{ "debug", 'd', 0, 0, "Enable debug mode"},
		{ 0 }
	};

	struct argp optionParser = { options, parserFunc, "EMPLEADOS SURTIDORES" };
}

ArgParser::ArgParser () : _debug(false), _empleados(0), _surtidores(0)
{
}

ArgParser::~ArgParser()
{
}

ArgParser& ArgParser::getInstance()
{
	static ArgParser parser;
	return parser;
}

void ArgParser::parse(int argc, char** argv)
{
	argp_parse (&optionParser, argc, argv, 0, 0, this);
}
