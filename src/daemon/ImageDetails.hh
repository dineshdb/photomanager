#ifndef IMAGE_DETAILS_H
#define IMAGE_DETAILS_H

#include <string>
#include <vector>
#include <glibmm.h>

class ImageDetails {

  Glib::ustring path;
//  std::vector<Face> faces;
  std::vector<std::string> tags;
  
public:
  
  ImageDetails(Glib::ustring filename){
  	 path = filename;
  }
  

};

#endif
