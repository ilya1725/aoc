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

/**
 * Helper function to figure out if the specified location coordinates
 * are within the specified point. The point is described by central
 * coordinates and size.
 */
static bool inPoint(Point2f pt, float size, location_train::point_t location)
{
	auto dx = abs(location.x - pt.x);
	auto dy = abs(location.y - pt.y);

	return ((dx*dx + dy*dy) < size*size);
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		help(argv);
		return 1;
	}

	// Verify the input values
	//VideoCapture cap(argv[1]); // open the passed video

	VideoCapture cap;

	// Futile attempt to try differetn codecs
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('D', 'I', 'V', '4'));
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('D', 'A', 'V', 'C'));
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('3', 'I', 'V', '2'));
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('3', 'I', 'V', 'X'));
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('A', 'V', 'C', '1'));
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('H', '2', '6', '4'));
	cap.open(argv[1]);
	
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

	// Load the trail of locations
	location_train locations;

	if (locations.load(argv[2]) != location_train::error_code::no_error) {
		cout << "Cannot load the location file '" << argv[2] << "'" << endl;
		return -1;
	}

	// do the simple sanity check
	if (locations.getCount() != cap.get(CV_CAP_PROP_FRAME_COUNT)) {
		cout << "Data points don't match." << endl;
		cout << " n frames   =" << cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;
		cout << " n locations=" << locations.getCount() << endl;
		return -1;
	}

	location_train::point_t ul{ 0,0 };
	location_train::point_t lr{ (unsigned long)cap.get(CV_CAP_PROP_FRAME_WIDTH),(unsigned long)cap.get(CV_CAP_PROP_FRAME_HEIGHT) };

	if (locations.verify(ul, lr) != location_train::error_code::no_error) {
		cout << "Data points don't fit into video space." << endl;
		return -1;
	}

	// Set up the detector with default parameters.
	SimpleBlobDetector detector;

	auto loc_index = 0;
	auto fps = cap.get(CV_CAP_PROP_FPS);

	// Process frame by frame
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from the file
		double frame_time = loc_index / fps;

		// Detect blobs.
		std::vector<KeyPoint> keypoints;
		detector.detect(frame, keypoints);
		
		// No need to check the range since we already verified that the number of locations
		// is the same as the number of frames
		auto location = locations[loc_index];
		loc_index++;

		if (keypoints.size() == 0) {
			cout << "Error: No objects found at time: " << frame_time << endl;
		}
		bool located = false;
		for ( auto key : keypoints ) {
			// The found blob should be at least 3x3
			if (key.size > 3) {
				if (inPoint(key.pt, key.size, location)) {
					located = true;
					break;
				}
			}
		}
		if (!located) {
			cout << "Error: No objects at time: " << frame_time << "located at expected position" << endl;
		}
	}

	// the video file will be deinitialized automatically in VideoCapture destructor
	return 0;
}
