#pragma once
#include <winsock2.h>
#include "MyOpenCV.h"
#include "define.h"

typedef struct
{
	int port;
	int cameraFlg;
	Mat smartImage;
	Mat resImage;
}PARAM, *lpPARAM;

class MyThread
{
private:
	unsigned thID;
	static int m_nbThread;
	static HANDLE	m_hMutex;
	static HANDLE	m_hThread[];
	PARAM param;

	static unsigned __stdcall func1(void *lpx);
	static void connect_s(int port, int* cameraFlg, Mat smartImage, Mat resImage);

public:
	MyThread(int port);
	MyThread(void);
	~MyThread(void);

	void beginSmartThread(void);
	static void closeHandle(void);
	int getCameraFlg(void);
	Mat getSmartImage(void);
	void inputResImage(Mat resImage);

};