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
		
		Glib::RefPtr<Gdk::Pixbuf> pimage;

	protected:
		double scale;
	
		//override default signal handler
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
		bool on_scroll_event(GdkEventScroll* ev);
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
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::SCROLL_MASK | Gdk::SMOOTH_SCROLL_MASK);
	scale = 0.8;
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
	
	cr->scale(scale, scale);	
	Gdk::Cairo::set_source_pixbuf(cr, pimage, (width/2)/scale - pimage->get_width()/2, (height/2)/scale - pimage->get_height()/2);
	cr->rectangle(0, 0, get_allocation().get_width() / scale, get_allocation().get_width()/scale);

	cr->paint();
	return true;
}

bool MyArea::on_scroll_event(GdkEventScroll *ev){
	//update scale according to mouse scrolling
	scale -= ev->delta_y / 10;
	
	//no more scaling below 0.5
	if (scale < 0.5)
		scale = 0.5;
	else if (scale > 2)
		scale = 2;
		
	std::cout << scale << std::endl;
	std::cout.flush();
	
	//update drawing
	queue_draw();
	
	//there is probably a good reason to do this
	return true;
}

#endif
