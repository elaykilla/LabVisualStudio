#include "MyThread.h"
#include"SocketServer.hpp"
#include <process.h>

int MyThread::m_nbThread = 0;
HANDLE	MyThread::m_hMutex = CreateMutex(NULL,FALSE,NULL);
HANDLE	MyThread::m_hThread[CAM_NUM];

MyThread::MyThread(int port)
{
	param.port = port;
	param.cameraFlg = false;
	param.resImage = Mat::zeros(MOBILE_IMAGE_HEIGHT, MOBILE_IMAGE_WIDTH, CV_8UC3);
	param.smartImage = Mat::zeros(MOBILE_IMAGE_HEIGHT, MOBILE_IMAGE_WIDTH, CV_8UC3);
}

MyThread::MyThread(void)
{
}

MyThread::~MyThread(void)
{
}

unsigned __stdcall MyThread::func1(void *lpx)
{
	lpPARAM lpParam = (lpPARAM)lpx;
	connect_s(lpParam->port, &lpParam->cameraFlg, lpParam->smartImage, lpParam->resImage);
	return 0;
}

void MyThread::beginSmartThread(void)
{
	m_hThread[m_nbThread++] = (HANDLE)_beginthreadex(NULL, 0, func1, &param, 0, &thID);	//Create thread
}

void MyThread::closeHandle(void)
{
	for(int i = 0; i < m_nbThread; i++)
		CloseHandle(m_hThread[i]);
	CloseHandle(m_hMutex);
}

int MyThread::getCameraFlg(void)
{
	return param.cameraFlg;
}

Mat MyThread::getSmartImage(void)
{
	return param.smartImage;
}

void MyThread::inputResImage(Mat resImage)
{
	resImage.copyTo(param.resImage);
}

void MyThread::connect_s(int port, int* cameraFlg, Mat smartImage, Mat resImage)
{
	int recvSize = 0;
	int sendSize = 0;
	double sensor[3] = {0.0, 0.0, 0.0};
	CSocketServer socket(port);
	Mat image;
	CImageProp props;

	while (1){
		/* Establish connection */
		socket.acceptConnect();
		props = socket.receiveProps();

		image.create(props.height, props.width, CV_8UC3);

		while (1){
			/* Receive JPEG image data */
			WaitForSingleObject(m_hMutex, INFINITE);
		
			recvSize = socket.receiveSensor(sensor);
			recvSize = socket.receiveImage(image);

			ReleaseMutex(m_hMutex);
			cvtColor(image, image, CV_BGR2RGB);
			image.copyTo(smartImage);
			*cameraFlg = true;
			if (recvSize < 0)	break;

			///Get sensor infoiunverifiedj
			//char text[255];
			//sprintf_s(text, 255, "x = %lf", sensor[0]);
			//putText(image, text, Point(30, 30), FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(0, 0, 255));
			//sprintf_s(text, 255, "y = %lf", sensor[1]);
			//putText(image, text, Point(30, 130), FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(0, 0, 255));
			//sprintf_s(text, 255, "z = %lf", sensor[2]);
			//putText(image, text, Point(30, 230), FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(0, 0, 255));
			
			/* Send JPEG image data */
			WaitForSingleObject(m_hMutex, INFINITE);
			if (!resImage.empty()){
				sendSize = socket.sendImage(resImage, props.jpg_ratio);
			}
			else{
				sendSize = socket.sendImage(image, props.jpg_ratio);
			}
			ReleaseMutex(m_hMutex);
			if (sendSize < 0)	break;
		}
		if (socket.isClosed())	break;
	}
	return;
}


