#include <iostream>
#include <string>
#include <sqlite3.h>

#include "dbus-service.hh"
#include "recognizer.hh"
#include "cmd-parser.hh"
#include "DirectoryScanner.hh"

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

 	CommandParser c;
	c.parse(argc, argv);
	return 0;
}

void diffFolders(std::vector<Glib::ustring> &t1, std::vector<Glib::ustring> &t2){
  for(auto entry : t1){
//    if(t2.contains(entry)){
//      t2.remove(entry);
//    }
  }
}

void scanFolders(){
  // TODO Diff these files with database, then patch the database with new info.
  DirectoryScanner scanner;
  scanner.addFolder("/home/dineshdb/Pictures");
  scanner.start();
  std::vector<Glib::ustring> files = scanner.getFiles();
  for(auto file : files){
    std::cout << file << std::endl;
  }  
}
