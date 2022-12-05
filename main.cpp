//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	//Open the default video camera
	VideoCapture cap(0);

	double fps = cap.get(CAP_PROP_FPS);
	cout << "Frames per second camera : " << fps << endl;

	// Number of frames to capture
	int num_frames = 1;
	int fps_count = 1;

	// Start and end times
	clock_t fps_start, start_capture, start_processing, full_start;
	clock_t fps_end, end_capture, end_processing, full_end;

	Mat frame;

	cout << "Capturing " << num_frames << " frames" << endl;

	double fpsLive = 1;
	double total_capture_time = 0;
	double processing_time = 0;
	double capture_time = 0;

	// if not success, exit program
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video camera" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

	string window_name = "My Camera Feed";
	namedWindow(window_name); //create a window called "My Camera Feed"
	fps_start = clock();
	full_start = clock();
	while (true)
	{
		Mat frame;
		start_capture = clock();
		bool bSuccess = cap.read(frame); // read a new frame from video 
		end_capture = clock();
		capture_time += (double(end_capture) - double(start_capture)) / double(CLOCKS_PER_SEC);

		//Breaking the while loop if the frames cannot be captured
		if (bSuccess == false)
		{
			cout << "Video camera is disconnected" << endl;
			cin.get(); //Wait for any key press
			break;
		}

		start_capture = clock();
		cap >> frame;
		end_capture = clock();
		capture_time += (double(end_capture) - double(start_capture)) / double(CLOCKS_PER_SEC);

		num_frames += 1;

		start_processing = clock();
		//cvtColor(frame, frame, COLOR_BGR2HSV, 1); //CHANGE COLOR MODEL
		cvtColor(frame, frame, COLORMAP_PINK, 1); //GREY FILTER
		//frame.convertTo(frame, -1, 0.5, 0); //decrease the contrast by 0.5
		//blur(frame, frame, Size(25, 25));
		end_processing = clock();
		processing_time += (double(end_processing) - double(start_processing)) / double(CLOCKS_PER_SEC);

		putText(frame, "FPS: " + to_string(fpsLive), { 50, 50 }, FONT_HERSHEY_COMPLEX, 1.5, (255, 255, 255));
		//show the frame in the created window
		imshow(window_name, frame);

		//wait for for 10 ms until any key is pressed.  
		//If the 'Esc' key is pressed, break the while loop.
		//If the any other key is pressed, continue the loop 
		//If any key is not pressed withing 10 ms, continue the loop 
		if (waitKey(33) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}

		fps_end = clock();
		double seconds = (double(fps_end) - double(fps_start)) / double(CLOCKS_PER_SEC);

		if (seconds >= 1) {
			fps_start = fps_end;
			cout << "Time taken : " << seconds << " seconds" << endl;
			fpsLive = double(num_frames) / double(seconds);
			cout << "Estimated frames per second : " << fpsLive << endl;
			fps_count += num_frames;
			num_frames = 0;
		}
	}
	full_end = clock();
	total_capture_time = (full_end - full_start) / double(CLOCKS_PER_SEC);
	cout << "Total capture time " << total_capture_time << endl;
	cout << "Processing time " << processing_time << endl;
	cout << "Capture frame time " << capture_time << endl;
	cout << "Percent processing time " << processing_time / total_capture_time * 100 << endl;
	cout << "Percent of capture frame time " << capture_time / total_capture_time * 100 << endl;
	cout << "Frames in total " << fps_count << endl;
	cap.release();//Releasing the buffer memory
	return 0;
}
