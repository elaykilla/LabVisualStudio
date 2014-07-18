#pragma once

#include <math.h>;

class cart2Sphere
{
public:
	cart2Sphere(void);
	static void cart2Spheric(Mat inputImage, Mat& outputImage, Mat intrinsic, Mat distortion);
};

