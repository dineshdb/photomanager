#include <iostream>
#include <string>

#include "dbus-service.hh"
#include "recognizer.hh"
#include "cmd-parser.hh"

// FIXME: Opencv methods don't work propely
// TODO: Add interfaces, objects and methods to dbus

using namespace std;

/* Daemon process that executes in the background.
 * The functions of this daemon is to
 *		- watch for given folder
 *		- respond to changes by updating database
 *
 *
 */
int main(int argc, char *argv[]) {
	std::locale::global(std::locale(""));

 	CommandParser c;
	c.parse(argc, argv);
	return 0;
}
