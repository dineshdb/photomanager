#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <iostream>
#include <boost/program_options.hpp>

#include "dbus-service.hh"
#include "recognizer.hh"

namespace cmd= boost::program_options;

using namespace std;

class CommandParser {
public:
	CommandParser(){}
	int parse(int argc, char **argv);
private:

};
#endif
