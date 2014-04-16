/***************************************************************************************/
//
//	Author:		Biruh Tesfaye & Rich Shoff
//	Date:		4/12/2014
//
//	Purpose:	This file contains the Face Detect OCL benchmark test function
//
//	Usage:		Press 'o' to switch between OCL and CPU, press 'q' to return to main menu.
//
/***************************************************************************************/

#include "perf_precomp.hpp"
#include <opencv2/ocl/ocl.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "h_oclPerfStats_ShoffTesfaye.h"
#include "h_BM_Functions_ShoffTesfaye.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


// Function Headers:
void detectAndDisplayCPU(Mat &frame);
void detectAndDisplayGPU(Mat &frame);


// Import Cascades.  NOTE: CASCADE XMLs MUST EXIST IN SAME DIRECTORY AS CPP FILE!!!
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye.xml";

// Global variables:
CascadeClassifier face_cascadeCPU;
CascadeClassifier eyes_cascadeCPU;
ocl::OclCascadeClassifier face_cascadeGPU;
ocl::OclCascadeClassifier eyes_cascadeGPU;
RNG rng(12345);


// ****************************** - Main Function - ******************************
 int faceDetect_ShoffTesfaye()
 {
	/********** - Main-wide Variable Declarations - **********/
	int64 opTimePre = 0;
	int64 opTimePost = 0;
	string fpsForImg = "";
	string opTimeForImg = "";

    Mat frame;

	Mat textBG = Mat::zeros(75, 200, CV_8UC3);

    bool OPENOCL = false;

    char k;
	/************************************************************/

    // Verify the Cascades have loaded:
    if(!face_cascadeCPU.load(face_cascade_name)) { cout << "--(!)Error loading CPU face cascade\n"; return -1; };
    if(!eyes_cascadeCPU.load(eyes_cascade_name)) { cout << "--(!)Error loading CPU eye cascade\n"; return -1; };
	if(!face_cascadeGPU.load(face_cascade_name)) { cout << "--(!)Error loading GPU face cascade\n"; return -1; };
    if(!eyes_cascadeGPU.load(eyes_cascade_name)) { cout << "--(!)Error loading GPU eye cascade\n"; return -1; };

    //Initialize video input:
    VideoCapture cap(0);  // Open the default camera.
    if (!cap.isOpened())  // check if camera opened.
        return -1;

	namedWindow("OpenCL Demo: Face Detect", 1);  // Create display window.

	for (;;)
	{
		cap >> frame;  // Get a new frame from camera.


		// IF Statement switches between CPU and GPU (OpenCL) if 'o' is pressed!
		if (OPENOCL)
        {
			/********** Actual operations w/ time calc **********/
			opTimePre = cv::getTickCount();
			detectAndDisplayGPU(frame);  // Detect and display face & eyes (OpenCL):
			opTimePost = cv::getTickCount();

			fpsForImg = calcImgFPS();  // Function obtains FPS, returns string.
			opTimeForImg = averageOpTime(opTimePre, opTimePost);  // Function averages Op times, returns string.
			/****************************************************/

			// Place CPU/GPU, FPS, and Operation time on frame:
			Mat textBGROI = frame(cv::Rect(0,0,200,75));
			cv::addWeighted(textBGROI,0.0,textBG,1.0,0.,textBGROI);
            putText(frame, "GPU (OpenCL)", cvPoint(8, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
            putText(frame, fpsForImg, cvPoint(8, 40), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
			putText(frame, opTimeForImg, cvPoint(8, 60), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);

			imshow("OpenCL Demo: Face Detect", frame);
		}
		else
		{
			/********** Actual operations w/ time calc **********/
			opTimePre = cv::getTickCount();
			detectAndDisplayCPU(frame);  // Detect and display face & eyes:
			opTimePost = cv::getTickCount();

			fpsForImg = calcImgFPS();  // Function obtains FPS, returns string.
			opTimeForImg = averageOpTime(opTimePre, opTimePost);  // Function averages Op times, returns string.
			/****************************************************/

			// Place CPU/GPU, FPS, and Operation time on frame:
			Mat textBGROI = frame(cv::Rect(0,0,200,75));
			cv::addWeighted(textBGROI,0.0,textBG,1.0,0.,textBGROI);
            putText(frame, "CPU", cvPoint(8, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
            putText(frame, fpsForImg, cvPoint(8, 40), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);
			putText(frame, opTimeForImg, cvPoint(8, 60), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 250, 250), 1, CV_AA);

			imshow("OpenCL Demo: Face Detect", frame);
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

    }  // End video capture for loop.
}



/***** - function detectAndDisplayCPU - *****/
void detectAndDisplayCPU(Mat &frame)
{
    std::vector<Rect> faces;
    Mat grayFrame;

    cvtColor(frame, grayFrame, CV_BGR2GRAY);

    equalizeHist(grayFrame, grayFrame);


    // Detect faces
    face_cascadeCPU.detectMultiScale(grayFrame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

	// Draw Ellipses around faces:
    for(size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
        ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		Mat faceROI = grayFrame(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascadeCPU.detectMultiScale(faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30));

		// Draw circles around eyes:
		for(size_t j = 0; j < eyes.size(); j++)
		{
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
	}
 }


/***** - function detectAndDisplayGPU - *****/
void detectAndDisplayGPU(Mat &frame)
{
	ocl::oclMat oclFrame(frame);  // OVERHEAD 1: Convert frame to OCL frame for operation.
    std::vector<Rect> faces;
    ocl::oclMat grayOCLFrame;

    ocl::cvtColor(oclFrame, grayOCLFrame, CV_BGR2GRAY);

    ocl::equalizeHist(grayOCLFrame, grayOCLFrame);


    // Detect faces (Done with GPU!)
    face_cascadeGPU.detectMultiScale(grayOCLFrame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

	// Draw Ellipses around faces:
    for(size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
        ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		ocl::oclMat faceROI = grayOCLFrame(faces[i]);
		std::vector<Rect> eyes;

		// In each face, detect eyes (Done with GPU!)
		eyes_cascadeGPU.detectMultiScale(faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30));

		// Draw circles around eyes:
		for(size_t j = 0; j < eyes.size(); j++)
		{
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
	}
 }