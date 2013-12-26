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
		imshow("Input image", inputImage);
		Mat outputImage;
		cvtColor(inputImage, outputImage, CV_BGR2GRAY);
		cvtColor(outputImage, outputImage, CV_GRAY2BGR);
		imshow("Output image", outputImage);
		////////////////////////////////////////////////

		//Send image to smartphone
		myThread->inputResImage(outputImage);			
	}

	//Close
	MyThread::closeHandle();	

	return 0;
}