#include <gtkmm.h>
#include <giomm.h>
#include <glibmm.h>
#include <iostream>
/**
 *	This is the main GUI for our photomanager application.
 *	It has buttons: titled "People", "Tags", "Open Camera", "Folders"
 *	Button "People" displays all pictures with specific faces or names that are identified and stored in the database
 *	Button "Tags" displays all pictures related with any tags e.g. an event or ocassion
 * 	Button "Open Camera" opens the device camera to capture a photo followed by options to store at specific location or edit
 *	Button "Folders" opens the view of folders that are under constant watch of the underlying daemon of our PhotoManager app
 */

Gtk::Window* pWindow = nullptr;

const std::string rootFolder = "~/Pictures";

std::vector<Glib::ustring> enumerate_files(const Glib::ustring& path)
{
  std::vector<Glib::ustring> file_names;
  Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path);
  Glib::RefPtr<Gio::FileEnumerator> child_enumeration = file->enumerate_children(G_FILE_ATTRIBUTE_STANDARD_NAME);
  Glib::RefPtr<Gio::FileInfo> file_info;
  while ((file_info = child_enumeration->next_file()))
  {
    file_names.push_back(file_info->get_name());
  }
  return file_names;
}


int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "com.skynet.manager");

// Calling a simple function GetTime in dbus daemon.
  auto connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
  Glib::RefPtr<Gio::Cancellable> cancellable;

  Glib::RefPtr<Gio::DBus::Proxy> proxy = Gio::DBus::Proxy::create_sync(
      connection, 
      "com.dinnux.PhotoManager",                        // service name
      "/com/dinnux/PhotoManager/Photo",                    // object path
      "com.dinnux.PhotoManager.Photo",  				// interface name
      cancellable); 

  Glib::VariantContainerBase result = proxy->call_sync("GetTime", cancellable);  
  Glib::Variant< Glib::ustring > introspection_str; 
  result.get_child(introspection_str); 

  std::cout << "answer is " << introspection_str.get() << std::endl;
  
// End of callng GetTime in dbus

  auto refBuilder = Gtk::Builder::create();
  
  try {
    refBuilder->add_from_file("src/manager/manager.glade");
  } catch(const Glib::FileError& ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  } catch(const Glib::MarkupError& ex) {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  } catch(const Gtk::BuilderError& ex) {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  Gtk::AppChooserButton *appChooser = nullptr;
  Gtk::HeaderBar *headerBar = nullptr;
  //Get the GtkBuilder-instantiated window
  refBuilder->get_widget("mainWindowv2", pWindow);
  refBuilder->get_widget("appChooserButton", appChooser);
  refBuilder->get_widget("headerBar", headerBar);
  

  // Create some icons
  Glib::RefPtr<Gio::Icon> cameraIcon = Gio::Icon::create("camera-photo");
  Glib::RefPtr<Gio::Icon> imageIcon = Gio::Icon::create("image-x-generic");
  Glib::RefPtr<Gio::Icon> icon = Gio::Icon::create("icon");
  
  appChooser->append_custom_item( (const gchar *)"photosButton", (const gchar *)" Photos ", imageIcon);
  appChooser->append_custom_item( (const gchar *)"peopleButton", (const gchar *)" People ", icon);
  appChooser->append_custom_item( (const gchar *)"eventsButton", (const gchar *)" Events ", icon);
  appChooser->append_custom_item( (const gchar *)"tagsButton", (const gchar *)" Tags ", icon);
  appChooser->append_custom_item( (const gchar *)"cameraButton", (const gchar *)" Camera ", cameraIcon);
  appChooser->append_custom_item( (const gchar *)"placesButton", (const gchar *)" Places ", icon);
    
  appChooser->set_active_custom_item("photosButton");
  
  pWindow->set_titlebar(*headerBar);
  
  if(pWindow)
  {
    app->run(*pWindow);
  }

  delete pWindow;

  return 0;
}


