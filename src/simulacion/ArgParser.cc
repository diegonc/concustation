#include <argp.h>

#include <ArgParser.h>

namespace {
	
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
	argp_parse (0, argc, argv, 0, 0, 0);
}
