////-----------------------------------
//// Unwrapping Omnidirectional Images
////-----------------------------------
//// Author: Andrzej Pronobis
//// Contact: pronobis@csc.kth.se
////          www.csc.kth.se/~pronobis
////-----------------------------------
//
//#include <stdio.h>
//#include <iostream>
////#include <cv.h>
//#include<opencv/cv.h>
//#include <opencv2/highgui/highgui.hpp>
//using namespace std;
//
//const double PI = 3.1415926535897932384626;
//
//struct Options
//{
//  int cx, cy, ri, ro;
//  string input, output, filtered, test;
//  bool interpolation, bilinear, bicubic;
//  double sx, sy;
//  int blackThr;
//  int borderT, borderB, borderL, borderR;
//  bool fixedCenter, estimateCenterThreshold, estimateCenterEdge;
//  bool unwrap, createTest;
//  int minRadius, maxRadius;
//  int edgeCX, edgeCY, edgeDX, edgeDY;
//};
//
//
//// ---------------------------------------
//void printHeader()
//{
//  cout<<"+-----------------------------------+"<<endl;
//  cout<<"|           Unwrap                  |"<<endl;
//  cout<<"| Unwrapping Omnidirectional Images |"<<endl;
//  cout<<"+-----------------------------------+"<<endl;
//  cout<<"| Author: Andrzej Pronobis          |"<<endl;
//  cout<<"| Contact: pronobis@csc.kth.se      |"<<endl;
//  cout<<"|          www.csc.kth.se/~pronobis |"<<endl;
//  cout<<"+-----------------------------------+"<<endl;
//  cout<<endl;
//}
//
//
//// ---------------------------------------
//void printHelp()
//{
//  cout<<endl<<"Usage: unwrap [-bl|-bc] [-sx <x_ratio>] [-sy <y_ratio>]"<<endl;
//  cout<<"               [-tc <black_threshold>|-fc <center_x> <center_y>|"<<endl;
//  cout<<"               -ec <min_radius> <max_radius> <center_x> <center_y> <dx> <dy>]"<<endl;
//  cout<<"               [-fi <image_file>] [-eb <l> <t> <r> <b>]"<<endl;
//  cout<<"               [-ti <image_file>] [-uw <output_file>]"<<endl;
//  cout<<"              <inner_radius> <outer_radius> <input_file>"<<endl;
//  cout<<endl<<"Options:"<<endl;
//  cout<<" -bl - use bilinear interpolation (default: OFF)"<<endl;
//  cout<<" -bc - use bicubic interpolation (default: OFF)"<<endl;
//  cout<<" -sx <x_ratio> produce output scaled horizontally by <x_ratio> (def. 1)"<<endl;
//  cout<<" -sy <y_ratio> produce output scaled vertically by <y_ratio> (def. 1)"<<endl;
//  cout<<" -tc <black_threshold> - estimate center of the image using fast (but"<<endl;
//  cout<<"                         not always robust) alg. based on thresholding."<<endl;
//  cout<<" -ec <min_radius> <max_radius> <center_x> <center_y> <dx> <dy> - estimate"<<endl;
//  cout<<"         center of the image using slower but more robust algorithm based"<<endl;
//  cout<<"         on edge detection and Hough transform. To speed up the search,"<<endl;
//  cout<<"         only images centered in (<center_x>+/-<dx>, <center_y>+/-<dy>)"<<endl;
//  cout<<"         and of radius <min_radius> to <max_radius> are considered."<<endl;
//  cout<<" -fi <image_file> - save the filtered image produced during"<<endl;
//  cout<<"                    thresholding or edge detection"<<endl;
//  cout<<" -eb <l> <t> <r> <b> - exclude border during center estimation (def. 0)"<<endl;
//  cout<<" -fc <center_x> <center_y> - use fixed center of the omni image"<<endl; 
//  cout<<" -uw <output_file> - unwrap the omnidirectional image"<<endl; 
//  cout<<" -ti <image_file> - create test image - the input image on which"<<endl; 
//  cout<<"                    the center and radii are plotted"<<endl; 
//  cout<<endl<<"Arguments:"<<endl;
//  cout<<" <inner_radius> - inner radius of the omnidirectional image"<<endl;
//  cout<<" <outer_radius> - outer radius of the omnidirectional image"<<endl;
//  cout<<" <input_file> - input omnidirectional image file"<<endl;
//  cout<<endl<<"Examples:"<<endl;
//  cout<<" - estimating the center of the image using the thresholding-based"<<endl;
//  cout<<"   method, 5 px boundary on top and bottom of the input img. is excluded"<<endl;
//  cout<<"   unwrap -tc 30 -eb 0 5 0 5 -fi filtered.jpg 40 230 input.jpg"<<endl;
//  cout<<" - estimating the center of the image using the edge-based method,"<<endl;
//  cout<<"   test image is also created"<<endl;
//  cout<<"   unwrap -ec 230 240 330 240 20 20 -ti test.jpg 40 230 input.jpg"<<endl;
//  cout<<" - unwrapping using fixed center and no interpolation or scaling:"<<endl;
//  cout<<"   unwrap -uw output.jpg -fc 330 240 40 230 input.jpg"<<endl;
//  cout<<" - unwrapping using automatic center point detection and bicubic interp.:"<<endl;
//  cout<<"   unwrap -uw output.jpg -ec 230 240 330 240 20 20 -bc 40 230 input.jpg"<<endl;
//  cout<<" - as above, but with vertical resolution of the output increased 2x:"<<endl;
//  cout<<"   unwrap -uw out.jpg -ec 230 240 330 240 20 20 -bc -sy 2 40 230 in.jpg"<<endl;
//}
//
//
//// ---------------------------------------
//bool parseCommandLineArgs(int argc, char *argv[], Options *opt)
//{
//  if (argc<4)
//  {
//    cerr<<"Error: Incorrect number of arguments."<<endl;
//    return false;
//  }
//
//  // Parse options
//  opt->interpolation=false;
//  opt->bilinear=false;
//  opt->bicubic=false;
//  opt->sx=1.0;
//  opt->sy=1.0;
//  opt->blackThr=30;
//  opt->cx=-1;
//  opt->cy=-1;
//  opt->estimateCenterThreshold=false;
//  opt->estimateCenterEdge=false;
//  opt->fixedCenter=false;
//  opt->unwrap=false;
//  opt->borderL=0;
//  opt->borderR=0;
//  opt->borderT=0;
//  opt->borderB=0;
//  opt->createTest=false;
//
//  bool options=true;
//  int optCount=0;
//  while (options)
//  {
//    if (!strcmp(argv[optCount+1], "-bl"))
//    {
//      if (opt->interpolation)
//      {
//        cerr<<"Error: Only one interpolation algorithm can be used."<<endl;
//        return false;
//      }
//      ++optCount;
//      opt->bilinear=true;
//      opt->interpolation=true;
//    }
//    else if (!strcmp(argv[optCount+1], "-bc"))
//    {
//      if (opt->interpolation)
//      {
//        cerr<<"Error: Only one interpolation algorithm can be used."<<endl;
//        return false;
//      }
//      ++optCount;
//      opt->bicubic=true;
//      opt->interpolation=true;
//    }
//    else if (!strcmp(argv[optCount+1], "-sx"))
//    {
//      optCount+=2;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing argument of the -sx option."<<endl;
//        return false;
//      }
//      opt->sx=atof(argv[optCount]);
//      if (opt->sx<=0)
//      {
//        cerr<<"Error: The argument of -sx cannot be <=0."<<endl;
//        return false;
//      }
//    }
//    else if (!strcmp(argv[optCount+1], "-sy"))
//    {
//      optCount+=2;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing argument of the -sy option."<<endl;
//        return false;
//      }
//      opt->sy=atof(argv[optCount]);
//      if (opt->sy<=0)
//      {
//        cerr<<"Error: The argument of -sy cannot be <=0."<<endl;
//        return false;
//      }
//    }
//    else if (!strcmp(argv[optCount+1], "-tc"))
//    {
//      optCount+=2;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing argument of the -tc option."<<endl;
//        return false;
//      }
//      opt->blackThr=atoi(argv[optCount]);
//      opt->estimateCenterThreshold=true;
//      if (opt->blackThr<0)
//      {
//        cerr<<"Error: The argument of -tc cannot be <0."<<endl;
//        return false;
//      }
//      if ((opt->fixedCenter) || (opt->estimateCenterEdge))
//      {
//        cerr<<"Error: Only one center estimation method can be used."<<endl;
//        return false;
//      }
//    }
//    else if (!strcmp(argv[optCount+1], "-ec"))
//    {
//      optCount+=7;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing arguments of the -ec option."<<endl;
//        return false;
//      }
//      opt->minRadius=atoi(argv[optCount-5]);
//      opt->maxRadius=atoi(argv[optCount-4]);
//      opt->edgeCX=atoi(argv[optCount-3]);
//      opt->edgeCY=atoi(argv[optCount-2]);
//      opt->edgeDX=atoi(argv[optCount-1]);
//      opt->edgeDY=atoi(argv[optCount]);
//      opt->estimateCenterEdge=true;
//      if ((opt->minRadius<0) || (opt->maxRadius<opt->minRadius) ||
//          (opt->edgeCX<0) || (opt->edgeCY<0) || (opt->edgeDX<0) || (opt->edgeDY<0))
//      {
//        cerr<<"Error: The arguments of -ec must be >=0 and <max_radius> >= <min_radius>."<<endl;
//        return false;
//      }
//      if ((opt->fixedCenter) || (opt->estimateCenterThreshold))
//      {
//        cerr<<"Error: Only one center estimation method can be used."<<endl;
//        return false;
//      }
//    }
//    else if (!strcmp(argv[optCount+1], "-fc"))
//    {
//      optCount+=3;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing arguments of the -fc option."<<endl;
//        return false;
//      }
//      opt->fixedCenter=true;
//      opt->cx=atoi(argv[optCount-1]);
//      opt->cy=atoi(argv[optCount]);
//      if ((opt->estimateCenterThreshold) || (opt->estimateCenterEdge))
//      {
//        cerr<<"Error: Only one center estimation method can be used."<<endl;
//        return false;
//      }
//    }
//    else if (!strcmp(argv[optCount+1], "-uw"))
//    {
//      optCount+=2;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing argument of the -uw option."<<endl;
//        return false;
//      }
//      opt->output=argv[optCount];
//      opt->unwrap=true;
//    }
//    else if (!strcmp(argv[optCount+1], "-fi"))
//    {
//      optCount+=2;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing argument of the -fi option."<<endl;
//        return false;
//      }
//      opt->filtered=argv[optCount];
//    }
//    else if (!strcmp(argv[optCount+1], "-ti"))
//    {
//      optCount+=2;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing argument of the -ti option."<<endl;
//        return false;
//      }
//      opt->test=argv[optCount];
//      opt->createTest=true;
//    }
//    else if (!strcmp(argv[optCount+1], "-eb"))
//    {
//      optCount+=5;
//      if (argc<(optCount+1))
//      {
//        cerr<<"Error: Missing arguments of the -eb option."<<endl;
//        return false;
//      }
//      opt->borderL=atoi(argv[optCount-3]);
//      opt->borderT=atoi(argv[optCount-2]);
//      opt->borderR=atoi(argv[optCount-1]);
//      opt->borderB=atoi(argv[optCount]);
//      if ((opt->borderL<0)||(opt->borderT<0)||(opt->borderR<0)||(opt->borderB<0))
//      {
//        cerr<<"Error: The arguments of -eb cannot be <0."<<endl;
//        return false;
//      }
//    }
//    else
//    {
//      options=false;
//    }
//  }
//
//  // Parse arguments
//  if ((argc-optCount)!=4)
//  {
//    cerr<<"Error: Incorrect number of arguments."<<endl;
//    return false;
//  }
//
//  opt->ri=atoi(argv[optCount+1]);
//  opt->ro=atoi(argv[optCount+2]);
//  opt->input=argv[optCount+3];
//
//  if ((!opt->unwrap) && (!opt->estimateCenterThreshold) && (!opt->estimateCenterEdge) && (!opt->createTest))
//  {
//    cerr<<"Error: There is nothing to do. Use -tc, -ec, -uw, -ti."<<endl;
//    return false;
//  }
//
//  if ((opt->ri<0) || (opt->ro<opt->ri))
//  {
//    cerr<<"Error: The radii must be: 0 < <inner_radius> < <outer_radius>."<<endl;
//    return false;
//  }
//
//  return true;
//}
//
//
//// ---------------------------------------
//void printOptions(const Options &opt)
//{
//  cout<<"Selected options:"<<endl;
//  if (opt.estimateCenterThreshold)
//  {
//    cout<<" - center will be estimated automatically using the thresholding-based alg."<<endl;
//    cout<<"     - black threshold: "<<opt.blackThr<<endl;
//    cout<<"     - border: (L:"<<opt.borderL<<" T:"<<opt.borderT<<" R:"<<opt.borderR<<" B:"<<opt.borderB<<")"<<endl;
//    cout<<"     - filtered image file: "<<((opt.filtered.empty())?"None":opt.filtered)<<endl;
//  }
//  if (opt.estimateCenterEdge)
//  {
//    cout<<" - center will be estimated automatically using the edge/Hough alg."<<endl;
//    cout<<"     - minimal image radius: "<<opt.minRadius<<endl;
//    cout<<"     - maximal image radius: "<<opt.maxRadius<<endl;
//    cout<<"     - ROI center: ("<< opt.edgeCX <<", "<< opt.edgeCY <<")"<<endl;
//    cout<<"     - ROI size: (+/-"<< opt.edgeDX <<", +/-"<< opt.edgeDY <<")"<<endl;
//    cout<<"     - border: (L:"<<opt.borderL<<" T:"<<opt.borderT<<" R:"<<opt.borderR<<" B:"<<opt.borderB<<")"<<endl;
//    cout<<"     - filtered image file: "<<((opt.filtered.empty())?"None":opt.filtered)<<endl;
//  }
//  if (opt.fixedCenter)
//  {
//    cout<<" - center: ("<< opt.cx <<", "<< opt.cy <<")"<<endl;
//  }
//  cout<<" - inner radius: "<<opt.ri<<endl;
//  cout<<" - outer radius: "<<opt.ro<<endl;
//  cout<<" - input image file: "<<opt.input<<endl;
//  cout<<" - test image file: "<<((opt.test.empty())?"None":opt.test)<<endl;
//  if (opt.unwrap)
//  {
//    cout<<" - output image file: "<<opt.output<<endl;
//    cout<<" - interpolation: "<<(opt.interpolation?"YES":"NO")<<
//                                (opt.bilinear?" (bilinear)":"")<<
//                                (opt.bicubic?" (bicubic)":"")<<endl;
//    cout<<" - scaling: ("<< opt.sx <<","<< opt.sy <<")"<<endl;
//  }
//  cout<<endl;
//}
//
//
//// ---------------------------------------
//int rnd(double d)
//{
//  return static_cast<int>(d+0.5);
//}
//int rndf(float d)
//{
//  return static_cast<int>(d+0.5);
//}
//
//
//// ---------------------------------------
//inline double linear(double v0, double v1, double x)
//{
//  return (v1-v0)*x+v0;
//}
//
//
//// ---------------------------------------
//double bilinear(double v00, double v01, double v10, double v11, double x, double y)
//{
//  // Notation: vXY
//  // Interpolate in X direction
//  double vX0=linear(v00, v10, x);
//  double vX1=linear(v01, v11, x);
//  // Interpolation in Y direction
//  return linear(vX0, vX1, y);
//}
//
//
//// ---------------------------------------
//inline double cubic(double v_1, double v0, double v1, double v2, double x)
//{
//  double a0, a1, a2, a3, x2;
//  x2 = x*x;
//  a0 = v2 - v1 - v_1 + v0;
//  a1 = v_1 - v0 - a0;
//  a2 = v1 - v_1;
//  a3 = v0;
//
//  return (a0*x*x2+a1*x2+a2*x+a3);
//}
//
//
//// ---------------------------------------
//double bicubic(double v_1_1, double v0_1, double v1_1, double v2_1,
//               double v_10, double v00, double v10, double v20,
//               double v_11, double v01, double v11, double v21,
//               double v_12, double v02, double v12, double v22,
//               double x, double y)
//{
//  // Notation: vXY
//  // Interpolate in X direction
//  double vX_1=cubic(v_1_1, v0_1, v1_1, v2_1, x);
//  double vX0 =cubic(v_10,  v00,  v10,  v20, x);
//  double vX1 =cubic(v_11,  v01,  v11,  v21, x);
//  double vX2 =cubic(v_12,  v02,  v12,  v22, x);
//  // Interpolation in Y direction
//  return cubic(vX_1, vX0, vX1, vX2, y);
//}
//
//
//// ---------------------------------------
//unsigned char getInterpolation(const Options &opt, IplImage* inputImg, int channel, double x, double y)
//{
//  // Notation: vXY
//  // Get channel values for both interpolation types
//  int x0=static_cast<int>(floor(x));
//  int y0=static_cast<int>(floor(y));
//  double v00=static_cast<double>(*reinterpret_cast<unsigned char *>
//        (inputImg->imageData + y0*inputImg->widthStep+x0*3+channel));
//  double v01=static_cast<double>(*reinterpret_cast<unsigned char *>
//        (inputImg->imageData + (y0+1)*inputImg->widthStep+x0*3+channel));
//  double v10=static_cast<double>(*reinterpret_cast<unsigned char *>
//        (inputImg->imageData + y0*inputImg->widthStep+(x0+1)*3+channel));
//  double v11=static_cast<double>(*reinterpret_cast<unsigned char *>
//        (inputImg->imageData + (y0+1)*inputImg->widthStep+(x0+1)*3+channel));
//
//  // Interpolate
//  double interpResult=0;
//  if (opt.bilinear)
//  { // BILINEAR INTERPOLATION
//    interpResult = bilinear(v00, v01, v10, v11, x-static_cast<double>(x0), y-static_cast<double>(y0));
//  }
//  else if (opt.bicubic)
//  { // BICUBIC INTERPOLATION
//    // Get additional channel values
//    double v_1_1=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0-1)*inputImg->widthStep+(x0-1)*3+channel));
//    double v0_1=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0-1)*inputImg->widthStep+(x0)*3+channel));
//    double v1_1=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0-1)*inputImg->widthStep+(x0+1)*3+channel));
//    double v2_1=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0-1)*inputImg->widthStep+(x0+2)*3+channel));
//
//    double v_10=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0)*inputImg->widthStep+(x0-1)*3+channel));
//    double v20=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0)*inputImg->widthStep+(x0+2)*3+channel));
//
//    double v_11=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0+1)*inputImg->widthStep+(x0-1)*3+channel));
//    double v21=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0+1)*inputImg->widthStep+(x0+2)*3+channel));
//
//    double v_12=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0+2)*inputImg->widthStep+(x0-1)*3+channel));
//    double v02=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0+2)*inputImg->widthStep+(x0)*3+channel));
//    double v12=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0+2)*inputImg->widthStep+(x0+1)*3+channel));
//    double v22=static_cast<double>(*reinterpret_cast<unsigned char *>
//          (inputImg->imageData + (y0+2)*inputImg->widthStep+(x0+2)*3+channel));
//
//    // Perform interpolation
//    interpResult = bicubic(v_1_1, v0_1, v1_1, v2_1,
//                           v_10,  v00,  v10,  v20,
//                           v_11,  v01,  v11,  v21,
//                           v_12,  v02,  v12,  v22,
//                           x-static_cast<double>(x0), y-static_cast<double>(y0));
//  }
//
//  // Check result before conversion
//  if (interpResult<0)
//    interpResult=0;
//  if (interpResult>255)
//    interpResult=255;
//
//  return static_cast<unsigned char>(interpResult);
//}
//
//
//// ---------------------------------------
//void unwrap(const Options &opt, IplImage* inputImg, IplImage** outputImg)
//{
//  // Create the unwrap image
//  int uwWidth = static_cast<int>(ceil((opt.ro * 2.0 * PI)*opt.sx));
//  int uwHeight = static_cast<int>(ceil((opt.ro-opt.ri + 1)*opt.sy));
//  IplImage* unwrappedImg = cvCreateImage(cvSize(uwWidth, uwHeight), 8, 3);
//
//  // Perform unwrapping
//  for (int uwX=0; uwX<uwWidth; ++uwX)
//    for (int uwY=0; uwY<uwHeight; ++uwY)
//  {
//    // Convert polar to cartesian
//    double w=-static_cast<double>(uwX)*2.0*PI/static_cast<double>(uwWidth);
//    double r=static_cast<double>(opt.ri) + 
//             static_cast<double>(uwHeight-uwY)*static_cast<double>(opt.ro-opt.ri + 1)/static_cast<double>(uwHeight);
//    double iX=r*cos(w)+opt.cx;
//    double iY=r*sin(w)+opt.cy;
//
//    // Do safety check
//    if ((iX<1) || (iX>inputImg->width-2) || (iY<1) || (iY>inputImg->height-2))
//    {
//      *(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+0) = 0;
//      *(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+1) = 0;
//      *(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+2) = 0;
//    }
//    else // Tansform image data
//    {
//      if (opt.interpolation)
//      { // With interpolation
//        *reinterpret_cast<unsigned char *>(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+0) =
//            getInterpolation(opt, inputImg, 0, iX, iY);
//        *reinterpret_cast<unsigned char *>(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+1) =
//            getInterpolation(opt, inputImg, 1, iX, iY);
//        *reinterpret_cast<unsigned char *>(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+2) =
//            getInterpolation(opt, inputImg, 2, iX, iY);
//      }
//      else 
//      { // No interpolation
//        int tmpX=rnd(iX);
//        int tmpY=rnd(iY);
//        *(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+0) =
//          *(inputImg->imageData + tmpY*inputImg->widthStep+tmpX*3+0);
//        *(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+1) =
//          *(inputImg->imageData + tmpY*inputImg->widthStep+tmpX*3+1);
//        *(unwrappedImg->imageData + uwY*unwrappedImg->widthStep+uwX*3+2) =
//          *(inputImg->imageData + tmpY*inputImg->widthStep+tmpX*3+2);
//      } // if
//    } // if
//  } // for
//
//  // Return the unwrapped image
//  (*outputImg) = unwrappedImg;
//}
//
//
//// ---------------------------------------
//void evaluateCenterThreshold(const Options &opt, IplImage* thrImg, int cx, int cy, double accuracy, double *var, double*mean)
//{
//  int iWidth = thrImg->width;
//  int iHeight = thrImg->height;
//
//  int outPointsSum=0;
//  int outPointsSum2=0;
//  int count=0;
//  for (double w=0; w<2*PI; w+=accuracy)
//  {
//    double r=opt.ro;
//    double iX=r*cos(w)+cx;
//    double iY=r*sin(w)+cy;
//    int outPoints=0;
//    while ((iX>=0) && (iX<iWidth) && (iY>=0) && (iY<iHeight) )//&& r<opt.ro+60
//    {
//      if ((*reinterpret_cast<unsigned char *>(thrImg->imageData + rnd(iY)*thrImg->widthStep+rnd(iX)))>0)
//        outPoints++;
//
//      // Increment r;
//      r+=1;
//      iX=r*cos(w)+cx;
//      iY=r*sin(w)+cy;
//    }
//
//    outPointsSum+=outPoints;
//    outPointsSum2+=(outPoints*outPoints);
//    count++;
//  }
//
//  // Calculate variance
//  *mean = static_cast<double>(outPointsSum)/count;
//  double mean2 = static_cast<double>(outPointsSum2)/count;
//  *var = mean2 - (*mean) * (*mean);
//}
//
//
//// ---------------------------------------
//void estimateCenterThreshold(const Options &opt, IplImage* inputImg, int *cx, int *cy)
//{
//  int uwWidth = static_cast<int>(ceil((opt.ro * 2.0 * PI)*opt.sx));
//  int iWidth = inputImg->width;
//  int iHeight = inputImg->height;
//
//  // Create thresholded image
//  IplImage* thrImg = cvCreateImage(cvSize(iWidth, iHeight), 8, 1);
//  for (int x=0; x<iWidth; ++x)
//    for (int y=0; y<iHeight; ++y)
//  {
//    double intensity;
//    if ((x<opt.borderL) || (y<opt.borderT) || (x>=iWidth-opt.borderR) || (y>=iHeight-opt.borderB))
//      intensity=0;
//    else
//    {
//      intensity = *reinterpret_cast<unsigned char *>(inputImg->imageData + y*inputImg->widthStep+x*3+0)+
//                  *reinterpret_cast<unsigned char *>(inputImg->imageData + y*inputImg->widthStep+x*3+1)+
//                  *reinterpret_cast<unsigned char *>(inputImg->imageData + y*inputImg->widthStep+x*3+2);
//      intensity/=3.0;
//    }
//    if (intensity<opt.blackThr)
//      *(thrImg->imageData + y*thrImg->widthStep+x)=0;
//    else
//      *(thrImg->imageData + y*thrImg->widthStep+x)=255;
//  }
//
//  // Save thresholding results
//  if (!opt.filtered.empty())
//    cvSaveImage(opt.filtered.c_str(),thrImg);
//
//  // Find center
//  double minVar=10000000;
//  double minMean=10000000;
//  double x1=opt.ro/2;
//  double x2=iWidth-opt.ro/2;
//  double y1=opt.ro/2;
//  double y2=iHeight-opt.ro/2;
//  int minX=rnd(x1), minY=rnd(y1);
//  double accuX=static_cast<double>(iWidth-1)/10.0;
//  double accuY=static_cast<double>(iHeight-1)/10.0;
//
//  while ((rnd(accuX)>=1) || (rnd(accuY)>=1))
//  {
//
//    for (double y=y1; y<y2; y+=accuY)
//    {
//      for (double x=x1; x<x2; x+=accuX)
//      {
//        double var,mean;
//        int rndx = rnd(x);
//        int rndy = rnd(y);
//        evaluateCenterThreshold(opt, thrImg, rndx, rndy,  accuX/opt.ro , &var, &mean);
//        if ( (var<minVar) || ((var==minVar)&&(mean<minMean)) )
//        {
//          minX=rndx;
//          minY=rndy;
//          minVar=var;
//          minMean=mean;
//        }
//      }
//    }
//    x1=minX-accuX/2;
//    x2=minX+accuX/2;
//    y1=minY-accuY/2;
//    y2=minY+accuY/2;
//    if (x1<0) x1=0;
//    if (y1<0) y1=0;
//    if (x2>iWidth) x2=iWidth;
//    if (y2>iHeight) y2=iHeight;
//    accuX=(accuX-1)/2.0;
//    accuY=(accuY-1)/2.0;
//  }
//
//  cvReleaseImage(&thrImg);
//  *cx = minX;
//  *cy = minY;
//}
//
//
//// ---------------------------------------
//void estimateCenterEdges(const Options &opt, IplImage* inputImg, int *cx, int *cy)
//{
//  int iWidth = inputImg->width;
//  int iHeight = inputImg->height;
//
//  // Min and max radius durin search
//  int minRadius=228;
//  int maxRadius=240;
//
//  // Region of interest for the center
//  int centerCX = opt.edgeCX;
//  int centerCY = opt.edgeCY;
//  int centerDX = opt.edgeDX;
//  int centerDY = opt.edgeDY;
//  int centerL = centerCX-centerDX;
//  int centerR = centerCX+centerDX;
//  int centerT = centerCY-centerDY;
//  int centerB = centerCY+centerDY;
//
//  // Region of interest for all operations
//  int roiL = centerCX-centerDX-maxRadius-10;
//  int roiT = centerCY-centerDY-maxRadius-10;
//  int roiR = centerCX+centerDX+maxRadius+10;
//  int roiB = centerCY+centerDY+maxRadius+10;
//  if (roiL<opt.borderL) roiL=opt.borderL;
//  if (roiT<opt.borderT) roiT=opt.borderT;
//  if (roiR>iWidth-1-opt.borderR) roiR=iWidth-1-opt.borderR;
//  if (roiB>iHeight-1-opt.borderB) roiB=iHeight-1-opt.borderB;
//  CvRect roi = cvRect(roiL, roiT, roiR-roiL+1, roiB-roiT+1);
//
//  // Convert image to gray scale
//  IplImage* grayImg = cvCreateImage(cvSize(iWidth, iHeight), IPL_DEPTH_32F, 1);
//  for (int x=roiL; x<=roiR; ++x)
//    for (int y=roiT; y<=roiB; ++y)
//  {
//    float intensity = *reinterpret_cast<unsigned char *>(inputImg->imageData + y*inputImg->widthStep+x*3+0)+
//                       *reinterpret_cast<unsigned char *>(inputImg->imageData + y*inputImg->widthStep+x*3+1)+
//                       *reinterpret_cast<unsigned char *>(inputImg->imageData + y*inputImg->widthStep+x*3+2);
//    intensity/=3.0;
//    cvSetReal2D(grayImg,y,x,logf(intensity+1));
//  }
//
//  // Filter the image
//  cvSetImageROI(grayImg, roi);
//  IplImage* filtXImg = cvCreateImage(cvSize(iWidth, iHeight), IPL_DEPTH_32F, 1);
//  cvSetImageROI(filtXImg, roi);
//  IplImage* filtYImg = cvCreateImage(cvSize(iWidth, iHeight), IPL_DEPTH_32F, 1);
//  cvSetImageROI(filtYImg, roi);
//  cvSobel(grayImg, filtXImg, 1, 0, 3);
//  cvSobel(grayImg, filtYImg, 0, 1, 3);
//  cvResetImageROI(filtXImg);
//  cvResetImageROI(filtYImg);
//
//  // Release gray image
//  cvReleaseImage(&grayImg);
//
//  // Get edges
//  float **edgeImg = new float*[iHeight];
//  for (int y=roiT; y<=roiB; ++y)
//  {
//    edgeImg[y] = new float[iWidth];
//    float *row = edgeImg[y];
//    for (int x=roiL; x<=roiR; ++x)
//    {
//      float eX=cvGetReal2D(filtXImg,y,x);
//      float eY=cvGetReal2D(filtYImg,y,x);
//      row[x] = logf(eX*eX+eY*eY+1.0);
//      //if (row[x]<4) row[x]=0; else row[x]=1;
//    }
//  }
//
//
//
//  // Release filtered images
//  cvReleaseImage(&filtXImg);
//  cvReleaseImage(&filtYImg);
//
//  // Do Hough transform and find max
//  float wInc = 1.0/maxRadius;
//  double max=-1000000000;
//  int maxX, maxY;
//
//  for (int x=centerL; x<=centerR; ++x)
//    for (int y=centerT; y<=centerB; ++y)
//    {
//      // Evaluate the center
//      double sum=0.0;
//      for (float w=0; w<2*PI; w+=wInc)
//      {
//        float tmpX=cosf(w);
//        float tmpY=sinf(w);
//        for (float r=minRadius; r<maxRadius; r+=1)
//        {
//          int iX = rndf(tmpX*r+x);
//          int iY = rndf(tmpY*r+y);
//          if ((iX>=roiL) && (iX<=roiR) && (iY>=roiT) && (iY<=roiB))
//            sum+=edgeImg[rnd(iY)][rnd(iX)];
//        }
//      }
//      // Find max
//      if (sum>max)
//      {
//        max=sum;
//        maxX=x;
//        maxY=y;
//      }
//    }
//
//  // Save edge image
//  if (!opt.filtered.empty())
//  {
//    float edgeMin = 1000000000;
//    float edgeMax = -1000000000;
//    for (int x=roiL; x<=roiR; ++x)
//      for (int y=roiT; y<=roiB; ++y)
//    {
//      float tmp=edgeImg[y][x];
//      if (tmp>edgeMax) edgeMax=tmp;
//      if (tmp<edgeMin) edgeMin=tmp;
//    }
//    IplImage* saveImg = cvCreateImage(cvSize(iWidth, iHeight), 8, 1);
//    for (int x=roiL; x<=roiR; ++x)
//      for (int y=roiT; y<=roiB; ++y)
//    {
//      *reinterpret_cast<unsigned char*>(saveImg->imageData + y*saveImg->widthStep+x) = 
//          rnd(255.0*(edgeImg[y][x]-edgeMin)/(edgeMax-edgeMin));
//    }
//    // Draw center point
//    int lineDiff = inputImg->width/100;
//    CvScalar color=CV_RGB(255, 255, 255);
//    cvLine(saveImg, cvPoint(maxX-lineDiff, maxY-lineDiff), 
//           cvPoint(maxX+lineDiff, maxY+lineDiff) , color, 1);
//    cvLine(saveImg, cvPoint(maxX+lineDiff, maxY-lineDiff), 
//           cvPoint(maxX-lineDiff, maxY+lineDiff) , color, 1);
//    // Draw outer radius
//    cvCircle(saveImg, cvPoint(maxX, maxY), opt.ro, color, 1);
//    // Save
//    cvSaveImage(opt.filtered.c_str(),saveImg);
//    cvReleaseImage(&saveImg);
//  }
//
//  // Release memory
//  for (int y=roiT; y<=roiB; ++y)
//    delete [] edgeImg[y];
//  delete edgeImg;
//
//  // Return results
//  *cx=maxX;
//  *cy=maxY;
//}
//
//// ---------------------------------------
//void createTestImage(const Options &opt, IplImage* inputImg)
//{
//  int lineDiff = inputImg->width/100;
//  CvScalar color=CV_RGB(255, 255, 0);
//
//  // Draw center point
//  cvLine(inputImg, cvPoint(opt.cx-lineDiff, opt.cy-lineDiff), 
//         cvPoint(opt.cx+lineDiff, opt.cy+lineDiff) , color, 2);
//  cvLine(inputImg, cvPoint(opt.cx+lineDiff, opt.cy-lineDiff), 
//         cvPoint(opt.cx-lineDiff, opt.cy+lineDiff) , color, 2);
//
//  // Draw inner radius
//  cvCircle(inputImg, cvPoint(opt.cx, opt.cy), opt.ri, color, 2);
//
//  // Draw outer radius
//  cvCircle(inputImg, cvPoint(opt.cx, opt.cy), opt.ro, color, 2);
//
//  // Save test image
//  if (!opt.test.empty())
//    cvSaveImage(opt.test.c_str(),inputImg);
//}
//
//
//// ---------------------------------------
//int main(int argc, char *argv[])
//{
//  // Print header and check command line args
//  printHeader();
//  Options opt;
//  if (!parseCommandLineArgs(argc, argv, &opt))
//  {
//    printHelp();
//    exit(-1);
//  }
//  printOptions(opt);
//
//  // Load image
//  cout<<"Loading input image... "<<flush;
//  IplImage* inputImg = cvLoadImage(opt.input.c_str());
//  if (!inputImg)
//  {
//    cout<<endl;
//    cerr<<"Error: Cannot load input image file "<<opt.input<<"."<<endl;
//    exit(-1);
//  }
//  if (opt.cx<0)
//    opt.cx=inputImg->width/2;
//  if (opt.cy<0)
//    opt.cy=inputImg->height/2;
//  cout<<"Done!"<<endl;
//
//  if ((opt.estimateCenterThreshold) || (opt.estimateCenterEdge))
//  {
//    cout<<"Estimating center... "<<flush;
//    int newCX, newCY;
//    if (opt.estimateCenterThreshold)
//      estimateCenterThreshold(opt, inputImg, &newCX, &newCY);
//    else if (opt.estimateCenterEdge)
//      estimateCenterEdges(opt, inputImg, &newCX, &newCY);
//    cout<<"Found ("<< newCX <<","<< newCY <<")!"<<endl;
//    opt.cy=newCY;
//    opt.cx=newCX;
//  }
//
//  // Unwrap and save result
//  IplImage* outputImg=0;
//  if (opt.unwrap)
//  {
//    // Unwrap
//    cout<<"Unwrapping... "<<flush;
//    unwrap(opt, inputImg, &outputImg);
//    cout<<"Done!"<<endl;
//
//    // Save image
//    cout<<"Writing output image... "<<flush;
//    if (!cvSaveImage(opt.output.c_str(),outputImg))
//    {
//      cout<<endl;
//      cerr<<"Error: Cannot write output image file "<<opt.output<<"."<<endl;
//      exit(-1);
//    }
//    cout<<"Done!"<<endl;
//  }
//
//  // Create test image
//  if (opt.createTest)
//  {
//    cout<<"Generating test image... "<<flush;
//    createTestImage(opt, inputImg);
//    cout<<"Done!"<<endl;
//  }
//
//  // Release memory
//  cout<<"Releasing memory... "<<flush;
//  if (outputImg)
//    cvReleaseImage(&outputImg);
//  cvReleaseImage(&inputImg);
//  cout<<"Done!"<<endl;
//
//  exit(0);
//}
//
