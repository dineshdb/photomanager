#include "recognizer.hh"

Recognizer::Recognizer(){
	face_cascade_name = "haarcascade_frontalface_alt.xml";
	recognizer_statefile = "lbh.xml";
	if( !face_cascade.load( face_cascade_name ) ) {
		cerr << "Error loading face cascade" << endl;
//    		return -1;
	}
	model = cv::createLBPHFaceRecognizer();
	if(fs::exists(recognizer_statefile)){
		model->load(recognizer_statefile);
	}
}

/**
 * @brief Detects the faces available in an image.
 * This does not try to recognize the faces, it only detects them.
 * @param Matrix of image to identify faces.
 * @return an vector of faces contained in the given image.
 */
std::vector<Rect> Recognizer::getFaces(Mat grayFrame){
	std::vector<Rect> faces;
	equalizeHist( grayFrame, grayFrame );
    	face_cascade.detectMultiScale( grayFrame, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	return faces;
}

/**
 * @brief Crops the face for recognition.
 * @param image Image to be cropped from
 * @param faces to be cropped
 * @return vector of cropped faces.
 */
Mat Recognizer::cropFace(Mat image, Rect face){
	Point tl(face.x, face.y);
	Point dr(face.x + face.width, face.y + face.height);
	Rect f(tl, dr);
	Mat croppedFace = image(f);
	return croppedFace;
}
