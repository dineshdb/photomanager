#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;

namespace fs = boost::filesystem;

class Recognizer {
public:
	Recognizer();
	~Recognizer(){
		model->save(recognizer_statefile);
	}

	Mat cropFace(Mat image, Rect face);
	std::vector<Rect> getFaces(Mat frame);
	std::vector<Rect> getFaces(string filename){
		Mat grayFrame = imread(filename, cv::IMREAD_GRAYSCALE);
		return getFaces(grayFrame);
	}
	int predictPhoto(string filename);
	int updatePhoto(string filename, int userid);
	int predictPhoto(Mat face){
		return model->predict(face);
	}
	void updatePhoto(vector<Mat> faces, vector<int> ids){
		model->update(faces, ids);
	}

	Mat loadPhoto(string filename){
		return imread (filename);
	}
	Mat loadPhotoBW(string filename){
		return imread (filename, cv::IMREAD_GRAYSCALE);
	}

private:
	Ptr<FaceRecognizer> model;
	CascadeClassifier face_cascade;
	String face_cascade_name ;
	String recognizer_statefile;

};
#endif
