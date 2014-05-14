/*
2011/3/29	Ver1.0
*/

#include "CvFunction.hpp"

void charToColorMat(const char* src, cv::Mat dst)
{
	int i, j, count = 0;
	for (i = 0; i < dst.rows; i++){
		for (j = 0; j < dst.cols; j++){
			dst.at<unsigned char>(i,j*3+0) = (unsigned char)src[count++];
			dst.at<unsigned char>(i,j*3+1) = (unsigned char)src[count++];
			dst.at<unsigned char>(i,j*3+2) = (unsigned char)src[count++];
		}
	}
}