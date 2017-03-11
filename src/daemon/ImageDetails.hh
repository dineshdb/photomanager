#ifndef IMAGE_DETAILS_H
#define IMAGE_DETAILS_H

#include <string>
#include <vector>
#include <glibmm.h>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace Glib;
// "(sa((iiii)ii)as)

class FaceDetail {
public:
  cv::Rect bounds;
  int label;
  int confidence;
  
  FaceDetail(cv::Rect b, int l, int c): bounds(b), label(l), confidence(c){}
};

class ImageDetails {

  Glib::ustring path;
  std::vector<FaceDetail> faces;
  std::vector<std::string> tags;
  
public:
  
  ImageDetails(Glib::ustring filename){
  	 path = filename;
  }
  
  void addFace(cv::Rect face, int label, int confidence){
    faces.push_back(FaceDetail{face, label, confidence});
  }
  
  void addTag(Glib::ustring &tag){
    // TODO Check whether the tag already exists
    tags.push_back(tag);
  }
  
  Glib::ustring getString(){
  	Glib::ustring literally_crap = "";				// an ImageDetails contains path(ustring), tags(vector of strings)
  													// faces(vector of integers)
		literally_crap += path + "#";				// space "#" is used as a delimiter here
		
		// get the face details
		for (auto face_detail : faces)
		{
			// changing integer into strings
			literally_crap += Glib::ustring::format(face_detail.bounds.x) + '$';		// delimiter at the end of each integer
			literally_crap += Glib::ustring::format(face_detail.bounds.y) + '$';
			literally_crap += Glib::ustring::format(face_detail.bounds.width) + '$';
			literally_crap += Glib::ustring::format(face_detail.bounds.height) + '$';
			literally_crap += Glib::ustring::format(face_detail.label) + '$';
			literally_crap += Glib::ustring::format(face_detail.confidence) + '$';
			literally_crap += '%';														// a delimiter for each face
		}
		
		literally_crap += "#";						// delimiter at the beginning of tags
		// get the tags
		for (auto tag : tags)
		{
			literally_crap += tag + "*";			// delimiter at the end of each tag
		}		
  	
  	return literally_crap;
  }
};

#endif
