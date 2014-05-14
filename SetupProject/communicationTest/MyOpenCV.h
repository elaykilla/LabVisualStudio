#ifndef MY_OPENCV_H
#define MY_OPENCV_H

#include "C:\Users\Elay\Workspace\opencv\include\opencv2\opencv.hpp"

#ifdef _DEBUG
//Debug
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_core300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_imgproc300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_highgui300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_objdetect300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_features2d300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_flann300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_calib3d300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_nonfree300d.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_ml300d.lib")
#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_video300d.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_contrib300d.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_legacy300d.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_gpu300d.lib")
////#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_haartraining_engine.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\opencv\\build\\install\\x64\\vc10\\lib\\opencv_ts300d.lib")
#else
#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_core246.lib")
#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_imgproc246.lib")
#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_highgui246.lib")
#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_objdetect246.lib")
#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_features2d246.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_flann246.lib")
#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_calib3d246.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_ml246.lib")
#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_video246.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_contrib246.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_legacy246.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_gpu246.lib")
////#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_haartraining_engine.lib")
//#pragma comment(lib,"C:\\Users\\Elay\\Workspace\\opencv\\build\\x64\\vc10\\lib\\opencv_ts246.lib")
#endif

using namespace std;
using namespace cv;

#endif