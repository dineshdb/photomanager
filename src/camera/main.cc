#include <iostream>
#include "MainWindow.hh"
/**
 * The entry point for our camera application.
 * It opens the camera stream for picture caption.
 * It can detect faces in the camera and identify the person.
 * Image will be captured and saved on user-action.
 */

int main(int argc, char **argv) {
	auto app =Gtk::Application::create(argc,argv, "com.dinnux.Camera");

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("src/camera/MainWindow.glade");

	MainWindow *mainWindow = 0;
	builder->get_widget_derived("mainwindow", mainWindow);
//	auto appMenu = builder->get_object("appmenu");
//	auto topMenuButton = Glib::RefPtr<Gtk::MenuButton>::cast_dynamic(builder->get_object("topmenu"));

//	auto app_menu = Glib::RefPtr<Gio::MenuModel>::cast_dynamic(appMenu);
//	topMenuButton->set_menu_model(app_menu);
//	if(app_menu){
//		app->set_app_menu(app_menu);
//	} else {
//		cout << "Could not set app menu"<< endl;
//	}


	app->run(*mainWindow);
	return 0;
}
