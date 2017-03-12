#include <iostream>
#include <string>


#include <libgdamm.h>
#include <gtkmm.h>

#include "FileDatabase.hh"
#include "dbus-service.hh"
#include "recognizer.hh"
#include "cmd-parser.hh"
#include "DirectoryScanner.hh"
#include "ImageDetails.hh"

Glib::RefPtr<Gnome::Gda::Connection> conn;
FilesDatabase::FilesDatabase scannedFiles;


// FIXME: Opencv methods don't work propely
// TODO: Add interfaces, objects and methods to dbus

using namespace std;

/* Daemon process that executes in the background.
 * The functions of this daemon is to
 *		- watch for given folder
 *		- respond to changes by updating database
 * Use this daemon as a dbus, server or a normal command line 
 * program.
 * To run this program as a dbus daemon, run `$ daemon -d `
 * You can check it's interface using d-feet program. Open dfeet and 
 * browse to com.dinnux.PhotoManager and see it's exposed functionality.
 *
 * To update the database with face, run `$ daemon -u <image> -i <userid>`
 * The userid field is compulsory but not functional yet.
 *
 * To predict a photo, run `$ daemon -p <image>`
 *
 */


int main(int argc, char *argv[]) {
  std::locale::global(std::locale(""));
  Gnome::Gda::init();
  
  conn = Gnome::Gda::Connection::open_from_string(
  			(const gchar *) "SQLite",
  			(const gchar *) "DB_DIR=.;DB_NAME=phots",
  			(const gchar *) "",
		    (Gnome::Gda::ConnectionOptions) 0
  		);
  		
  Recognizer r;	// Initialization of recognizer
   
  CommandParser c;
  c.parse(argc, argv);
  return 0;
}

