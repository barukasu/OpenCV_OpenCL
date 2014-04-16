/***************************************************************************************/
//
//	Author:		Biruh Tesfaye & Rich Shoff
//	Date:		4/12/2014
//
//	Purpose:	This file contains the Sobel filter OCL benchmark test function.
//
//	Usage:		Press 'o' to switch between OCL and CPU, press 'q' to return to main menu.
//
/***************************************************************************************/

#include "perf_precomp.hpp"
#include <opencv2/ocl/ocl.hpp>
#include "h_oclPerfStats_ShoffTesfaye.h"
#include "h_BM_Functions_ShoffTesfaye.h"

using namespace cv;  // OpenCL defined under this namespace!!!
using namespace std;

int sobelOCL_ShoffTesfaye()
{
	/********** - Program-wide Variable Declarations - **********/
	int64 opTimePre = 0;
	int64 opTimePost = 0;

	Mat frame;
    Mat grayFrame;
	Mat textBG = Mat::zeros(75, 200, CV_8UC1);

    bool OPENOCL = false;

    string fpsForImg = "";
	string opTimeForImg = "";
	
	char k;
	/************************************************************/

	//Initialize video input:
    VideoCapture cap(0);  // Open the default camera.
    if (!cap.isOpened())  // check if camera opened.
        return -1;
	
	namedWindow("OpenCL Demo: Sobel Filter", 1);  // Create video display window.

    for (;;)  // Infinite read video loop.
    {
        cap >> frame;  // Get a new frame from camera.

        cvtColor(frame, grayFrame, CV_BGR2GRAY);


		// IF Statement switches between CPU and GPU (OpenCL) if 'o' is pressed!
        if (OPENOCL)
        {
            ocl::oclMat oclFrameSrc(grayFrame);  // Convert frame to OCL frame for operation.
            ocl::oclMat oclFrameDest;

			/********** Actual operations w/ time calc **********/
			opTimePre = cv::getTickCount();
			ocl::Sobel(oclFrameSrc, oclFrameDest, oclFrameSrc.depth(), 1, 1, 5, 1, 0.0, BORDER_DEFAULT);
			opTimePost = cv::getTickCount();

			fpsForImg = calcImgFPS();  // Function obtains FPS, returns string.
			opTimeForImg = averageOpTime(opTimePre, opTimePost);  // Function averages Op times, returns string.
			/****************************************************/

            Mat frameDest(oclFrameDest);  // Convert OCL frame back to OpenCV frame for imshow.

			// Place CPU/GPU, FPS, and Operation time on frame:
			Mat textBGROI = frameDest(cv::Rect(0,0,200,75)); 
			cv::addWeighted(textBGROI,0.0,textBG,1.0,0.,textBGROI);
            putText(frameDest, "GPU (OpenCL)", cvPoint(8, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
            putText(frameDest, fpsForImg, cvPoint(8, 40), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
			putText(frameDest, opTimeForImg, cvPoint(8, 60), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);

            imshow("OpenCL Demo: Sobel Filter", frameDest);  // Show image.
        }
        else
        {
            Mat outFrame;

			/********** Actual operations w/ time calc **********/
			opTimePre = cv::getTickCount();
			Sobel(grayFrame, outFrame, grayFrame.depth(), 1, 1, 5, 1, 0.0, BORDER_DEFAULT);
			opTimePost = cv::getTickCount();

			fpsForImg = calcImgFPS();  // Function obtains FPS, returns string.
			opTimeForImg = averageOpTime(opTimePre, opTimePost);  // Function averages Op times, returns string.
			/****************************************************/

			// Place black background, CPU/GPU, FPS, and Operation time on frame:
			Mat textBGROI = outFrame(cv::Rect(0,0,200,75)); 
			cv::addWeighted(textBGROI,0.0,textBG,1.0,0.,textBGROI);
			putText(outFrame, "CPU", cvPoint(8, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
            putText(outFrame, fpsForImg, cvPoint(8, 40), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
			putText(outFrame, opTimeForImg, cvPoint(8, 60), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);

            imshow("OpenCL Demo: Sobel Filter", outFrame);  // Show image.
        }
        

		// Switches b/w CPU and GPU (OpenCL) if 'o' pressed, exits program if 'q' pressed.
        k = cvWaitKey(1);
        if (k == 'q')
        {
            cap.release();
			cv::destroyAllWindows();
            return 0;
        }
        if (k == 'o')
        {
            OPENOCL = !OPENOCL;
        }
    
	}  // End video capture for(;;).
}