//#include <stdio.h>
//#include <iostream>
//#include "opencv2/core/core.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/highgui/highgui.hpp"
//
////For 2D feature dectectors
//#include <opencv2/nonfree/features2d.hpp>
//#include "opencv2/nonfree/nonfree.hpp"
//
//using namespace cv;
//using namespace std;
//
//void readme();
//
////** @function main */
//int main( int argc, char** argv )
//{
//	 //This is a sample code to see if the opencv libs are correctly include in the project
//  cv::initModule_nonfree();
//  if( argc != 3 )
//  { readme(); return -1; }
//
//  
//  Mat img_1 = imread( argv[1], IMREAD_GRAYSCALE );
//  Mat img_2 = imread( argv[2], IMREAD_GRAYSCALE );
//
//  if( !img_1.data || !img_2.data )
//  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }
//
//  //-- Step 1: Detect the keypoints using SURF Detector
//  int minHessian = 200;
//
//  SurfFeatureDetector detector( minHessian );
//
//  std::vector<KeyPoint> keypoints_1, keypoints_2;
//
//  detector.detect( img_1, keypoints_1 );
//  detector.detect( img_2, keypoints_2 );
//
//  //-- Draw keypoints
//  Mat img_keypoints_1; Mat img_keypoints_2;
//
//  drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
//  drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
//
//
//  //show original images
//  //imshow("Keypoints 1", img_1 );
//
//  //-- Show detected (drawn) keypoints
//  imshow("Keypoints 1", img_keypoints_1 );
//  imshow("Keypoints 2", img_keypoints_2 );
//
//  //-- Step 2: Calculate descriptors (feature vectors)
//  SurfDescriptorExtractor extractor;
//
//  Mat descriptors_1, descriptors_2;
//
//  extractor.compute( img_1, keypoints_1, descriptors_1 );
//  extractor.compute( img_2, keypoints_2, descriptors_2 );
//
//   //-- Step 3: Matching descriptor vectors using FLANN matcher
//  FlannBasedMatcher matcher;
//  std::vector< DMatch > matches;
//  matcher.match( descriptors_1, descriptors_2, matches );
//
//  double max_dist = 0; double min_dist = 100;
//
//  //-- Quick calculation of max and min distances between keypoints
//  for( int i = 0; i < descriptors_1.rows; i++ )
//  { double dist = matches[i].distance;
//    if( dist < min_dist ) min_dist = dist;
//    if( dist > max_dist ) max_dist = dist;
//  }
//
//  printf("-- Max dist : %f \n", max_dist );
//  printf("-- Min dist : %f \n", min_dist );
//
//  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
//  //-- PS.- radiusMatch can also be used here.
//  std::vector< DMatch > good_matches;
//
//  int c;
//  int times = 3; 
//
//  while(1){
//
//  for( int i = 0; i < descriptors_1.rows; i++ )
//  { if( matches[i].distance <= times*min_dist )
//    { good_matches.push_back( matches[i]); }
//  }
//
//  //-- Draw only "good" matches
//  Mat img_matches;
//  drawMatches( img_1, keypoints_1, img_2, keypoints_2,
//               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
//               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//
//  //-- Show detected matches
//  imshow( "Good Matches", img_matches );
//
//  for( int i = 0; i < good_matches.size(); i++ )
//  { printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }
//
//  c = waitKey(); 
//  if(c==61){
//	  times = times+1;
//  }
//  if(c==45){
//	  times = std::max(0,times-1);
//  }
//  good_matches.clear();
//  std::cout<<"C: "<<c<<" times: "<<times<<std::endl;
//  //waitKey(0);
//  } 
//  return 0;
//}
//
//  ///** @function readme */
//  void readme()
//  { std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }


/*****************************************************************************************************************************************/
/**
* @file MatchTemplate_Demo.cpp
* @brief Sample code to use the function MatchTemplate
* @author OpenCV team
*/
//#include <iostream>
//#include <stdio.h>
//
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/imgproc/imgproc_c.h"
//#include <opencv2/features2d/features2d.hpp>
//
//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/nonfree/nonfree.hpp>

//using namespace std;
//using namespace cv;
//
///// Global Variables
//Mat img; Mat templ; Mat result;
//const char* image_window = "Source Image";
//const char* result_window = "Result window";
//const char* templ_window = "Template";
//const char* original = "Original";
//
//int match_method;
//int max_Trackbar = 5;
//
///// Function Headers
//void MatchingMethod( int, void* );
//
//////**
//////* @function main
//////*/
//int main( int, char** argv )
//{
//	cv::initModule_nonfree();
//	/// Load image and template
//	//img = imread( argv[1], 1 );
//	templ = imread( argv[1], 1 );
//
//	if(!templ.data){
//		std::cout<<"--- Error no image found at specified address"<<std::endl; 
//		return -1;
//	}
//	/// Create windows
//	//namedWindow(original,WINDOW_AUTOSIZE);
//	namedWindow(templ_window,WINDOW_AUTOSIZE);
//	namedWindow( image_window, WINDOW_AUTOSIZE );
//	namedWindow( result_window, WINDOW_AUTOSIZE );
//
//
//	CvMemStorage* storage = cvCreateMemStorage(0);
//	//CvCapture* capture = CvCaptureFromCAM(-1); 
//	VideoCapture capture(0);
//
//	int key=0;
//	while( key != 'q' ){
//
//		Mat frame;
//		capture >> frame;
//		
//		img = frame;
//		/// Create Trackbar
//		const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
//		createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );
//		img = Mat(frame); 
//		
//		//imshow(original,img);
//		imshow(templ_window,templ);
//		MatchingMethod( 0, 0 );
//
//		key = waitKey(10);
//		if(key == 27){
//            break; // if ESC, break and quit
//		}
//	}
//
//	return 0;
//}
//
//////**
//////* @function MatchingMethod
//////* @brief Trackbar callback
//////*/
//void MatchingMethod( int, void* )
//{
//
//	//Threshold value 
//	double thresholdMatchMin = 0;
//	double thresholdMatchMax = 0.9;
//
//	/// Source image to display
//	Mat img_display;
//	img.copyTo( img_display );
//
//	/// Create the result matrix
//	int result_cols =  img.cols - templ.cols + 1;
//	int result_rows = img.rows - templ.rows + 1;
//
//	result.create( result_cols, result_rows, CV_32FC1 );
//
//	/// Do the Matching and Normalize
//	matchTemplate( img, templ, result, match_method );
//	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
//
//	/// Localizing the best match with minMaxLoc
//	double minVal; double maxVal; Point minLoc; Point maxLoc;
//	Point matchLoc;
//
//	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
//
//
//	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
//	if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
//	{ matchLoc = minLoc; }
//	else
//	{ matchLoc = maxLoc; }
//
//
//	//We then compare the min val to Threshold for SQDIFF
//
//	/// Show me what you got
//	if((match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) && minVal < thresholdMatchMin){
//		rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
//		rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
//	}
//	else if(!(match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) && maxVal > thresholdMatchMax){
//		rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(255,0,0), 2, 8, 0 );
//		rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(100), 2, 8, 0 );
//	}
//
//	imshow( image_window, img_display );
//	imshow( result_window, result );
//
//	return;
//}
