//===============================================================
//File name : SURFdetector.h
//     Usage : head file for Object detection by SURF
//     Create Date : 2016.4.22
//OpenCV Version 3.1
//Author : XinweiZHAO
#ifndef _SURF_Object_detector_
#define _SURF_Object_detector_
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
/*include the OpenCV3.1 and opencv_contrib Framework*/
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

class SURF_Object_detector
{
public:
	SURF_Object_detector();
	~SURF_Object_detector();
	Point2f SURF_Objdetector(UMat capture, bool detected_flag);
	int ObjimgSURF();
	Mat drawGoodMatches(const Mat& img1,
		const Mat& img2,
		const std::vector<KeyPoint>& keypoints1,
		const std::vector<KeyPoint>& keypoints2,
		std::vector<vector<DMatch>>& matches,
		std::vector<Point2f>& scene_corners_,
		Mat	screne_descritor,
		bool detected_flag
		);
	UMat _loadobjetimage;//object image
	Mat loadobjetimage;

private:
	/******private valuables******/
	bool timecount;
	bool testing;//use to do performance analysis
	bool Info_out;//whether to printout the detection information
	double SURF_detect_time; //timing variable

	/*==============object information==============*/
	//object keypoits, descriptor
	vector<KeyPoint> obj_kpoints;
	UMat _obj_descritor;
	Mat obj_descritor;

	//SURF detector and descriptor
	struct SURFDetector
	{
		Ptr<Feature2D> surf;
		SURFDetector(double hessian = 800.0)
		{
			surf = SURF::create(hessian);
		}
		template<class T>
		void operator()(const T& in, const T& mask, std::vector<cv::KeyPoint>& pts, T& descriptors, bool useProvided = false)
		{
			surf->detectAndCompute(in, mask, pts, descriptors, useProvided);
		}
	};

	/*=============keypoints matcher===========*/
	/*DescriptorMatcher:Abstract base class for matching keypoint descriptors.
	* FlannBasedMatcher -> class FlannBasedMatcher : public DescriptorMatcher
	* BFMatcher -> brief Brute-force descriptor matcher
	*/
	template<class KPMatcher>
	struct SURFMatcher
	{
		KPMatcher matcher;
		template<class T>
		void match(const T& in1, const T& in2, std::vector<std::vector<DMatch> >& matches, int k)
		{
			matcher.knnMatch(in1, in2, matches, k);
		}
	};
	const int GOOD_PTS_MAX = 50;//maximum detected points
	const float GOOD_PORTION = 0.01f;//select 10 percent good matches
	//instantiate detectors/matchers
	SURFDetector surf;
	SURFMatcher<BFMatcher> matcher;
	//SURFMatcher<FlannBasedMatcher> matcher;
	Mat H;//homography matrix
	Mat img_matches;//Matching results

	/*keypoints Matching parameters*/
	//float matchdistance;//use to define a good match
	float thresholdMatchingNN;
	unsigned int thresholdGoodMatches;
	//Object corner points for plotting box
	vector<Point2f> obj_corners;
	/*output the founded object center*/
	Point2f obj_center;

	/******private functions******/
	void help();
};

#endif