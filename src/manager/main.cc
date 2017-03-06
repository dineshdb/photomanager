#include <gtkmm.h>
#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include "proxy.hh"

Gtk::Window *pWindow = nullptr;
// ServerProxy sp;
// when ServerProxy object is declared as global it gives errors as follows
// glibmm-CRITICAL **: Glib::ObjectBase* Glib::wrap_create_new_wrapper(GObject*): assertion 'wrap_func_table != nullptr' failed
// glibmm-WARNING **: Failed to wrap object of type 'GDBusConnection'. 
// Hint: this error is commonly caused by failing to call a library init() function.

bool on_eventbox_button_press(GdkEventButton* button, Glib::RefPtr<Gdk::Pixbuf>& pimage)
{
    if (button->type == GDK_DOUBLE_BUTTON_PRESS)
    {
        std::cout << "Double click handled " << std::endl;
        return true;
    }
    else if (button->type == GDK_BUTTON_PRESS)
    {
        std::cout << "Single click handled " << std::endl;
        return true;
    }
}

void on_my_custom_item_activated(const Glib::ustring& item_name)
{
    if (item_name.compare("photosButton") == 0)
    {   
        // get reply from proxy here
        ServerProxy sp;
        auto listOfFiles = sp.getPhotos();
        for (auto file : listOfFiles)
        	std::cout << file << std::endl;

    	Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());
    	Gtk::ScrolledWindow* scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
        // a Gtk::ApplicationWindow already contains a  Gtk::ScrolledWindow

    	grid->set_column_spacing(10);
    	grid->set_row_spacing(10);

    	scrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        scrolledWindow->add(*grid);

        //this->remove();
        pWindow->add(*scrolledWindow);
		
//		sW->updateContent(nC, filter_fn)
    	int row = 0;
		for (int i = 0; i < listOfFiles.size(); i++)
		{
			Glib::RefPtr<Gdk::Pixbuf> ptr_image;
			try
			{
				ptr_image = Gdk::Pixbuf::create_from_file(listOfFiles[i]);
			}
			catch (const Gio::ResourceError& ex)
			{
				std::cerr << "ResourceError: " << ex.what() << std::endl;
			}
			catch (const Gdk::PixbufError& ex)
			{
				std::cerr << "PixbufError: " << ex.what() << std::endl;
			}

			Gtk::Image* image = Gtk::manage(new Gtk::Image());
			Gtk::EventBox* eventBox = Gtk::manage(new Gtk::EventBox());

			eventBox->set_events(Gdk::BUTTON_PRESS_MASK);
			eventBox->signal_button_press_event().connect(
                                                 sigc::bind</*Glib::ustring*/Glib::RefPtr<Gdk::Pixbuf>&>(
                                                 			sigc::ptr_fun(
                                             						on_eventbox_button_press), /*listOfFiles[i]*/ptr_image));
            eventBox->set_margin_top(10);
            eventBox->set_margin_bottom(10);
            eventBox->set_margin_left(10);
            eventBox->set_margin_right(10);
            eventBox->set_focus_on_click(true);

			image->set(ptr_image->scale_simple(350, 200, Gdk::INTERP_HYPER));
            eventBox->add(*image);

			if (i % 3 == 0)
				row++;

			grid->attach(*eventBox, i % 3, row, 1, 1);
		}
        std::cout << "Ok upto here " << std::endl;
        //show_all();
    }
    else if (item_name.compare("peopleButton"))
    {

    }
    else if (item_name.compare("eventsButton"))
    {

    }
    else if (item_name.compare("tagsButton"))
    {

    }
    else if (item_name.compare("cameraButton"))
    {

    }
    else if (item_name.compare("placesButton"))
    {

    }
}


int main (int argc, char **argv){
  auto app = Gtk::Application::create(argc, argv, "com.dinnux.manager");

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

  Gtk::AppChooserButton *appChooserButton = nullptr;
  Gtk::HeaderBar *headerBar = nullptr;
  //Get the GtkBuilder-instantiated window
  refBuilder->get_widget("mainWindowv2", pWindow);
  refBuilder->get_widget("appChooserButton", appChooserButton);
  refBuilder->get_widget("headerBar", headerBar);

  // Create some icons
  Glib::RefPtr<Gio::Icon> cameraIcon = Gio::Icon::create("camera-photo");
  Glib::RefPtr<Gio::Icon> imageIcon = Gio::Icon::create("image-x-generic");
  Glib::RefPtr<Gio::Icon> icon = Gio::Icon::create("icon");
  
  appChooserButton->append_custom_item( (const gchar *)"photosButton", (const gchar *)" Photos ", imageIcon);
  appChooserButton->append_custom_item( (const gchar *)"peopleButton", (const gchar *)" People ", icon);
  appChooserButton->append_custom_item( (const gchar *)"eventsButton", (const gchar *)" Events ", icon);
  appChooserButton->append_custom_item( (const gchar *)"tagsButton", (const gchar *)" Tags ", icon);
  appChooserButton->append_custom_item( (const gchar *)"cameraButton", (const gchar *)" Camera ", cameraIcon);
  appChooserButton->append_custom_item( (const gchar *)"placesButton", (const gchar *)" Places ", icon);
    
	appChooserButton->signal_custom_item_activated().connect(sigc::ptr_fun(on_my_custom_item_activated));
	
  appChooserButton->set_active_custom_item("photosButton");
  
  pWindow->set_titlebar(*headerBar);
  
  if(pWindow){
    app->run(*pWindow);
  }

  delete pWindow;

  return 0;
}


