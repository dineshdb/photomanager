#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <cstdlib>
#include "../daemon/ImageDetails.hh"
#include "myarea.hh"
#include "proxy.hh"

std::vector<Glib::ustring> get_various_strings(Glib::ustring& general_string, char c)
{
	std::size_t general_pos;
	std::vector<Glib::ustring> general_vector;
	while ((general_pos = general_string.find(c)) != std::string::npos)
	{
		general_vector.push_back(general_string.substr(0, general_pos));
		general_string = general_string.substr(general_pos + 1);	
	}
	return general_vector;
}

class MainWindow
{
protected:
    Gtk::HeaderBar *headerBar = nullptr;
    Gtk::Button *left = nullptr;
    Gtk::Button *rotate = nullptr;
    Gtk::SearchEntry *searchEntry = nullptr;
    Gtk::AppChooserButton *appChooserButton = nullptr;
    Gtk::Box *box = nullptr;
    Gtk::Grid grid;
    Gtk::ScrolledWindow scrolledWindow;
    MyArea area;
    Gtk::Statusbar statusBar;

    const Glib::RefPtr<Gtk::Builder> refBuilder;
    Glib::ustring mainWindowGlade;


public:
    MainWindow();
    virtual ~MainWindow();
    void load_scrolled_window(); 
    void load_single_image(Glib::ustring image);

	Gtk::ApplicationWindow *appWindow = nullptr;

    // signal handlers
    void on_my_custom_item_activated(const Glib::ustring& item_name);
    bool on_eventbox_button_press(GdkEventButton* button, Glib::ustring file/*Glib::RefPtr<Gdk::Pixbuf>& pimage*/);
	void on_back_button_clicked();    
	void on_rotate_button_clicked();
};

// destructor here
MainWindow::~MainWindow()
{
	std::cout << "MainWindow destructor " << std::endl;
}

// constructor here
MainWindow::MainWindow() : mainWindowGlade("src/manager/gui.glade")
{

    auto refBuilder = Gtk::Builder::create();

    try
    {
        refBuilder->add_from_file(mainWindowGlade);
    }
    catch(const Glib::FileError& ex)
    {
        std::cerr << "FileError: " << ex.what() << std::endl;
    }
    catch(const Glib::MarkupError& ex)
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
    }
    catch(const Gtk::BuilderError& ex)
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
    }

	refBuilder->get_widget("applicationWindow", appWindow);
    refBuilder->get_widget("headerBar", headerBar);
    refBuilder->get_widget("left", left);
    refBuilder->get_widget("rotate", rotate);
    refBuilder->get_widget("searchEntry", searchEntry);
    refBuilder->get_widget("appChooserButton", appChooserButton);
    refBuilder->get_widget("box", box);
    
    left->set_sensitive(false);
    rotate->set_sensitive(false);
    left->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_back_button_clicked));
    rotate->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_rotate_button_clicked));
    
    appWindow->set_titlebar(*headerBar);
    appWindow->set_default_size(1110, 700);
    appWindow->add(*box);

    Glib::RefPtr<Gio::Icon> cameraIcon = Gio::Icon::create("camera-photo");
    Glib::RefPtr<Gio::Icon> imageIcon = Gio::Icon::create("image-x-generic");
    Glib::RefPtr<Gio::Icon> icon = Gio::Icon::create("icon");

    appChooserButton->append_custom_item((const gchar *)"photosButton", (const gchar *)" Photos ", imageIcon);
    appChooserButton->append_custom_item((const gchar *)"peopleButton", (const gchar *)" People ", icon);
    appChooserButton->append_custom_item((const gchar *)"eventsButton", (const gchar *)" Events ", icon);
    appChooserButton->append_custom_item((const gchar *)"tagsButton", (const gchar *)" Tags ", icon);
    appChooserButton->append_custom_item((const gchar *)"cameraButton", (const gchar *)" Camera ", cameraIcon);
    appChooserButton->append_custom_item((const gchar *)"placesButton", (const gchar *)" Places ", icon);

    appChooserButton->signal_custom_item_activated().connect(sigc::mem_fun(*this, &MainWindow::on_my_custom_item_activated));

    appChooserButton->set_active_custom_item("photosButton");
    //show_all_children(true);
}

// member functions from here
void MainWindow::load_scrolled_window()
{
	ServerProxy sp;
	auto listOfFiles = sp.getPhotos();
	
    Glib::ustring file_name;
	std::vector<Glib::ustring> tags;
	std::vector<FaceDetail> faces;
	
    grid.set_column_spacing(10);
    grid.set_row_spacing(10);

    scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrolledWindow.remove_with_viewport();												/* right here */
    scrolledWindow.add(grid);
    
    if (!(box == nullptr)){
    	auto children = box->get_children();
    	for (auto child : children)
    		box->remove(*child);
    }
    
    box->pack_start(scrolledWindow);

    int row = 0;
	for (unsigned int i = 0; i < listOfFiles.size(); i++)
	{
		Glib::RefPtr<Gdk::Pixbuf> ptr_image;
		Glib::ustring formatted;
		try
		{
		
			
			// ------------------------------------------------
			// find the space delimiter and obtain only the path of file for now
			std::size_t path_pos = listOfFiles[i].find('#');
			if (path_pos != std::string::npos)
				file_name = listOfFiles[i].substr(0, path_pos);
			
			std::cout << file_name << std::endl;						// debug

			// ------------------------------------------------
			// find the tags at the end of the string 
			// find the last occurance of '#' delimiter
			std::size_t	tags_pos = listOfFiles[i].rfind('#');
			Glib::ustring tags_string;
			
			if (tags_pos != std::string::npos)
				tags_string = listOfFiles[i].substr(tags_pos + 1);		// extract until the end of the string
			
			std::cout << tags_string << std::endl;						// debug
			
			tags = get_various_strings(tags_string, '*');
			
			for (auto tag : tags)
				std::cout << tag << std::endl;							// debug
			
			//-------------------------------------------------
			// get the integers out of string
			std::vector<Glib::ustring> damn_integers;
			Glib::ustring facedetails_string = listOfFiles[i].substr(path_pos + 1, tags_pos - path_pos - 1);
			
			std::size_t each_facedetail_pos;
			std::vector<Glib::ustring> each_facedetail_string; 
			
			std::vector<FaceDetail> temp_faces;
			
			// find '%' because each facedetail ends with '%'
			each_facedetail_string = get_various_strings(facedetails_string, '%');
			
			// find '$' because each integer ends with '$'
			for (auto finally_int_string : each_facedetail_string)
			{
				damn_integers = get_various_strings(finally_int_string, '$');
				// there are 6 integers x, y, width, length, label, confidence
				temp_faces.push_back(
								FaceDetail(
										   cv::Rect(
													atoi(damn_integers[0].c_str()) /* x */, atoi(damn_integers[1].c_str()) /* y */, 
													atoi(damn_integers[2].c_str())/*width*/, atoi(damn_integers[3].c_str()) /*length*/
													), 
													atoi(damn_integers[4].c_str()) /*label*/, 
													atoi(damn_integers[5].c_str())/*confidence*/
										  )
								);
			}
			faces = temp_faces;
			//-------------------------------------------------
			
			// make formatted string here;
			
			formatted = "File: (" + file_name + ")";
	
			formatted += "Tags: ";
			for (auto tag : tags)
				formatted += tag + " ";
	
			formatted += "No. of faces: " + Glib::ustring::format(faces.size()) + " ";
			for (auto face : faces)
				formatted += "x=" + Glib::ustring::format(face.bounds.x) + " y=" + Glib::ustring::format(face.bounds.y) +
				  	 " width=" + Glib::ustring::format(face.bounds.width) + " height=" + Glib::ustring::format(face.bounds.height) +
				  	 " label=" + Glib::ustring::format(face.label) + " confidence=" + Glib::ustring::format(face.confidence);
			
			
			//-------------------------------------------------
			
			ptr_image = Gdk::Pixbuf::create_from_file(file_name);
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
	                                               sigc::bind<Glib::ustring>(
                                                                sigc::mem_fun(*this,
                                                                 	&MainWindow::on_eventbox_button_press), formatted));
        
        eventBox->set_margin_top(10);
        eventBox->set_margin_bottom(10);
        eventBox->set_margin_left(10);
        eventBox->set_margin_right(10);
        eventBox->set_focus_on_click(true);

		image->set(ptr_image->scale_simple(350, 200, Gdk::INTERP_HYPER));
        eventBox->add(*image);

		if (i % 3 == 0)
			row++;

		grid.attach(*eventBox, i % 3, row, 1, 1);
		}
        std::cout << "Ok upto here " << std::endl;
        appWindow->show_all();
}

void MainWindow::load_single_image(Glib::ustring image)
{
	std::size_t front = image.find('(');
	std::size_t back = image.find(')');
	std::cout << image.substr(front + 1, back - front - 1) << std::endl;
	this->area.set_image(image.substr(front + 1, back - front - 1));
	box->pack_start(area);
	box->pack_start(statusBar);
	statusBar.remove_all_messages();
	statusBar.push(image);
	appWindow->show_all();
}


void MainWindow::on_back_button_clicked()
{
	// TODO: review editing of titlebar
	headerBar->add(*appChooserButton);
	headerBar->remove(*left);
	headerBar->remove(*rotate);
	
	left->set_sensitive(false);
	rotate->set_sensitive(false);
	
	this->load_scrolled_window();
	headerBar->set_title("Photo Manager");
}


// signal handlers from here
void MainWindow::on_rotate_button_clicked()
{
	area.pimage = area.pimage->rotate_simple(Gdk::PIXBUF_ROTATE_CLOCKWISE);
	area.queue_draw();
	std::cout << "Rotate button clicked " << std::endl;
}


void MainWindow::on_my_custom_item_activated(const Glib::ustring& item_name)
{
    if (item_name.compare("photosButton") == 0)
    {
	    // TODO: review editing of titlebar
    	headerBar->remove(*left);
    	headerBar->remove(*rotate);
		this->load_scrolled_window();
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

bool MainWindow::on_eventbox_button_press(GdkEventButton* button, Glib::ustring image)
{
    if (button->type == GDK_DOUBLE_BUTTON_PRESS)
    {
    	// TODO: review editing of titlebar;
    	headerBar->add(*left);
    	headerBar->add(*rotate);
    	left->set_sensitive(true);
    	rotate->set_sensitive(true);
    	headerBar->remove(*appChooserButton);
    	
    	auto children = box->get_children();
    	for (auto child : children)
 			box->remove(*child);			
 			
    	std::cout << "removed scrolled window " << std::endl;
        this->load_single_image(image);
        return true;
    }
    else if (button->type == GDK_BUTTON_PRESS)
    {
        std::cout << "Single click handled " << std::endl;
        return true;
    }
}

#endif
