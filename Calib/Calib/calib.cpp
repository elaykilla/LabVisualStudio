#include "cvIncludes.h";

void showInstructions(){
	cout << "Camera Calibration" <<endl;
	cout << "The input images should be named with a number suffix. For example: example_1 example_2 etc..." <<endl;
	cout << "inputImageName is the name of the input image without the number suffix" <<endl;
	cout << "Image number is the number of input images" <<endl;
	cout << "imageWidth is the width of the images. All images should have equal width" <<endl;
	cout << "imageHeight is the height of the images. All images should have equal Height" <<endl;
	cout << "Calib.exe inputImageName imageNumb imageWidth imageheight" <<endl;

}

void calibrate(string filename, int nbImages, int imageWidth, int imageHeight, Mat& intrinsic, Mat& distortion)
{

	/*if (argc != 5){
		cout << "Please verify the number of arguments";
		showInstructions();
		return -1;

	}*/
	//CHESS_ROWS and CHESS_COLS represent number of cornes along X and Y
	const int CHESS_ROWS = 7;
	const int CHESS_COLS = 10;
	const int CORNER_NUM = CHESS_COLS * CHESS_ROWS;

	//CHESSBOARD_NUM is the number of input images
	const int CHESSBOARD_NUM = nbImages;
	const bool DISPLAY_CHESS = true;

	//The Width and Height of the images
	int PGR_Width = imageWidth;
	int PGR_Height = imageHeight;

	//DetectedChessCount is the number of images in which the chessboard has been detected
	int DetectedChessCount = 0;

	//CHESS_SIZE is the size in mm of a black square
	const double CHESS_SIZE = 25.0;
	const  cv::Size patternSize(CHESS_COLS, CHESS_ROWS);

	//Detected_2DCorners contains the corners of all the images
	std::vector<std::vector<cv::Point2f>> Detected_2DCorners;		
	//Chessboard
	std::cout << "detection...."<<std::endl;

	//Run through each image
	for(int j=0; j<CHESSBOARD_NUM; j++){	
		//
		string imageLoc;
		std::vector<cv::Point2f>	PGRCorners;
		bool canFindPGRCorners;
		//Load

		//Read image location
		imageLoc = filename;
		std::ostringstream PGRChessName;
		PGRChessName << imageLoc <<"_"<< j << ".jpg";
		cv::Mat PGRChessImage = cv::imread(PGRChessName.str(), 0);

		//verify that image is found
		if(!PGRChessImage.data){
			cout << "One or more of the input images were not found" << PGRChessName.str() <<endl;
			return;
		}

		//Verify image width and height
		if(PGRChessImage.cols!=PGR_Width | PGRChessImage.rows!=PGR_Height){
			cout << "One or more of the input images have incorrect dimensions." <<endl;
			cout << "Expected: (Height, Width) = (" << PGR_Height << "," << PGR_Width << ")"<< "but found: (" << PGRChessImage.cols << "," << PGRChessImage.rows << ")" << endl;
			return;
		}
		


		//verify if Chessboard is found
		canFindPGRCorners = cv::findChessboardCorners(
			PGRChessImage, 
			patternSize, 
			PGRCorners,
			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
		if(canFindPGRCorners){
			//detect the corners
			cv::cornerSubPix(
				PGRChessImage,
				PGRCorners,
				cv::Size(11, 11),
				cv::Size(-1, -1),
				cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 30, 0.01) );

			//Add the detected corners 
			Detected_2DCorners.push_back(PGRCorners);

			//Augment the number of successfuly detected chessboards
			DetectedChessCount++;
		}
	}
	std::cout << DetectedChessCount*CORNER_NUM<<" points were detected..."<<std::endl;
	//object points
	std::vector<std::vector<cv::Point3f>> Chess3Dpoints;
	cv::Point3f point_3d;
	//3Dpoints

	//Calculate 3D cordinnates of each corner 
	std::vector<cv::Point3f> Chess3Dpoints_tmp;
	for (int j = 0; j < CHESS_ROWS; j++) {
		for (int k = 0; k < CHESS_COLS; k++) {
			//3Dpoints‚ÌŠi”[
			point_3d.x = j*CHESS_SIZE;
			point_3d.y = k*CHESS_SIZE;
			point_3d.z = 0.0;
			Chess3Dpoints_tmp.push_back(point_3d);
		}
	}

	//Add cordinates for DetectedChessCount times
	for (int i = 0; i < DetectedChessCount; i++) {
		Chess3Dpoints.push_back(Chess3Dpoints_tmp);
	}

	cv::Mat intrinsic_matrix = cv::Mat::eye(3, 3, CV_64F);
	//distortion
	cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
	//
	std::vector<cv::Mat> rvecs, tvecs;
	//cv::Mat rvecs;
	//cv::Mat tvecs;



	cv::Size image_size(PGR_Width, PGR_Height);
	//
	//Find intrinsic and extrinsic camera parameters
	double rms = cv::calibrateCamera(
		Chess3Dpoints,
		Detected_2DCorners,
		image_size,
		intrinsic_matrix,
		distCoeffs, 
		rvecs, tvecs);
	//CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE); 

	std::cout << "Re-projection error reported by calibrateCamera: "<< rms << std::endl;


	//ƒtƒ@ƒCƒ‹o—Í
	std::ostringstream parameters;
	parameters << "Calibration" << ".xml";
	cv::FileStorage	cvfs(parameters.str(), cv::FileStorage::WRITE);

	//fill matrices
	intrinsic <- intrinsic_matrix;
	distortion <-distCoeffs;

	//wirte data to filer
	cv::write(cvfs, "intrinsic", intrinsic_matrix);
	cv::write(cvfs, "distortion", distCoeffs); 
}