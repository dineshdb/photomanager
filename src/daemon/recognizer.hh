#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <glibmm.h>

#include <mutex>
#include <thread>
#include <map>

#include <boost/filesystem.hpp>

#include "ImageDetails.hh"
#include "DirectoryScanner.hh"

using namespace cv;
using namespace std;

namespace fs = boost::filesystem;

const static String face_cascade_name = "haarcascade_frontalface_alt.xml";
const static String recognizer_statefile = "lbh.xml";

class Recognizer {
  
  Ptr<FaceRecognizer> model;
  CascadeClassifier face_cascade;

public:
  Recognizer(){

	if( !face_cascade.load( face_cascade_name ) ) {
		cerr << "Error loading face cascade" << endl;
	}
	model = cv::createLBPHFaceRecognizer();
	if(fs::exists(recognizer_statefile)){
		model->load(recognizer_statefile);
	}  
  }
  Mat cropFace(Mat image, Rect face){
	Point tl(face.x, face.y);
	Point dr(face.x + face.width, face.y + face.height);
	Rect f(tl, dr);
	Mat croppedFace = image(f);
	return croppedFace;
  }
  
  ImageDetails analyzePhoto(Glib::ustring &filename){
	Mat frame = imread(filename, cv::IMREAD_GRAYSCALE);
   	auto faces = getFaces(frame);
   	ImageDetails image(filename);

   	for(auto face : faces) {
		Mat f = cropFace(frame, face);
		int label;
		double confidence;
		model->predict(f, label, confidence);
		image.addFace(face, label, confidence);	
   	}
   	return image;
  }
  
  void updatePhoto(vector<Mat> faces, vector<int> ids){
	model->update(faces, ids);
  }
  
  void save(){
  	model->save(recognizer_statefile);
  }

  std::vector<Rect> getFaces(Mat frame){
	std::vector<Rect> faces;
	equalizeHist( frame, frame );
   	face_cascade.detectMultiScale( frame, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	return faces;
  }
};

#endif
