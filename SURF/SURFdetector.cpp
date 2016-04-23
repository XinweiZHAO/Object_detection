//===============================================================
//File name : SURFdetector.cpp
//     Usage : SURF based object detection
//     Create Date : 2016.4.13
//Author : XinweiZHAO

/*===============================================================
%					include some head files
===============================================================*/
#include "SURFdetector.h"

/*===============================================================
%					 define functions
===============================================================*/
/* *****************************************************************
%  Function name: SURF_Object_detector
%           Usage : class construct function
%  input para:
%  output para:
%
%  update time: 2016.4.13
*********************************************************************/
SURF_Object_detector::SURF_Object_detector()
{
	help();
	timecount = true;
	testing = false;
	minHess = 2000;
	detector = SurfFeatureDetector(minHess);
	obj_corners = vector<Point2f>(4);
	thresholdMatchingNN = 0.7;
	thresholdGoodMatches = 15;
	
}
SURF_Object_detector::~SURF_Object_detector()
{
	cout << "Detect Over";
}
/* *****************************************************************
%  Function name: Objimgload
%  Usage : use to load the object image
%  input para:
%
%  output para:
%
%  update time: 2016.4.13
*********************************************************************/
int SURF_Object_detector::ObjimgSURF()
{
	/*load training image
	* image path needed to be redefined
	*/
	//make sure image is loaded
	if (!loadobjetimage.data){
		cout << "Can't open image";
		return -1;
	}
	/*Detects keypoints in an image
	* save the keypoints -> kpObject
	*/
	detector.detect(loadobjetimage, obj_kpoints);
	/*Computes the descriptors for a set of keypoints detected in an image
	* save the descriptor -> desObject
	*/
	extractor.compute(loadobjetimage, obj_kpoints, obj_descritor);
	/*object four corners, use to transform to current screne*/
	obj_corners[0] = Point2f(0, 0);
	obj_corners[1] = Point2f(loadobjetimage.cols, 0);
	obj_corners[2] = Point2f(loadobjetimage.cols, loadobjetimage.rows);
	obj_corners[3] = Point2f(0, loadobjetimage.rows);
}
/* *****************************************************************
%  Function name: SURF_Objdetector
%  Usage : main function to detect object
%  input para:
%			 capture: captured image(detect oject from where)
%
%  output para:
%
%  update time: 2016.4.13
*********************************************************************/
Point2f SURF_Object_detector::SURF_Objdetector(Mat capture)
{
	//class Dmatch: Class for matching keypoint descriptors
	vector<vector<DMatch>> matches;//many matchs 
	vector<DMatch> good_matches;//good matchs
	vector<Point2f> obj_SURFkp;
	vector<Point2f> scene_SURFkp;
	vector<Point2f> scene_corners(4);
	/*==============screne information==============*/
	//captured screne keypoits and descriptor
	vector<KeyPoint> screne_kpoints;
	Mat screne_descritor;

	

	if (timecount)//COUNT TIME	
	{
		t = (double)getTickCount();
	}
	Mat GRAY_screne;
	cvtColor(capture, GRAY_screne, CV_RGB2GRAY);
	//detect current screne feature points and build up descriptor
	detector.detect(GRAY_screne, screne_kpoints);//detect keypoints
	extractor.compute(GRAY_screne, screne_kpoints, screne_descritor);//computer descriptor
	/* k=2 -> Count of best matches found per each query descriptor or less if a
	* query descriptor has less than k possible matches in total.*/
	matcher.knnMatch(obj_descritor, screne_descritor, matches, 2);

	for (int i = 0; i < min(screne_descritor.rows - 1, (int)matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
	{
		if ((matches[i][0].distance < thresholdMatchingNN*(matches[i][1].distance)) && ((int)matches[i].size() <= 2 && (int)matches[i].size()>0))
		{
			good_matches.push_back(matches[i][0]);//Adds elements to the bottom of the matrix
		}
	}

	//Draw only "good" matches
	drawMatches(loadobjetimage, obj_kpoints, GRAY_screne, screne_kpoints, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	obj_center.x = (scene_corners[0].x + scene_corners[1].x + scene_corners[2].x + scene_corners[3].x) / 4;
	obj_center.y = (scene_corners[0].y + scene_corners[1].y + scene_corners[2].y + scene_corners[3].y) / 4;
	
	if (good_matches.size() >= thresholdGoodMatches)
	{
		//Display that the object is found
		putText(img_matches, "Object Found", Point2f(10, 50), FONT_HERSHEY_COMPLEX_SMALL, 2, Scalar(0, 0, 250), 1, CV_AA);
		for (unsigned int i = 0; i < good_matches.size(); i++)
		{
			//Get the keypoints from the good matches
			obj_SURFkp.push_back(obj_kpoints[good_matches[i].queryIdx].pt);
			scene_SURFkp.push_back(screne_kpoints[good_matches[i].trainIdx].pt);
		}

		H = findHomography(obj_SURFkp, scene_SURFkp, CV_RANSAC);

		perspectiveTransform(obj_corners, scene_corners, H);

		//Draw lines between the corners (the mapped object in the scene image )
		line(img_matches, scene_corners[0] + Point2f(loadobjetimage.cols, 0), scene_corners[1] + Point2f(loadobjetimage.cols, 0), Scalar(0, 255, 0), 4);
		line(img_matches, scene_corners[1] + Point2f(loadobjetimage.cols, 0), scene_corners[2] + Point2f(loadobjetimage.cols, 0), Scalar(0, 255, 0), 4);
		line(img_matches, scene_corners[2] + Point2f(loadobjetimage.cols, 0), scene_corners[3] + Point2f(loadobjetimage.cols, 0), Scalar(0, 255, 0), 4);
		line(img_matches, scene_corners[3] + Point2f(loadobjetimage.cols, 0), scene_corners[0] + Point2f(loadobjetimage.cols, 0), Scalar(0, 255, 0), 4);
		//Draw object center points
		circle(img_matches, (scene_corners[0] + scene_corners[1] + scene_corners[2] + scene_corners[3])*0.25 + Point2f(loadobjetimage.cols, 0), 8, Scalar(0, 0, 255), -1, 8, 0);
		//print out the object corners and center point coordinate
		printf("Object corner: 00(%f, %f), 10(%f, %f)\n"
			"               11(%f, %f), 01(%f, %f)pixels \n"
			"       center:(%f,%f) pixels\n", scene_corners[0].x, scene_corners[0].y, scene_corners[1].x, scene_corners[1].y, \
			scene_corners[2].x, scene_corners[2].y, scene_corners[3].x, scene_corners[3].y, \
			obj_center.x, obj_center.y);
	}
	else
	{
		putText(img_matches, "Object Not Found", Point2f(10, 50), FONT_HERSHEY_COMPLEX_SMALL, 2, Scalar(0, 0, 250), 1, CV_AA);
	}

	//show the SURF maching results
	imshow("Good Matches", img_matches);
	//print out each loop time
	if (timecount)
	{
		t = 1000.*((double)getTickCount() - t) / (double)getTickFrequency();
		printf("       detection time = %g ms\n", t);
	}
	return obj_center;

}
/* *****************************************************************
%  Function name: help
%  Usage : show program information
%  input para:
%
%  output para:
%
%  update time: 2016.4.13
*********************************************************************/
void SURF_Object_detector::help()
{
	cout << "\nThis program is SURF based object detection\n"
		"This program is tested in OpenCV 2.4.9\n"
		"an objcvt image is needed before running this code.\n"
		"Step1 : detect all the scale invariant feature/interesting points\n"
		"Step2 : build up the descriptors for these points\n"
		"Step3 : do good matching for these feature points according to the descriptors\n"
		"Step4 : perspectiveTransform the points in the prepared image to the current screne\n"
		"Step5 : draw matches and the detected object(boudary corners and the center)\n"
		"Usage:\n"
		"\tUse key to escape the infinite loop -> Esc / q / Q\n"
		"OpenCV version " << CV_VERSION << "\n" << endl;
}