//This define is used to counter the windows.h usage of min and max
#define NOMINMAX 
////Include headers
//#include "Pattern.hpp"
//#include "PatternDetector.hpp"
#include "MyThread.h"
#include "ARDrawingContext.hpp"
#include "ARPipeline.hpp"
#include "DebugHelpers.hpp"

////OpenCv includes
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
//
//

/***************************************************************************************************************/
//Only Image part with no Video
//int main(int argc, const char *argv[])
//{
//	cv::initModule_nonfree();
//
//	if (argc<3)
//	{
//		std::cout << "please specify input image images" << std::endl;
//		return 1;
//	}
//
//	//Get the pattern image and verify that it is not empty
//	cv::Mat patternImage = cv::imread(argv[1]);
//	cv::Mat inputImage = cv::imread(argv[2]);
//	if(patternImage.empty())
//	{
//		std::cout << "The input image seems to be empty, please check it :D" << std::endl;
//		return 2;
//	}
//	if(inputImage.empty())
//	{
//		std::cout << "The input image seems to be empty, please check it :D" << std::endl;
//		return 2;
//	}
//
//	PatternDetector patternDetector;
//	Pattern pattern;
//	PatternTrackingInfo info;
//
//	//1. Make pattern from input image. The detector is trained with the pattern
//	patternDetector.buildPatternFromImage(patternImage,pattern);
//
//	patternDetector.train(pattern);
//
//	patternDetector.findPattern(inputImage,info);
//}

//*******************************************************************************************************************************/
////Part using openGL uncomment this part in order to use video and project 3D onto image
//**
// * Processes a recorded video or live view from web-camera and allows you to adjust homography refinement and 
// * reprojection threshold in runtime.
// */
void processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, cv::VideoCapture& capture);


// * Same as above except takes a Mat as parameter instead of capture
// * 
// */
void processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, MyThread myThread[]);
//**
// * Processes single image. The processing goes in a loop.
// * It allows you to control the detection process by adjusting homography refinement switch and 
// * reprojection threshold in runtime.
// */
void processSingleImage(const cv::Mat& patternImage, CameraCalibration& calibration, const cv::Mat& image);

////**
//// * Performs full detection routine on camera frame and draws the scene using drawing context.
//// * In addition, this function draw overlay with debug information on top of the AR window.
//// * Returns true if processing loop should be stopped; otherwise - false.
//// */
bool processFrame(const cv::Mat& cameraFrame, ARPipeline& pipeline, ARDrawingContext& drawingCtx);

//Thread used for communication with Android Phone
MyThread myThread[CAM_NUM] = {PORT1};

void init(void)
{
	//Start the thread which sends and receives images
	myThread[0].beginSmartThread();

	//Wait until smartphone is connected
	while(!myThread[0].getCameraFlg())	
		waitKey(1);
}

int main(int argc, const char * argv[])
{
	init();
    // Change this calibration to yours:
    CameraCalibration calibration(526.58037684199849f, 524.65577209994706f, 318.41744018680112f, 202.96659047014398f);
    
    if (argc < 2)
    {
        std::cout << "Input image not specified" << std::endl;
        std::cout << "Usage: markerless_ar_demo <pattern image> [filepath to recorded video or image]" << std::endl;
        return 1;
    }

    // Try to read the pattern:
    cv::Mat patternImage = cv::imread(argv[1]);
    if (patternImage.empty())
    {
        std::cout << "Input image cannot be read" << std::endl;
        return 2;
    }

    if (argc == 2)
    {
		//Use for normal Webcam
		//processVideo(patternImage, calibration, cv::VideoCapture(1));
		
		//For capturing image from SmartPhone
		//cv::Mat inputImage;
		//myThread->getSmartImage().copyTo(inputImage);
		processVideo(patternImage, calibration, myThread);
		
	}
    else if (argc == 3)
    {
        std::string input = argv[2];
        cv::Mat testImage = cv::imread(input);
        if (!testImage.empty())
        {
            processSingleImage(patternImage, calibration, testImage);
        }
        else 
        {
            cv::VideoCapture cap(1);
            if (cap.open(input))
            {
                processVideo(patternImage, calibration, cap);
            }
        }
    }
    else
    {
        std::cerr << "Invalid number of arguments passed" << std::endl;
        return 1;
    }

	MyThread::closeHandle();
    return 0;
}

//ProcessVideo with Capture from Webcam
void processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, cv::VideoCapture& capture)
{
    // Grab first frame to get the frame dimensions
    cv::Mat currentFrame;  
    capture >> currentFrame;

    // Check the capture succeeded:
    /*if (currentFrame.empty())
    {
        std::cout << "Cannot open video capture device" << std::endl;
        return;
    }*/

    cv::Size frameSize(currentFrame.cols, currentFrame.rows);

    ARPipeline pipeline(patternImage, calibration);
    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);

    bool shouldQuit = false;
    do
    {
        capture >> currentFrame;
        if (currentFrame.empty())
        {
            shouldQuit = true;
            continue;
        }

        shouldQuit = processFrame(currentFrame, pipeline, drawingCtx);
    } while (!shouldQuit);
}

//processVideo with Mat directly
void processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, MyThread myThread[])
{
    // Grab first frame to get the frame dimensions
    cv::Mat currentFrame;  
	myThread->getSmartImage().copyTo(currentFrame);


    // Check the capture succeeded:
    /*if (currentFrame.empty())
    {
        std::cout << "Cannot open video capture device" << std::endl;
        return;
    }*/

    cv::Size frameSize(currentFrame.cols, currentFrame.rows);

    ARPipeline pipeline(patternImage, calibration);
    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);

    bool shouldQuit = false;
    do
    {
		myThread->getSmartImage().copyTo(currentFrame);
        if (currentFrame.empty())
        {
            shouldQuit = true;
            continue;
        }

        shouldQuit = processFrame(currentFrame, pipeline, drawingCtx);
    } while (!shouldQuit);
}

void processSingleImage(const cv::Mat& patternImage, CameraCalibration& calibration, const cv::Mat& image)
{
    cv::Size frameSize(image.cols, image.rows);
    ARPipeline pipeline(patternImage, calibration);
    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);

    bool shouldQuit = false;
    do
    {
        shouldQuit = processFrame(image, pipeline, drawingCtx);
    } while (!shouldQuit);
}

bool processFrame(const cv::Mat& cameraFrame, ARPipeline& pipeline, ARDrawingContext& drawingCtx)
{
    // Clone image used for background (we will draw overlay on it)
    cv::Mat img = cameraFrame.clone();

    // Draw information:
    if (pipeline.m_patternDetector.enableHomographyRefinement)
        cv::putText(img, "Pose refinement: On   ('h' to switch off)", cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));
    else
        cv::putText(img, "Pose refinement: Off  ('h' to switch on)",  cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

    cv::putText(img, "RANSAC threshold: " + ToString(pipeline.m_patternDetector.homographyReprojectionThreshold) + "( Use'-'/'+' to adjust)", cv::Point(10, 30), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

    // Set a new camera frame:
    drawingCtx.updateBackground(img);

    // Find a pattern and update it's detection status:
    drawingCtx.isPatternPresent = pipeline.processFrame(cameraFrame);

    // Update a pattern pose:
    drawingCtx.patternPose = pipeline.getPatternLocation();

    // Request redraw of the window:
    drawingCtx.updateWindow();

    // Read the keyboard input:
    int keyCode = cv::waitKey(5); 

    bool shouldQuit = false;
    if (keyCode == '+' || keyCode == '=')
    {
        pipeline.m_patternDetector.homographyReprojectionThreshold += 0.2f;
        pipeline.m_patternDetector.homographyReprojectionThreshold = std::min(10.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    }
    else if (keyCode == '-')
    {
        pipeline.m_patternDetector.homographyReprojectionThreshold -= 0.2f;
        pipeline.m_patternDetector.homographyReprojectionThreshold = std::max(0.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    }
    else if (keyCode == 'h')
    {
        pipeline.m_patternDetector.enableHomographyRefinement = !pipeline.m_patternDetector.enableHomographyRefinement;
    }
    else if (keyCode == 27 || keyCode == 'q')
    {
        shouldQuit = true;
    }

    return shouldQuit;
}