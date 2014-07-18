#include "cvIncludes.h";
#include "cart2Sphere.hpp";

#include <math.h>;
/**
This function converts and image from cartesian coordinates (x,y) to spherical coordinates (theta, phi)
With normalized coordinates, weapply radial distortion then focal length translate

It requires to have camera intrinsic and extrensic parameters 
*/
void cart2Sphere::cart2Spheric(Mat inputImage, Mat& outputImage, Mat intrinsic, Mat distortion ){

	/*outputImage <- inputImage;*/

	double fx = intrinsic.at<double>(0,0);
	double fy = intrinsic.at<double>(1,1);

	//Parameters for spherical conversion
	double theta,phy,r;

	//Points to be used
	Point2d imPoint;
	Point3d normPoint;
	Point2d sphyPoint;

	cout << "beginning process" <<endl;
	for(int i=0;i<inputImage.rows;i++){
		for(int j=0;j<inputImage.cols;j++){
			//

			cout << "i,j" <<i << "," <<j << endl;
			imPoint.x=normPoint.x=i;
			imPoint.y=normPoint.y=j;
			normPoint.z=1;

			r = sqrt(double(i*i + j*j + 1));
			theta = acos((double)(1./r));
			phy = atan2((double)j,(double)i);
			cout << "theta,phy" << theta << " , " << phy << endl;

			sphyPoint.x= floor(fx*theta);
			sphyPoint.y = floor(fy*phy);

			cout << "Xsphy,Ysphy" << sphyPoint.x << "," << sphyPoint.y << endl;

			//outputImage.at<double>(sphyPoint.x,sphyPoint.y) = inputImage.at<double>(i,j);
		}
	
	
	}

}