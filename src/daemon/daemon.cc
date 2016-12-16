#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

namespace fs = boost::filesystem;
/**
 * Daemon process that executes in the background.
 * The functions of this daemon is to 
 *		- watch for given folder
 *		- respond to changes by updating database
 *	Uses libuv for async I?O and inotify/epoll for dir watching
 *
 */
int main(int argc, char *argv[]) {

    return 0;
}

/**
 * @brief Detects the faces available in an image.
 * This does not try to recognize the faces, it only detects them.
 * @param Matrix of image to identify faces.
 * @return an vector of faces contained in the given image.
 */

std::vector<Rect> getFaces(const string &str){
	std::vector<Rect> faces;
	Mat frame = cv::imread(str);
	cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
	equalizeHist( frame, frame );
    face_cascade.detectMultiScale( frame, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	return faces;
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

