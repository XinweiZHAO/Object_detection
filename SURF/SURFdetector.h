//===============================================================
//File name : SURFdetector.h
//     Usage : head file for Object detection by SURF
//     Create Date : 2016.4.13
//OpenCV Version 2.4.9
//Author : XinweiZHAO
#ifndef _SURF_Object_detector_
#define _SURF_Object_detector_
/*include the OpenCV2.4.9 Framework*/
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
using namespace cv;
using namespace std;
class SURF_Object_detector
{
public:
	SURF_Object_detector();
	~SURF_Object_detector();
	Point2f SURF_Objdetector(Mat capture, bool & detected_flag);
	int ObjimgSURF();
	Mat loadobjetimage;

private:
	/******private valuables******/
	bool timecount;
	//use to do performance analysis
	bool testing;
	bool Info_out;//whether to printout the detection information
	double t; //timing variable
	//SURF Detector, and descriptor parameters, match object initialization
	int minHess;//minHess->Threshold for hessian keypoint detector used in SURF
	
	/*==============object information==============*/
	//object image, keypoits, descriptor
	vector<KeyPoint> obj_kpoints;
	Mat obj_descritor;
	//Object corner points for plotting box
	vector<Point2f> obj_corners;
	//SURF detector and descriptor
	SurfFeatureDetector detector;
	SurfDescriptorExtractor extractor;

	/*=============keypoints matcher===========*/
	/*DescriptorMatcher:Abstract base class for matching keypoint descriptors.
	* FlannBasedMatcher-> class FlannBasedMatcher : public DescriptorMatcher
	*/
	FlannBasedMatcher matcher;//Fast Approximate Nearest Neighbor Search

	/*keypoints Macthing parameters*/
	float thresholdMatchingNN;
	unsigned int thresholdGoodMatches;
	/*return object center*/
	Point2f obj_center;
	/******private functions******/
	void help();
};

#endif