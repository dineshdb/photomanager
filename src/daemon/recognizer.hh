#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <glibmm.h>

#include <boost/filesystem.hpp>

#include "ImageDetails.hh"

using namespace cv;
using namespace std;

namespace fs = boost::filesystem;

class Recognizer {
public:
	Recognizer(){
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

	}
	~Recognizer(){
		model->save(recognizer_statefile);
	}
	/**
	 * @brief Crops the face for recognition.
	 * @param image Image to be cropped from
	 * @param faces to be cropped
	 * @return vector of cropped faces.
	 */

	Mat cropFace(Mat image, Rect face){
		Point tl(face.x, face.y);
		Point dr(face.x + face.width, face.y + face.height);
		Rect f(tl, dr);
		Mat croppedFace = image(f);
		return croppedFace;
	}
	/**
	 * @brief Detects the faces available in an image.
	 * This does not try to recognize the faces, it only detects them.
	 * @param Matrix of image to identify faces.
	 * @return an vector of faces contained in the given image.
	 */
	std::vector<Rect> getFaces(Mat frame){
		std::vector<Rect> faces;
		equalizeHist( frame, frame );
	   	face_cascade.detectMultiScale( frame, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
		return faces;
	}
	
	void updatePhoto(vector<Mat> faces, vector<int> ids){
		model->update(faces, ids);
	}

	Mat loadPhotoBW(string filename){
		return imread (filename, cv::IMREAD_GRAYSCALE);
	}
	
	ImageDetails analyzePhoto(Glib::ustring &filename){
		Mat frame = imread(filename, cv::IMREAD_GRAYSCALE);
	   	auto faces = getFaces(frame);

	   	for(auto face : faces) {
			Mat f = cropFace(frame, face);
			int label;
			double confidence;
			model->predict(f, label, confidence);
			// TODO Save to database		
	   	}
	   	return ImageDetails(filename);
	}
	
	
private:
	Ptr<FaceRecognizer> model;
	CascadeClassifier face_cascade;
	String face_cascade_name ;
	String recognizer_statefile;
};
#endif
