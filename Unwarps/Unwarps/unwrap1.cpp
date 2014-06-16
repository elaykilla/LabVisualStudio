//// This program unwraps an omni-image, produced by an omnidirectional camera.
//// The program is intended, in default mode, to function properly for the lens
//// produced by 0-360.com. Some parameters can be set by the user, which allows
//// for the use of different image sources as well.
////
//// This source code is based on original source code in Matlab, which probably
//// shows. 
//
//#include <stdio.h>
//#include <iostream>
//#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc_c.h>
//
//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//using namespace std;
//
//const double M_PI = 3.1415926535897932384626;
//
//// Some structs for internal use
//struct OmniParams
//{
//	int centerX;
//	int centerY;
//	int innerR; 
//	int outerR; 
//};
//struct OmniParamsd
//{
//	double centerX;
//	double centerY;
//	double innerR; 
//	double outerR; 
//};
//
//void showGreeting()
//{
//	cout << "-=================================-" << endl;
//	cout << "    OMNI-IMAGE UNWRAPPER 1.02      " << endl;
//	cout << "-=================================-" << endl;
//	cout << "     (C) Christoffer Wahlgren      " << endl;
//	cout << "-=================================-" << endl;
//	cout << " Parts of this code are based on..." << endl;
//	cout << " ...OpenCV.                        " << endl;
//	cout << "-=================================-" << endl;
//}
//
//void showInstructions()
//{
//	cout << "USAGE: UNWRAP infile outfile [-q]                     " << endl;
//	cout << "-innerVALUE -outerVALUE -centerxVALUE -centeryVALUE   "   << endl;
//	cout << endl;
//	cout << "infile is the input file that should be unwrapped.      " << endl;
//	cout << "outfile is the output file that should be written.      " << endl;
//	cout << "infile and outfile can have various image formats       " << endl;
//	cout << "(OpenCV are used for saving and loading images.)        " << endl;
//	cout << endl;
//	cout << "The parameter -q is used to suppress screen output from " << endl;
//	cout << "the program.                                            " << endl;
//	cout << endl;
//	cout << "This program assumes that the center point, inner and   " << endl;
//	cout << "outer radius of the omni-image (in pixels) are given,   " << endl;
//	cout << "perhaps by using a program such as omniautodetect.      " << endl;
//	cout << endl;
//	cout << "EXAMPLE USAGE:" << endl;
//	cout << "UNWRAP test_omni.jpg unwrapped.gif -inner40 -outer200 -centerx250 -centery300" << endl;
//	cout << endl;
//}
//
//// The result codes are used to signal the outcome of the program to the operating system
//enum ResultCodes
//{
//	UW_NO_ERROR = 0,					// As the name implies, success!
//	UW_INPUT_FILE_NOT_FOUND ,  		// Input file was not found
//	UW_OUTPUT_FILE_CANNOT_CREATE,		// Output file could not be created, write protected?
//	UW_DATA_FILE_CANNOT_CREATE,
//	UW_VALUE_ERROR						// The calculation halted with an error
//};
//
//int main(int argc, char* argv[])
//{
//	cv::initModule_nonfree();
//	showInstructions();
//	bool quietMode = false;
//	ResultCodes result = UW_NO_ERROR;
//	float aspectRatio = 0.29;
//	int midx = 0;
//	int midy = 0;
//	int inner = 0;
//	int outer = 0;
//
//
//	if (argc < 7)
//	{
//		showGreeting();
//		showInstructions();
//	}
//	else
//	{
//		for (int i = 3; i < argc; i++)
//		{
//			if (stricmp("-q",argv[i])==0)
//				quietMode = true;
//			else if (strnicmp("-centerx",argv[i],8)==0)
//			{
//				float h;
//				int scanresult = sscanf(argv[i],"-centerx%f",&h);
//				if (scanresult == 1)
//				{
//					midx = h;
//				}
//				else
//				{
//					cerr << "Warning: Could not read center position X from command line." << endl;
//				}
//			}
//			else if (strnicmp("-centery",argv[i],8)==0)
//			{
//				float h;
//				int scanresult = sscanf(argv[i],"-centery%f",&h);
//				if (scanresult == 1)
//				{
//					midy = h;
//				}
//				else
//				{
//					cerr << "Warning: Could not read center position Y from command line." << endl;
//				}
//			}
//			else if (strnicmp("-inner",argv[i],6)==0)
//			{
//				float h;
//				int scanresult = sscanf(argv[i],"-inner%f",&h);
//				if (scanresult == 1)
//				{
//					inner = h;
//				}
//				else
//				{
//					cerr << "Warning: Could not read inner radius from command line." << endl;
//				}
//			}
//			else if (strnicmp("-outer",argv[i],6)==0)
//			{
//				float h;
//				int scanresult = sscanf(argv[i],"-outer%f",&h);
//				if (scanresult == 1)
//				{
//					outer = h;
//				}
//				else
//				{
//					cerr << "Warning: Could not read outer radius from commmand line." << endl;
//				}
//			}
//		}
//		if (!quietMode)
//			showGreeting();
//
//		// Load image
//		if (!quietMode)	cout << "Loading image " << argv[1] << endl;
//		IplImage* image = cvLoadImage(argv[1]);
//
//		if (image == NULL)
//		{
//			result = UW_INPUT_FILE_NOT_FOUND;
//			cerr << "Error: Cannot open file " << argv[1] << endl;
//		}
//		if (!result)
//		{
//			// Attempt to detect center point, inner radius and outer radius
//			OmniParams omniParams;
//			omniParams.centerX = midx;
//			omniParams.centerY = midy;
//			omniParams.innerR = inner;
//			omniParams.outerR = outer;
//
//			// Unwrap image
//			// This is a very basic polar-to-cartesian conversion.
//			if (!quietMode) cout << "Unwrapping" << endl;
//			omniParams.innerR +=3;
//			omniParams.outerR -=3;
//			double totx = omniParams.outerR * 2 * M_PI;
//			double toty = (omniParams.outerR-omniParams.innerR) + 1;
//
//			IplImage* imgUW = cvCreateImage(cvSize(ceil(totx),ceil(toty)),8,3);
//			int i, j = 0;
//			for (double angle = 0; angle < 2*M_PI; angle += 2*M_PI/totx)
//			{
//				i = floor(toty)-1;
//				for (int radius = omniParams.innerR; radius <= omniParams.outerR; radius++)
//				{
//					int ii = radius * cos(angle-M_PI/2)+(double)omniParams.centerY;
//					int jj = radius * sin(angle-M_PI/2)+(double)omniParams.centerX;
//					if (ii >= 0 && ii <= image->height && jj >=0 && jj <= image->width)
//					{
//						((uchar *)(imgUW->imageData + i*imgUW->widthStep))[j*3+0] = 
//							((uchar *)(image->imageData + ii*image->widthStep))[jj*3+0];
//						((uchar *)(imgUW->imageData + i*imgUW->widthStep))[j*3+1] = 
//							((uchar *)(image->imageData + ii*image->widthStep))[jj*3+1];
//						((uchar *)(imgUW->imageData + i*imgUW->widthStep))[j*3+2] = 
//							((uchar *)(image->imageData + ii*image->widthStep))[jj*3+2];
//					}
//					else
//					{
//						((uchar *)(imgUW->imageData + i*imgUW->widthStep))[j*3+0] = 0;
//						((uchar *)(imgUW->imageData + i*imgUW->widthStep))[j*3+1] = 0;
//						((uchar *)(imgUW->imageData + i*imgUW->widthStep))[j*3+2] = 0;
//					}
//					i--;
//				}
//				j++;
//			}
//
//			// Rescale image using the built-in functionality in OpenCV
//			if (!quietMode) cout << "Rescaling" << endl;
//			int newHeight = imgUW->height;
//			int newWidth = double(newHeight) / aspectRatio;
//
//			IplImage* imgResized = cvCreateImage(cvSize(newWidth,newHeight),8,3);
//			cvResize(imgUW,imgResized);
//			if (!quietMode) cout << "Writing image" << endl;
//
//			if (cvSaveImage(argv[2],imgResized)==0)
//			{
//				result = UW_OUTPUT_FILE_CANNOT_CREATE;
//				cerr << "Error: Cannot create output file " << argv[2] << endl;
//			}
//
//			// Release image
//			cvReleaseImage(&imgResized);
//			cvReleaseImage(&image);
//			cvReleaseImage(&imgUW);
//		}
//	}
//	if (!quietMode && result == 0) cout << "Done." << endl;
//	return result;
//}
