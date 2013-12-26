/**
* @file MatchTemplate_Demo.cpp
* @brief Sample code to use the function MatchTemplate
* @author OpenCV team
*/
#include <iostream>
#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";
const char* templ_window = "Template";
const char* original = "Original";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

/**
* @function main
*/
int main( int, char** argv )
{
	cv::initModule_nonfree();
	/// Load image and template
	//img = imread( argv[1], 1 );
	templ = imread( argv[1], 1 );

	if(!templ.data){
		std::cout<<"--- Error no image found at specified address"<<std::endl; 
		return -1;
	}
	/// Create windows
	//namedWindow(original,WINDOW_AUTOSIZE);
	namedWindow(templ_window,WINDOW_AUTOSIZE);
	namedWindow( image_window, WINDOW_AUTOSIZE );
	namedWindow( result_window, WINDOW_AUTOSIZE );


	CvMemStorage* storage = cvCreateMemStorage(0);
	CvCapture* capture = cvCaptureFromCAM(-1); 

	int key=0;
	while( key != 'q' ){

		IplImage* frame = cvQueryFrame(capture);

		if(!frame){
			break;
		}
		img = cvCreateMat(frame->height, frame->width, CV_32F);
		/// Create Trackbar
		const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
		createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );
		img = Mat(frame); 
		
		//imshow(original,img);
		imshow(templ_window,templ);
		MatchingMethod( 0, 0 );

		key = waitKey(10);
		if(key == 27){
            break; // if ESC, break and quit
		}
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow(original);
	return 0;
}

/**
* @function MatchingMethod
* @brief Trackbar callback
*/
void MatchingMethod( int, void* )
{

	//Threshold value 
	double thresholdMatchMin = 0;
	double thresholdMatchMax = 0.9;

	/// Source image to display
	Mat img_display;
	img.copyTo( img_display );

	/// Create the result matrix
	int result_cols =  img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create( result_cols, result_rows, CV_32FC1 );

	/// Do the Matching and Normalize
	matchTemplate( img, templ, result, match_method );
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );


	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
	{ matchLoc = minLoc; }
	else
	{ matchLoc = maxLoc; }


	//We then compare the min val to Threshold for SQDIFF

	/// Show me what you got
	if((match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) && minVal < thresholdMatchMin){
		rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
		rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
	}
	else if(!(match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) && maxVal > thresholdMatchMax){
		rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(255,0,0), 2, 8, 0 );
		rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(100), 2, 8, 0 );
	}

	imshow( image_window, img_display );
	imshow( result_window, result );

	return;
}
