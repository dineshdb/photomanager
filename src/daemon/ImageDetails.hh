#ifndef IMAGE_DETAILS_H
#define IMAGE_DETAILS_H

#include <string>
#include <vector>
#include <glibmm.h>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace Glib;
// "(sa((ii)ii)as)

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
  
  Glib::VariantContainerBase getVariant(){
  }
};

#endif
