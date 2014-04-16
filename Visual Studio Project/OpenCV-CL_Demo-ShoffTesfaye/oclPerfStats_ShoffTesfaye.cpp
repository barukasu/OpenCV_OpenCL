/***************************************************************************************/
//
//	Author:		Biruh Tesfaye & Rich Shoff
//	Date:		4/12/2014
//
//	Purpose:	This file contains the functions to calculate FPS & Op execution time.
//
/***************************************************************************************/

//***** - Include Files - *****
#include "perf_precomp.hpp"
#include <opencv2/ocl/ocl.hpp>
#include "h_BM_Functions_ShoffTesfaye.h"
#include "h_oclPerfStats_ShoffTesfaye.h"

using namespace cv;  // OpenCL defined under this namespace!!!
using namespace std;


//**************************************** - Global Variables - ****************************************

//For Frames Per Second (FPS) calculator Function:
double frameNthOutFPS = 5;  // Frame interval to display FPS.
double frameCountFPS = 0;  // Current frame.
double timePre = 0;
double timePost = 0;
double fps = 0;
double fpsPeriod = 0;
double fpsAverage = 0;
string fpsAverageStr = "Calculating...";
int fpsAverageInt;
double fpsTotal = 0;

//For Execution time of specific operation averaging function:
int64 frameNthOutOpTime = 5;  // Frame interval to display Op time.
int64 frameCountOpTime = 0;
//int64 opTimePre = 0;  // Calculate before operation & func call.
//int64 opTimePost = 0;  // Calculate after operation & func call.
int64 opTime = 0;
int64 opTimeAvg = 0;
int64 opTimeTot = 0;
string opTimeAvgStr = "Calculating...";


//**************************************** - Functions - ****************************************

// ***** - Calculate Frames Per Second for image display - *****
string calcImgFPS()
{
    timePre = cv::getTickCount();
    fpsPeriod = ((timePre - timePost) / cv::getTickFrequency());
    fps = 1 / fpsPeriod;
    timePost = cv::getTickCount();

    if (frameCountFPS < frameNthOutFPS)
    {
        frameCountFPS++;
        fpsTotal += fps;
        return fpsAverageStr;
    }
    else
    {
        frameCountFPS++;
        fpsTotal += fps;
        fpsAverage = fpsTotal / frameCountFPS;

        //Convert FPS Avg to int, then string for display:
        fpsAverageInt = int(fpsAverage);
        string fpsAverageStrMid = to_string(fpsAverageInt);
        fpsAverageStr = "FPS:  " + fpsAverageStrMid;

        frameCountFPS = 0;
        fpsTotal = 0;
		frameNthOutFPS = fpsAverage;  // Forces output to update every 1 second on video.

        return fpsAverageStr;
    }
}

// ***** - Averages out the op time, returns as string - *****
string averageOpTime(int64 opTimePre, int64 opTimePost)
{
	if (frameCountOpTime < frameNthOutOpTime)
    {
		frameCountOpTime++;
		opTime = ((opTimePost - opTimePre) / cv::getTickFrequency()) * 1000000;  // in microseconds (us).
        opTimeTot += opTime;
        return opTimeAvgStr;
    }
    else
    {
		frameCountOpTime++;
		opTime = ((opTimePost - opTimePre) / cv::getTickFrequency()) * 1000000;  // in microseconds (us).
        opTimeTot += opTime;
        opTimeAvg = (opTimeTot / frameCountOpTime);  // in microseconds (us).

        //Convert Op time average to string for display:
        opTimeAvgStr = "Op time:  " + to_string(opTimeAvg) + "us";

		frameCountOpTime = 0;
        opTimeTot = 0;
		frameNthOutOpTime = frameNthOutFPS;  // Forces output to update every 1 second on ONLY IF FPS FUNCTION USED.

        return opTimeAvgStr;
    }
}