/*M//////////////////////////////////////////////////////////////////////////////////////
//
//		Written By:		Biruh Tesfaye & Rich Shoff
//		Date:			04/05/2014
//		
//		Program:		main program for OpenCL Benchmark testing. Part of OpenCL presentation.
//
//		Notes:			-Windows Environment Variable OPENCV_OPENCL_DEVICE was set to AMD:GPU:0
//							Rich's desktop.
//
//M*/////////////////////////////////////////////////////////////////////////////////////


#include "perf_precomp.hpp"
#include <opencv2/ocl/ocl.hpp>
#include "h_BM_Functions_ShoffTesfaye.h"

using namespace cv;  // OpenCL defined under this namespace!!!
using namespace std;


char userinp;  // User input from menu.


// ******************** - Main Menu Function - ********************
char mainMenu()
{
	// ********** - Display Menu - **********
	cout << "********** - OpenCL Benchmark Program: Main Menu - **********" << endl << endl;
    cout << "Choose a menu option by striking the corresponding key, then press Enter:" << endl << endl;
	cout << "    b  -  Bilateral Filter" << endl;
    cout << "    c  -  Canny" << endl;
	cout << "    f  -  Face Detector" << endl;
	cout << "    s  -  Sobel Filter" << endl;
	cout << "    x  -  -- Exit program --" << endl << endl << endl;
	cout << "Selection: ";
	cin >> userinp;

	return userinp;
}


// **************************************** - Main - ****************************************

int main()
{
	mainMenu();

	for(;;)
	{
		// Switches between programs, quits if 'q' is pressed.
        if (userinp == 'x')
        {
			cout << endl << "Exiting program..." << endl;
            return 0;
        }
		else if (userinp == 'c')
		{
			//Menu for Canny demo:
			system("cls");
			cout << "Canny demo: Please strike a key:" << endl << endl;
			cout << "o - Switch between CPU and OpenCL" << endl;
			cout << "q - Quit back to the main menu" << endl;

			cannyOCL_ShoffTesfaye();

			system("cls");
			mainMenu();
		}
		else if (userinp == 'b')
		{
			// Menu for Bilateral Filter demo:
			system("cls");
			cout << "Bilateral Filter demo: Please strike a key:" << endl << endl;
			cout << "o - Switch between CPU and OpenCL" << endl;
			cout << "q - Quit back to the main menu" << endl;

			bilateralOCL_ShoffTesfaye();

			system("cls");
			mainMenu();
		}
		else if (userinp == 's')
		{
			// Menu for Face Detect demo:
			system("cls");
			cout << "Sobel Filter demo: Please strike a key:" << endl << endl;
			cout << "o - Switch between CPU and OpenCL" << endl;
			cout << "q - Quit back to the main menu" << endl;

			sobelOCL_ShoffTesfaye();

			system("cls");
			mainMenu();
		}
		else if (userinp == 'f')
		{
			// Menu for Face Detect demo:
			system("cls");
			cout << "Face Detector demo: Please strike a key:" << endl << endl;
			cout << "o - Switch between CPU and OpenCL" << endl;
			cout << "q - Quit back to the main menu" << endl;

			faceDetect_ShoffTesfaye();

			system("cls");
			mainMenu();
		}
		else  // Default menu state.
		{
			// Belittling notice:
			cout << endl << endl << "Nice try...pick an actual choice this time." << endl << endl;
			system("PAUSE");

			system("cls");
			mainMenu();
		}

    
	}  // End Program loop.
}  // End main().