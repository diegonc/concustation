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
		case 'b':
		{
			char* endptr;
			long burstSize = strtol (arg, &endptr, 10);
			if (*endptr != '\0' || burstSize == 0) {
				argp_failure (state, 1, 0, "BURST_SIZE must be a number greater than 0");
			} else {
				argParser->_burstSize = static_cast<int> (burstSize);
			}
			break;
		}
		case ARGP_KEY_END:
			if (state->arg_num > 0) {
				argp_failure (state, 1, 0, "too many arguments");
			}
			break;
	}
	return 0;
}

namespace {
	struct argp_option options[] = {
		{ "debug", 'd', 0, 0, "Enable debug mode"},
		{ "burst", 'b', "BURST_SIZE", 0, "Produce BURST_SIZE items and exit"},
		{ 0 }
	};

	struct argp optionParser = { options, parserFunc };
}

ArgParser::ArgParser () : _debug(false), _burstSize(0)
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
