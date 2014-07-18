#include "cvIncludes.h";
#include "cart2Sphere.h";

#include <math.h>;
/**
This function converts and image from cartesian coordinates (x,y) to spherical coordinates (theta, phi)
With normalized coordinates, weapply radial distortion then focal length translate

It requires to have camera intrinsic and extrensic parameters 
*/
void cart2Sphere(Mat inputImage, Mat& outputImage, Mat intrinsic, Mat distortion ){

	outputImage <- inputImage;

	double fx = intrinsic.at<double>(0,0);
	double fy = intrinsic.at<double>(1,1);

	for(int i=0;i<inputImage.rows;i++){
		for(int j=0;j<inputImage.cols;j++){
			//Applying radial distortion
			double r = sqrt(double(i*i + j*j));

		}
	
	
	}

}