#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

//For 2D feature dectectors
#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace std;

void readme();

/** @function main */
int main( int argc, char** argv )
{
	// This is a sample code to see if the opencv libs are correctly include in the project
  //cv::initModule_nonfree();
  //if( argc != 3 )
  //{ readme(); return -1; }

  //
  //Mat img_1 = imread( argv[1], IMREAD_GRAYSCALE );
  //Mat img_2 = imread( argv[2], IMREAD_GRAYSCALE );

  //if( !img_1.data || !img_2.data )
  //{ std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  ////-- Step 1: Detect the keypoints using SURF Detector
  //int minHessian = 200;

  //SurfFeatureDetector detector( minHessian );

  //std::vector<KeyPoint> keypoints_1, keypoints_2;

  //detector.detect( img_1, keypoints_1 );
  //detector.detect( img_2, keypoints_2 );

  ////-- Draw keypoints
  //Mat img_keypoints_1; Mat img_keypoints_2;

  //drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  //drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );


  ////show original images
  ////imshow("Keypoints 1", img_1 );

  ////-- Show detected (drawn) keypoints
  //imshow("Keypoints 1", img_keypoints_1 );
  //imshow("Keypoints 2", img_keypoints_2 );

  ////-- Step 2: Calculate descriptors (feature vectors)
  //SurfDescriptorExtractor extractor;

  //Mat descriptors_1, descriptors_2;

  //extractor.compute( img_1, keypoints_1, descriptors_1 );
  //extractor.compute( img_2, keypoints_2, descriptors_2 );

  // //-- Step 3: Matching descriptor vectors using FLANN matcher
  //FlannBasedMatcher matcher;
  //std::vector< DMatch > matches;
  //matcher.match( descriptors_1, descriptors_2, matches );

  //double max_dist = 0; double min_dist = 100;

  ////-- Quick calculation of max and min distances between keypoints
  //for( int i = 0; i < descriptors_1.rows; i++ )
  //{ double dist = matches[i].distance;
  //  if( dist < min_dist ) min_dist = dist;
  //  if( dist > max_dist ) max_dist = dist;
  //}

  //printf("-- Max dist : %f \n", max_dist );
  //printf("-- Min dist : %f \n", min_dist );

  ////-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  ////-- PS.- radiusMatch can also be used here.
  //std::vector< DMatch > good_matches;

  //int c;
  //int times = 0; 

  //while(1){

  //for( int i = 0; i < descriptors_1.rows; i++ )
  //{ if( matches[i].distance <= times*min_dist )
  //  { good_matches.push_back( matches[i]); }
  //}

  ////-- Draw only "good" matches
  //Mat img_matches;
  //drawMatches( img_1, keypoints_1, img_2, keypoints_2,
  //             good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
  //             vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  ////-- Show detected matches
  //imshow( "Good Matches", img_matches );

  //for( int i = 0; i < good_matches.size(); i++ )
  //{ printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }

  //c = waitKey(); 
  //if(c==2555904){
	 // times = times+1;
  //}
  //if(c==2424832){
	 // times = std::max(0,times-1);
  //}
  //good_matches.clear();
  //std::cout<<"C: "<<c<<" times: "<<times<<std::endl;
  ////waitKey(0);
  //} 
  //return 0;
}

  ///** @function readme */
  //void readme()
  //{ std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }