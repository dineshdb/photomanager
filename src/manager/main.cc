#include <gtkmm.h>
/**
 *	This is the main GUI for our photomanager application.
 *	It has buttons: titled "People", "Tags", "Open Camera", "Folders"
 *	Button "People" displays all pictures with specific faces or names that are identified and stored in the database
 *	Button "Tags" displays all pictures related with any tags e.g. an event or ocassion
 * 	Button "Open Camera" opens the device camera to capture a photo followed by options to store at specific location or edit
 *	Button "Folders" opens the view of folders that are under constant watch of the underlying daemon of our PhotoManager app
 */

Gtk::Window* pWindow = nullptr;

int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "com.skynet.manager");

  //Load the Gtk::Builder file and instantiate its widgets
  auto refBuilder = Gtk::Builder::create();
  
  try
  {
    refBuilder->add_from_file("manager.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  //Get the GtkBuilder-instantiated window
  
  refBuilder->get_widget("mainWindow", pWindow);
  
  if(pWindow)
  {
    app->run(*pWindow);
  }

  delete pWindow;

  return 0;
}


