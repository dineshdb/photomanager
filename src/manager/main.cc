#include <gtkmm.h>
#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include "mainWindow.hh"

// ServerProxy sp;
// when ServerProxy object is declared as global it gives errors as follows
// glibmm-CRITICAL **: Glib::ObjectBase* Glib::wrap_create_new_wrapper(GObject*): assertion 'wrap_func_table != nullptr' failed
// glibmm-WARNING **: Failed to wrap object of type 'GDBusConnection'. 
// Hint: this error is commonly caused by failing to call a library init() function.

int main (int argc, char **argv){
<<<<<<< HEAD
	  auto app = Gtk::Application::create(argc, argv, "com.dinnux.manager");
	  
	  MainWindow main;
	  
	  return app->run(*main.appWindow);
>>>>>>> crap
}


