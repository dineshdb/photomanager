#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <gtkmm.h>


using namespace std;
using namespace Gtk;

// Class Prototype
class MainWindow : public Gtk::ApplicationWindow{

protected:
	Glib::RefPtr<Gtk::Builder> builder;

public:
	MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);//constructor

};

// Class Definition

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::ApplicationWindow(cobject), builder(refGlade)
{
}
#endif
