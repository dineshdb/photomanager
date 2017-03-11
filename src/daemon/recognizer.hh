#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <glibmm.h>

#include <mutex>
#include <thread>

#include <boost/filesystem.hpp>

#include "ImageDetails.hh"
#include "DirectoryScanner.hh"

using namespace cv;
using namespace std;

namespace fs = boost::filesystem;

namespace{
static std::vector<ImageDetails> scannedFiles;
static std::mutex scannedFilesLock;
  
static Ptr<FaceRecognizer> model;
 
static CascadeClassifier face_cascade;
static String face_cascade_name ;
static String recognizer_statefile;

static bool isInitialized;

}
class Recognizer {
public:

  Recognizer(){
    init();
  }
  
  static void init(){
    if(isInitialized) return;
    
    cout << "Initializing recognizer" << endl;
    
	face_cascade_name = "haarcascade_frontalface_alt.xml";
	recognizer_statefile = "lbh.xml";
	if( !face_cascade.load( face_cascade_name ) ) {
		cerr << "Error loading face cascade" << endl;
		// TODO : Implement backup plan for it.
	}
	model = cv::createLBPHFaceRecognizer();
	if(fs::exists(recognizer_statefile)){
		model->load(recognizer_statefile);
	}
	isInitialized = true;
  }

  static std::vector<Rect> getFaces(Mat frame){
	std::vector<Rect> faces;
	equalizeHist( frame, frame );
   	face_cascade.detectMultiScale( frame, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	return faces;
  }

  static Mat cropFace(Mat image, Rect face){
	Point tl(face.x, face.y);
	Point dr(face.x + face.width, face.y + face.height);
	Rect f(tl, dr);
	Mat croppedFace = image(f);
	return croppedFace;
  }

  void updatePhoto(vector<Mat> faces, vector<int> ids){
	model->update(faces, ids);
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

  static void scanFolders(){
      std::vector<Glib::ustring> folders;
      folders.push_back(Glib::get_home_dir() + "/Pictures/test");
	  DirectoryScanner s(folders);
	  s.start();
	  // TODO Diff the result with preprocessed files
	  auto files = s.getFiles(); 
	  Recognizer r;
	  
	  for( auto file : files){
	    auto details = r.analyzePhoto(file);
		  {
			std::lock_guard<mutex> l(scannedFilesLock);
			scannedFiles.push_back(details);
			cout << "Scanned  :" << file << endl;
			// TODO Save the result to database
		  }
	  }
	  cout << "Finished scanning files" << endl;
  }
  
  static std::vector<ImageDetails> getFiles(){
    lock_guard<mutex> l(scannedFilesLock);
    return scannedFiles; 
  }

};
#endif
