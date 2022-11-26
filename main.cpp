//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Ptr<BackgroundSubtractor> pBackSub;
	pBackSub = createBackgroundSubtractorKNN();

	//Open the default video camera
	VideoCapture cap(0);

	double fps = cap.get(CAP_PROP_FPS);
	cout << "Frames per second camera : " << fps << endl;

	// Number of frames to capture
	int num_frames = 1;

	// Start and end times
	clock_t start;
	clock_t end;

	Mat frame, fgMask;

	cout << "Capturing " << num_frames << " frames" << endl;

	double ms, fpsLive;

	int keyboard;

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

	while (true)
	{
		Mat frame, fgMask;
		bool bSuccess = cap.read(frame); // read a new frame from video 
		
		//Breaking the while loop if the frames cannot be captured
		if (bSuccess == false)
		{
			cout << "Video camera is disconnected" << endl;
			cin.get(); //Wait for any key press
			break;
		}

		start = clock();
		cap >> frame;
		pBackSub->apply(frame, fgMask);
		long sum = 0;
		int N = 1;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				sum += 1;
			}
		}
		end = clock();
		double seconds = (double(end) - double(start)) / double(CLOCKS_PER_SEC);
		cout << "Time taken : " << seconds << " seconds" << endl;
		fpsLive = double(num_frames) / double(seconds);
		cout << "Estimated frames per second : " << fpsLive << endl;

		//cvtColor(frame, frame, COLOR_BGR2HSV, 1); //CHANGE COLOR MODEL
		//cvtColor(frame, frame, COLORMAP_PINK, 1); //GREY FILTER
		frame.convertTo(frame, -1, 0.5, 0); //decrease the contrast by 0.5
		//blur(frame, frame, Size(25, 25));
		
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
	}
	cap.release();//Releasing the buffer memory
	return 0;
}
