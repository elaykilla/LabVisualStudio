#include "cvIncludes.h";
#include "cart2Sphere.hpp";


int main(int argc, char** argv)
{
	Mat inputImage =imread(argv[1]);
	Mat outputImage (inputImage.rows,inputImage.cols,CV_64F);

	Mat intrinsic = Mat::eye(3, 3, CV_64F);
	Mat distortion = Mat::zeros(8, 1, CV_64F);
	
	intrinsic.at<double>(0,0) = 3.6734407997956879;
	intrinsic.at<double>(1,1) = 3.9330476376696015;

	cart2Sphere::cart2Spheric(inputImage,outputImage,intrinsic,distortion);

	imshow( "Image", inputImage );
	imshow( "Spheric", outputImage );
}