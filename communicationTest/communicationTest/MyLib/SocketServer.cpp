#include "SocketServer.hpp"

using namespace std;



CSocketServer::CSocketServer(int port, int numOfConnect)
{
	init(port, numOfConnect);
}



CSocketServer::CSocketServer()
{
}



CSocketServer::~CSocketServer(void)
{
	delete []m_statusPacket;
	delete []m_buffer;
	delete []m_sensorBuffer;
	WSACleanup();
}

/********************************************************************/
/*public function*/
/********************************************************************/



void CSocketServer::init(int port, int numOfConnect)
{
	m_dstAddrSize = sizeof(m_dstAddr);
	WSAStartup(MAKEWORD(2,0), &m_data);					// For Windows
	memset(&m_srcAddr, 0, sizeof(m_srcAddr));
	m_srcAddr.sin_port = htons(port);					//Port number
	m_srcAddr.sin_family = AF_INET;						//Between different machines
	m_srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);		//Server IP
	createSocket();
	bindSocket();
	listenSocket(numOfConnect);
	m_status = 1;
	m_statusPacket = new char[sizeof(int)];
	m_sensorBuffer = new char[sizeof(double) * 3];
}



void CSocketServer::acceptConnect(void)
{
	cout << "------------------\nwaiting for connecting\n------------------\n\n" << endl;
	m_dstSocket = (int)accept(m_srcSocket, (struct sockaddr *) &m_dstAddr, &m_dstAddrSize);
	cout << "Received connection from " << inet_ntoa(m_dstAddr.sin_addr) << "\n" << endl ;
}



int CSocketServer::receive(char* buffer, int sizeOfBuffer)
{
	int recvSize = 0;
	int numrcv = 0;
	int count = 0;
	int i = 0;
	char* tmpBuffer = new char[sizeOfBuffer];

	for (; recvSize < sizeOfBuffer; ){
		numrcv = recv(m_dstSocket, tmpBuffer, sizeOfBuffer - recvSize, 0);
		recvSize += numrcv;
		for (i = 0; i < numrcv; i++){
			buffer[count++] = tmpBuffer[i];
		}

		switch (numrcv){
		case 0:					/* End communication */
			delete []tmpBuffer;
			return -1;
		case -1:				/* Error */
			delete []tmpBuffer;
			cout << "error : " << WSAGetLastError() << endl;
			return -1;
		default:
			break;
		}
	}
	delete []tmpBuffer;
	return recvSize;
}



int CSocketServer::sendData(char* buffer, int sizeOfBuffer)
{
	int sendSize = 0;
	sendSize = send(m_dstSocket, buffer, sizeOfBuffer, 0);

	if (sendSize == SOCKET_ERROR) return WSAGetLastError();
	else return sendSize;
}	



void CSocketServer::closeSocket(void)
{
	m_status = closesocket(m_dstSocket);
}



bool CSocketServer::isClosed(void) const
{
	return (m_status == 0);
}


int CSocketServer::receiveImage(cv::Mat& image)
{
	int status = receive(m_statusPacket, sizeof(int));	//4byte
	int size = packetToByteLength(m_statusPacket);
	status = receive(m_buffer, size);
	charToMat(size, m_buffer, image);
	return status;
}

int CSocketServer::receiveInt(void)
{
	int status = receive(m_statusPacket, sizeof(int));	//4byte
	int size = packetToByteLength(m_statusPacket);
	status = receive(m_buffer, size);
	return packetToByteLength(m_buffer);
}


int CSocketServer::sendImage(const cv::Mat& image, int ratio)
{
	int dataLength = 0;
	char* sendDataArray = imageToJPEG(image, &dataLength, ratio);
	setPacket(m_statusPacket, dataLength);
	int sendSize = sendData(m_statusPacket, sizeof(int));
	sendSize = sendData(sendDataArray, dataLength);
	delete []sendDataArray;
	return sendSize;
}

CImageProp CSocketServer::receiveProps()
{
	CImageProp props;
	int total_byteLength = 16;
	char* buffer = new char[total_byteLength];
	int result = receive(buffer, total_byteLength);
	char width[4];
	char height[4];
	char ratio[4];
	char channel[4];
	for (int i = 0; i < 4; i++){
		width[i] = (unsigned char)buffer[i];
		height[i] = (unsigned char)buffer[i+4];
		ratio[i] = (unsigned char)buffer[i+8];
		channel[i] = (unsigned char)buffer[i+12];
	}
	props.width = packetToByteLength(buffer);
	props.height = packetToByteLength(height);
	props.jpg_ratio = packetToByteLength(ratio);
	props.channel = packetToByteLength(channel);
	m_buffer = new char[props.width * props.height * props.channel];

	cout << "Width : " << props.width << endl;
	cout << "Height : " << props.height << endl;
	cout << "Channel : " << props.channel << endl;
	cout << "Ratio : " << props.jpg_ratio << endl << endl;

	return props;
}

int CSocketServer::receiveSensor(double* sensor)
{
	union{
		double d;
		char c_a[8];
	} tmp[3];

	int total_byteLength = 24;
	int status = receive(m_sensorBuffer, total_byteLength);
	for (int i = 0; i < 8; i++){
		tmp[0].c_a[i] = (unsigned char)m_sensorBuffer[7-i];
		tmp[1].c_a[i] = (unsigned char)m_sensorBuffer[15-i];
		tmp[2].c_a[i] = (unsigned char)m_sensorBuffer[23-i];
	}
	for (int i = 0; i < 3; i++){
		sensor[i] = tmp[i].d;
	}
	//cout << tmp[0].d << "  \t" << tmp[1].d << "  \t" << tmp[2].d << endl;

	return status;
}

/********************************************************************/
/*private function*/
/********************************************************************/


void CSocketServer::createSocket(void)
{
	m_srcSocket = (int)socket(AF_INET, SOCK_STREAM, 0);
}



void CSocketServer::bindSocket(void)
{
	bind(m_srcSocket, (struct sockaddr *) &m_srcAddr, sizeof(m_srcAddr));
}


void CSocketServer::listenSocket(int numOfconnect)
{
	listen(m_srcSocket, numOfconnect);
}



void CSocketServer::charToMat(int dataLength, char* data, cv::Mat& image) const
{
	int i;
	std::vector<uchar> tmp;
	for (i = 0; i < dataLength; i++){
		tmp.push_back((unsigned char)data[i]);
	}
	cv::Mat mat(tmp);
	cv::imdecode(mat, CV_LOAD_IMAGE_ANYCOLOR).copyTo(image);
}



int CSocketServer::packetToByteLength(char* packet) const
{
	int result = 0;
	result += (unsigned char)packet[0] * 256 * 256 * 256;
	result += (unsigned char)packet[1] * 256 * 256;
	result += (unsigned char)packet[2] * 256;
	result += (unsigned char)packet[3];
	return result;
}



char* CSocketServer::imageToJPEG(const cv::Mat& image, int* dataLength, int ratio) const
{
	int i;
	int count = 0;
	int size = 0;
	cv::Mat tmp(image);
	static std::vector<uchar> uc;
	vector<int> param = vector<int>(2);
	param[0]=CV_IMWRITE_JPEG_QUALITY;
	param[1]=ratio;//default(95) 0-100

	cv::imencode(".jpg", tmp, uc, param);
	size = (int)uc.size();

	char* result = new char[size];
	for (i = 0; i < size; i++){
		result[count] = uc[count];
		count++;
	}
	*dataLength = size;
	return result;
}



void CSocketServer::setPacket(char* sendPacket, int dataLength) const
{
	sendPacket[0] = (dataLength >> 24) & 0xFF;
	sendPacket[1] = (dataLength >> 16) & 0xFF;
	sendPacket[2] = (dataLength >> 8) & 0xFF;
	sendPacket[3] = dataLength & 0xFF;
}



double CSocketServer::charToDouble(char* data)
{
	//double result;
	//for (int i = 0; i < sizeof(double); i++){
	//	;
	//	result << 1;
	//}
	return 0.0;
}

