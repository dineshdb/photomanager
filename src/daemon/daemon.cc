#include <iostream>
#include <string>

#include <libgdamm.h>
#include <gtkmm.h>

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
 
Glib::RefPtr<Gnome::Gda::Connection> conn;
Recognizer r;

void scanFile(Glib::ustring &filename){
  auto faces = r.getFaces(filename);
  
  cout << filename;
  for(auto face : faces){
    cout << face;
  }
  cout << endl;
  
}

// a function that returns the extension of a file
Glib::ustring getFileExt(const Glib::ustring& s) {

  size_t i = s.rfind('.', s.length());
  if (i != Glib::ustring::npos) {
  	return(s.substr(i+1, s.length() - i));
  }
  // Glib::ustring::npos is constant (representing -1)
  // returned by method find when the pattern was not found
  return("");
}

// a function that returns the supported extensions
std::vector<Glib::ustring> getSupportedExts(){
	
	std::vector<Gdk::PixbufFormat> pixbuf_formats = Gdk::Pixbuf::get_formats();
	std::vector<Glib::ustring> extensions;
	for (Gdk::PixbufFormat pf : pixbuf_formats)
	{
		auto a = pf.get_extensions();
		// append to extensions vector
		extensions.insert(extensions.end(), a.begin(), a.end());
	}
	
	return extensions;
}

void scanFolders(){
  // TODO Diff these files with database, then patch the database with new info.
  DirectoryScanner scanner;
  scanner.addFolder(Glib::get_home_dir() + "/Pictures");
  scanner.start();
  std::vector<Glib::ustring> files = scanner.getFiles();
  std::vector<Glib::ustring> newFiles;
  
  std::vector<Glib::ustring> ext = getSupportedExts();
  
  std::vector<Glib::ustring>::iterator search_ext;
  std::vector<Glib::ustring>::iterator iter;

  for(iter = files.begin(); iter != files.end(); ++iter){
  	search_ext = find(ext.begin(), ext.end(), getFileExt(*iter));
  	if (search_ext != ext.end()){
		// extension found
		scanFile(*iter);
  	} else {
		std::cout << "Not supported file "<< *iter << std::endl;
		// remove the file
		files.erase(iter);
  	}
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
  		
  scanFolders();
  CommandParser c;
  c.parse(argc, argv);
  return 0;
}

