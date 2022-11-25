#include<opencv2/opencv.hpp>//OpenCV header to use VideoCapture class//
#include<iostream>
#include <sys/times.h> // for times
#include <unistd.h> // for sysconf

using namespace std;

int main() {
    cv::Mat myImage;//Declaring a matrix to load the frames//
    cv::Mat myImageColorBlur;//Declaring a matrix to load the frames//
    cv::Mat myImageGeneralBlur;//Declaring a matrix to load the frames//
    int n = 25;
    int fps = 0;
    cv::namedWindow("Video Player");//Declaring the video to show the video//
    cv::VideoCapture cap(0);//Declaring an object to capture stream of frames from third camera//
    long clocks_per_sec = sysconf(_SC_CLK_TCK); // количество квантов времени в секунду
    long clocks;
    struct tms start {}, end{};
    if (!cap.isOpened()) { //This section prompt an error message if no video stream is found//
        cout << "No video stream detected" << endl;
        system("pause");
        return -1;
    }
    while (true) { //Taking an everlasting loop to show the video//
        times(&start);
        cap >> myImage;
        if (myImage.empty()) { //Breaking the loop if no video frame is detected//
            break;
        }
        cv::medianBlur(myImage, myImageColorBlur, 5);
        cv::medianBlur(myImage, myImageGeneralBlur, 3);
        for (int i = 0; i < myImage.rows; ++i) {
            for (int j = 0; j < myImage.cols; ++j) {
                myImage.at<cv::Vec3b>(i, j) = cv::Vec3b(
                    static_cast<int>(myImageColorBlur.at<cv::Vec3b>(i, j).val[0] / n) * n,
                    static_cast<int>(myImageColorBlur.at<cv::Vec3b>(i, j).val[1] / n) * n,
                    static_cast<int>(myImageColorBlur.at<cv::Vec3b>(i, j).val[2] / n) * n
                );
            }
        }
        cv::putText(myImage, "FPS: " + to_string(fps), { 50, 50 }, cv::FONT_HERSHEY_TRIPLEX, 1.5, (255, 255, 255));
        imshow("Video Player", myImage);//Showing the video//
        char c = (char)cv::waitKey(
            25);//Allowing 25 milliseconds frame processing time and initiating break condition//
        if (c == 27) { //If 'Esc' is entered break the loop//
            break;
        }
        times(&end);
        clocks = end.tms_utime - start.tms_utime;
        fps = static_cast<int>(static_cast<double>(clocks_per_sec) / clocks);
    }
    cap.release();//Releasing the buffer memory//
    return 0;
}