/**
 * Sample programming file to resolve simple object identification problem.
 *
 * Setup
 *
 * A moving camera captures a scene that has an object moving freely. 
 * The location of the object in a given time is given in the text file. 
 * Write a program that validates the coordinates in the text file. 
 * An object is “present” by making sure that the pixels in a 3x3 grid are darker 
 * than the average grayscale value of the frame. 
 * Print out all timestamps of the video where this case is not true. 
 * 
 * Three sample videos and accompanying text file 
 *
 * Input: A video and a file with the location of the object in every frame
 * 
 * Expected output: Either command line output or text file output with timestamps 
 * of the video whenever this isn’t true. 
 * 
 * Assumptions: The frame rate is high enough (and the camera and object speeds are low
 * enough) so the changes in apparent size are smooth. 
 *
 * Because this is a naïve approach to identifying objects, 
 * it will not be accurate (so false positives and false negatives will occur). 
 *
 */
#include <iostream>
#include <list>
#include <vector>

#include "opencv2/opencv.hpp"

#include "location_train.h"

using namespace std;
using namespace cv;

static void help(char **argv)
{
	cout << "\nThis program validates the coordinates of the \"object\" in the text file\n\n" << endl;
	cout << "usage: " << argv[0] << " <video file name> <location file name>\n" << endl;
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		help(argv);
		return 1;
	}

	// Verify the input values
	VideoCapture cap(argv[1]); // open the passed video
	
	if (!cap.isOpened()) {		// check if we succeeded
		cout << "\nCan not open video file '" << argv[1] << "'" << endl;
		return -1;
	} else {
		cout << "Video " << argv[1] << endl;
		cout << " width  =" << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
		cout << " height =" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
		cout <<	" nframes=" << cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;
		cout << " fps    =" << cap.get(CV_CAP_PROP_FPS) << endl;
	}


	Mat edges;
	namedWindow("edges", 1);
	//CvScalar cvAvg(const CvArr* arr,const CvArr* mask=NULL ) 
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
