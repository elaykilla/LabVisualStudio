#pragma once
#pragma comment(lib, "ws2_32.lib")

#include "winsock2.h"	/* Include before "windows.h" */
#include <ws2tcpip.h>
#include "iostream"
#include "MyTimer.hpp"
#include "header.hpp"

class CImageProp
{
public:
	int width;
	int height;
	int channel;
	int jpg_ratio;
};

class CSocketServer
{
private:
	int m_srcSocket;
	int m_dstSocket;
	int m_dstAddrSize;
	struct sockaddr_in m_srcAddr;
	struct sockaddr_in m_dstAddr;
	WSADATA m_data;
	int m_status;
	char* m_statusPacket;
	char* m_buffer;
	char* m_sensorBuffer;
	CImageProp m_props;

public:
	CSocketServer(int port, int numOfConnect = 1);
	CSocketServer(void);
	~CSocketServer(void);

	// Initialize variables
	void init(int port, int numOfConnect);
	// Wait until establishing connection
	void acceptConnect(void);
	int receive(char* buffer, int sizeOfBuffer);
	int sendData(char* buffer, int bufferSize);
	void closeSocket(void);
	bool isClosed() const;
	int receiveImage(cv::Mat& image);				
	int receiveInt(void);			
	int sendImage(const cv::Mat& image, int ratio);	
	CImageProp receiveProps();
	int receiveSensor(double* sensor);


private:
	void createSocket(void);
	void bindSocket(void);
	void listenSocket(int numOfConnect = 1);
	void charToMat(int dataLength, char* data, cv::Mat& image) const;
	int packetToByteLength(char* packet) const;
	char* imageToJPEG(const cv::Mat& image, int* dataLength, int ratio) const;
	void setPacket(char* sendPacket, int dataLength) const;
	double charToDouble(char* data);
};
