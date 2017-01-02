#include <iostream>
#include <gtkmm.h>
/**
 * The entry point for our camera application.
 * It opens the camera stream for picture caption.
 * It can detect faces in the camera and identify the person.
 * Image will be captured and saved on user-action.
 */

Gtk::Window* mainWindow = nullptr;
int main(int argc, char **argv) {
	auto app = Gtk::Application::create(argc,argv, "com.dinnux.Camera");

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("src/camera/camera.glade");

	//Load the Gtk::Builder file and instantiate its widgets
  	auto refBuilder = Gtk::Builder::create();

	try {
    		refBuilder->add_from_file("src/camera/camera.glade");
  	}

  	catch(const Glib::FileError& ex) {
   		std::cerr << "FileError: " << ex.what() << std::endl;
    		return 1;
  	}

  	catch(const Glib::MarkupError& ex){
    		std::cerr << "MarkupError: " << ex.what() << std::endl;
    		return 1;
  	}

  	catch(const Gtk::BuilderError& ex) {
    		std::cerr << "BuilderError: " << ex.what() << std::endl;
    		return 1;
  	}

  	Gtk::HeaderBar *headerBar = nullptr;
  	//Get the GtkBuilder-instantiated window
  	refBuilder->get_widget("mainWindow", mainWindow);
  	refBuilder->get_widget("headerBar", headerBar);


	mainWindow->set_titlebar(*headerBar);

	if(mainWindow) {
    		app->run(*mainWindow);
  	}

  	delete mainWindow;

  	return 0;
	// app->run(*mainWindow);
	return 0;
}
