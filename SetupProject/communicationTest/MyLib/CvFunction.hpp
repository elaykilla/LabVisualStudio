/*
2011/8/2	Ver2.0
*/

#pragma once
//#include "OpenCvHeader.hpp"
#include "../MyOpenCV.h"
#include "iostream"
#include "fstream"

#ifndef OPENCV_1_1
template<typename _Tp> void printMatrix(const cv::Mat& mat)
{
	int i, j, k;

	for (i = 0; i < mat.rows; i++){
		for (j = 0; j < mat.cols; j++){
			for (k = 0; k < mat.channels(); k++){
				std::cout << mat.at<_Tp>(i, j * mat.channels() + k) << "\t";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
#endif

#ifndef OPENCV_1_1
template<typename _Tp> void saveMatrix(const std::string filename, const cv::Mat& mat)
{
	int i, j, k;
	std::ofstream ofs(filename.c_str());
	
	for (i = 0; i < mat.rows; i++){
		for (j = 0; j < mat.cols; j++){
			for (k = 0; k < mat.channels(); k++){
				ofs << mat.at<_Tp>(i, j * mat.channels() + k) << "\t";
			}
		}
		ofs << "\n";
	}
}
#endif

void charToColorMat(const char* src, cv::Mat dst);