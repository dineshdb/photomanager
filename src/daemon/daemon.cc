#include <iostream>
#include <string>

#include <libgdamm.h>
#include <gtkmm.h>
#include <mutex>
#include <thread>

#include "dbus-service.hh"
#include "recognizer.hh"
#include "cmd-parser.hh"
#include "DirectoryScanner.hh"
#include "ImageDetails.hh"

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
 
Glib::RefPtr<Gnome::Gda::Connection> conn;

static mutex dbLock;

void scanFolders(){
  Recognizer r;
  // TODO Diff these files with database, then patch the database with new info.
  DirectoryScanner scanner;
  scanner.addFolder(Glib::get_home_dir() + "/Pictures/test");
  scanner.start();
  std::vector<Glib::ustring> files = scanner.getFiles();

  for(auto file : files){
	r.analyzePhoto(file);
  }
}
int main(int argc, char *argv[]) {
  std::locale::global(std::locale(""));
  Gnome::Gda::init();
  
  conn = Gnome::Gda::Connection::open_from_string(
  			(const gchar *) "SQLite",
  			(const gchar *) "DB_DIR=.;DB_NAME=phots",
  			(const gchar *) "",
		    (Gnome::Gda::ConnectionOptions) 0
  		);
  
  thread t(scanFolders);  // Runs scanning in another thread  
  		
  CommandParser c;
  c.parse(argc, argv);
  t.join();
  return 0;
}

