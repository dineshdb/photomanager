#ifndef IMAGE_DETAILS_H
#define IMAGE_DETAILS_H

#include <string>
#include <vector>
#include <glibmm.h>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

typedef struct _FaceDetail {
  cv::Rect bounds;
  int label;
  int confidence;
} FaceDetail;

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
};

#endif
