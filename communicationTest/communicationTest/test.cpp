 #include "MyThread.h"

#include <iostream>
#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

MyThread myThread[CAM_NUM] = {PORT1};

void init(void)
{
	//Start the thread which sends and receives images
	myThread[0].beginSmartThread();

	//Wait until smartphone is connected
	while(!myThread[0].getCameraFlg())	
		waitKey(1);
}

int main(void)
{
	cv::initModule_nonfree();
	init();

	while(waitKey(1) != 'q'){
		Mat inputImage;

		//Get smartphone image
		myThread->getSmartImage().copyTo(inputImage);	

		//////////////////////Processing//////////////////////
		int key =0;
		imshow("Input image", inputImage);
		Mat outputImage;
		cvtColor(inputImage, outputImage, CV_BGR2GRAY);
		//cvtColor(outputImage, outputImage, CV_GRAY2BGR);

		Mat targetImage = imread("picture1.jpg",CV_LOAD_IMAGE_GRAYSCALE);

		if(!targetImage.data){
			std::cout<<"--- Error no image found at specified address"<<std::endl; 
			return -1;
		}


		//Define sequence for storing surf keypoints and descriptors
		std::vector<KeyPoint> imageKeypoints, targetImageKeypoints;
		Mat imageDescriptors, targetImageDescriptors;
		int i;

		//Extract SURF points by initializing parameters
		int minHessian = 500;
		SurfFeatureDetector detector(minHessian);

		//cvSURFParams params = cvSURFParams(500, 1);
		detector.detect(outputImage,imageKeypoints); 
		detector.detect(targetImage,targetImageKeypoints);

		SurfDescriptorExtractor extractor; 
		extractor.compute(outputImage,imageKeypoints,imageDescriptors);
		extractor.compute(targetImage,targetImageKeypoints,targetImageDescriptors);

		//cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );

		//std::cout<<"Image Descriptors: "<<imageDescriptors;

		//draw the keypoints on the captured frame
		Mat imageKeypoints1,targetImageKeypoints1;
		drawKeypoints(outputImage,imageKeypoints,imageKeypoints1,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
		drawKeypoints(targetImage,targetImageKeypoints,targetImageKeypoints1,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
		//imshow("Image",imageKeypoints1);
		imshow("TargetImage",targetImageKeypoints1);



		//Matching descriptor vectors using FLANN matcher
		FlannBasedMatcher matcher;
		std::vector<DMatch> matches;
		matcher.match(imageDescriptors,targetImageDescriptors, matches);

		double max_dist = 0 ; double min_dist = 100;

		//Quickly finding max and mix distances for matching
		for(i=0;i<imageDescriptors.rows;i++){
			double dist = matches[i].distance;
			if(dist<min_dist) min_dist = dist;
			if(dist>max_dist) max_dist = dist;
		}

		//keeping only good matches
		std::vector<DMatch> good_matches;

		int times = 2;

		//while(1){

		for( int i = 0; i < imageDescriptors.rows; i++ )
		{ if( matches[i].distance <= times*min_dist )
		{ good_matches.push_back( matches[i]); }
		}

		//-- Draw only "good" matches
		Mat img_matches;
		drawMatches( outputImage, imageKeypoints, targetImage, targetImageKeypoints,
			good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

		//-- Show detected matches
		imshow( "Image", img_matches );

		for( int i = 0; i < good_matches.size(); i++ )
		{ printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }

		/*int c = cvWaitKey();
		if(c==2555904){
			times = times+1;
		}
		if(c==2424832){
			times = std::max(0,times-1);
		}*/
		good_matches.clear(); 

		cvWaitKey(5);
		key = cvWaitKey(10);
		if(key==27){
		break;
		}

		imshow("Output image", outputImage);
		////////////////////////////////////////////////

		//Send image to smartphone
		myThread->inputResImage(outputImage);			
	}

	//Close
	MyThread::closeHandle();	

	return 0;
}