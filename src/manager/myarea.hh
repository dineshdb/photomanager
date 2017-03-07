#ifndef MY_AREA_H
#define MY_AREA_H

#include <gtkmm.h>
#include <iostream>
#include <glibmm.h>
#include <giomm.h>

class MyArea : public Gtk::DrawingArea{
	public:
	    MyArea();
	    void set_image(Glib::ustring image);
		virtual ~MyArea();

	protected:
		//override default signal handler
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
		Glib::RefPtr<Gdk::Pixbuf> pimage;
};

void MyArea::set_image(Glib::ustring image){

      pimage = Gdk::Pixbuf::create_from_file(image);
      std::cout << "Set the pixbuf of image : in parameterized constructor of MyArea " << std::endl;
      //show at lease a quarter of the image.
      if (pimage)
        set_size_request(pimage->get_width()/2, pimage->get_height()/2);
}

MyArea::MyArea()
{
    std::cout << "In default constructor of MyArea " << std::endl;
}

MyArea::~MyArea(){

}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    std::cout << "In on_draw function" << std::endl;
	if (!pimage)
		return false;

	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	//draw the image in the middle of the drawing area, or (if the image is larger than the drawing area)
	//draw the middle part of the image.
	
	Gdk::Cairo::set_source_pixbuf(cr, pimage, (width - pimage->get_width())/2, (height - pimage->get_height())/2);

	cr->paint();
	return true;
}

#endif
