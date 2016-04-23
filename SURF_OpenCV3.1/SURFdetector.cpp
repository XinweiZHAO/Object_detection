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
	obj_corners = vector<Point2f>(4);
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
	Mat loadobjetimage = _loadobjetimage.getMat(ACCESS_RW);
	if (!loadobjetimage.data){
		cout << "Can't open image";
		return -1;
	}
	/*Detects keypoints in an image
	* save the keypoints -> kpObject
	* Computes the descriptors for a set of keypoints detected in an image
	* save the descriptor -> desObject
	*/
	obj_descritor = _obj_descritor.getMat(ACCESS_RW);
	surf(_loadobjetimage.getMat(ACCESS_READ), Mat(), obj_kpoints, obj_descritor);

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
Point2f SURF_Object_detector::SURF_Objdetector(UMat capture, bool detected_flag)
{
	//class Dmatch: Class for matching keypoint descriptors
	vector<DMatch> matches;//many matchs finded
	vector<Point2f> obj_SURFkp;//final object's good points, use to find homography matrix
	vector<Point2f> scene_SURFkp;//final current frame's good points, use to find homography matrix
	vector<Point2f> scene_corners(4);
	/*==============screne information==============*/
	//captured screne keypoits and descriptor
	vector<KeyPoint> screne_kpoints;
	UMat _screne_descritor;
	Mat	screne_descritor = _screne_descritor.getMat(ACCESS_RW);

	if (timecount)//COUNT TIME	
	{
		SURF_detect_time = (double)getTickCount();
	}
	UMat GRAY_screne;
	cvtColor(capture, GRAY_screne, CV_RGB2GRAY);
	//detect current screne feature points and build up descriptor
	surf(GRAY_screne.getMat(ACCESS_READ), Mat(), screne_kpoints, screne_descritor);

	/* k=2 -> Count of best matches found per each query descriptor or less if a
	* query descriptor has less than k possible matches in total.*/
	Mat img_matches;
	if (!obj_descritor.empty() && !screne_descritor.empty())//current screne may no detected keypoints
	{
		matcher.match(obj_descritor, screne_descritor, matches);
		std::vector<Point2f> detected_corners;
		img_matches = drawGoodMatches(_loadobjetimage.getMat(ACCESS_READ), GRAY_screne.getMat(ACCESS_READ), obj_kpoints, screne_kpoints, matches, detected_corners, detected_flag);
		//show the SURF maching results
		imshow("Good Matches", img_matches);
	}
	else
	{
		img_matches = Mat::ones(480, 640, CV_8U);
		putText(img_matches, "Object Found", Point2f(10, 50), FONT_HERSHEY_COMPLEX_SMALL, 2, Scalar(0, 0, 250), 1, CV_AA);
		imshow("Good Matches", img_matches);
	}
	//print out each loop time
	if (timecount)
	{
		SURF_detect_time = 1000.*((double)getTickCount() - SURF_detect_time) / (double)getTickFrequency();
		printf("       detection time = %g ms\n", SURF_detect_time);
	}
	
	return obj_center;

}
/* *****************************************************************
%  Function name: drawGoodMatches
%  Usage : find good matches and return
%  input para:	img1: object image
%				img2: current screne image
%				keypoints1: detected object keypoints
%				keypoints2: detected current screne keypoints
%				matches: match the keypoints
%				scene_corners_:
%  output para:
%
%  update time: 2016.4.13
*********************************************************************/
Mat SURF_Object_detector::drawGoodMatches(const Mat& img1,
	const Mat& img2,
	const std::vector<KeyPoint>& keypoints1,
	const std::vector<KeyPoint>& keypoints2,
	std::vector<DMatch>& matches,
	std::vector<Point2f>& scene_corners_,
	bool detected_flag
	)
{

	//-- Sort matches and preserve top 10% matches
	std::sort(matches.begin(), matches.end());
	std::vector< DMatch > good_matches;
	double minDist = matches.front().distance;
	double maxDist = matches.back().distance;
	for (int i = 0; i < (int)matches.size(); i++)
	{
		if (matches[i].distance < 1.2 * minDist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	/*const int ptsPairs = std::max(2, (int)(matches.size() * GOOD_PORTION));
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches.push_back(matches[i]);
	}
	std::cout << "\nMax distance: " << maxDist << std::endl;
	std::cout << "Min distance: " << minDist << std::endl;

	std::cout << "Calculating homography using " << ptsPairs << " point pairs." << std::endl;*/

	//for (int i = 0; i < min(screne_descritor.rows - 1, (int)matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
	//{
	//	if ((matches[i][0].distance < thresholdMatchingNN*(matches[i][1].distance)) && ((int)matches[i].size() <= 2 && (int)matches[i].size()>0))
	//	{
	//		good_matches.push_back(matches[i][0]);//Adds elements to the bottom of the matrix
	//	}
	//}
	// drawing the results
	Mat img_matches;
	//Draw only "good" matches
	drawMatches(img1, keypoints1, img2, keypoints2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS/*DRAW_RICH_KEYPOINTS*/);

	//-- Localize the object
	std::vector<Point2f> obj_SURFkp;
	std::vector<Point2f> scene_SURFkp;
	if (good_matches.size() >= thresholdGoodMatches)
	{
		//object detected in current screne
		detected_flag = true;
		//Display that the object is found
		putText(img_matches, "Object Found", Point2f(10, 50), FONT_HERSHEY_COMPLEX_SMALL, 2, Scalar(0, 0, 250), 1, CV_AA);
		for (size_t i = 0; i < good_matches.size(); i++)
		{
			//-- Get the keypoints from the good matches
			obj_SURFkp.push_back(keypoints1[good_matches[i].queryIdx].pt);
			scene_SURFkp.push_back(keypoints2[good_matches[i].trainIdx].pt);
		}

		std::vector<Point2f> scene_corners(4);

		Mat H = findHomography(obj_SURFkp, scene_SURFkp, RANSAC);
		if (!H.empty())//Homography Matrix may not be found
		{
			perspectiveTransform(obj_corners, scene_corners, H);
			scene_corners_ = scene_corners;//output the screne corner
			//-- Draw lines between the corners (the mapped object in the scene - image_2 )
			line(img_matches,
				scene_corners[0] + Point2f((float)img1.cols, 0), scene_corners[1] + Point2f((float)img1.cols, 0),
				Scalar(0, 255, 0), 3, LINE_AA);
			line(img_matches,
				scene_corners[1] + Point2f((float)img1.cols, 0), scene_corners[2] + Point2f((float)img1.cols, 0),
				Scalar(0, 255, 0), 3, LINE_AA);
			line(img_matches,
				scene_corners[2] + Point2f((float)img1.cols, 0), scene_corners[3] + Point2f((float)img1.cols, 0),
				Scalar(0, 255, 0), 3, LINE_AA);
			line(img_matches,
				scene_corners[3] + Point2f((float)img1.cols, 0), scene_corners[0] + Point2f((float)img1.cols, 0),
				Scalar(0, 255, 0), 3, LINE_AA);
			//Draw object center points
			circle(img_matches, (scene_corners[0] + scene_corners[1] + scene_corners[2] + scene_corners[3])*0.25 + Point2f(img1.cols, 0), 8, Scalar(0, 0, 255), -1, 8, 0);
			//find the center points
			obj_center.x = (scene_corners[0].x + scene_corners[1].x + scene_corners[2].x + scene_corners[3].x) / 4;
			obj_center.y = (scene_corners[0].y + scene_corners[1].y + scene_corners[2].y + scene_corners[3].y) / 4;
			//print out the object corners and center point coordinate
			printf("Object corner: 00(%f, %f), 10(%f, %f)\n"
				"               11(%f, %f), 01(%f, %f)pixels \n"
				"       center:(%f,%f) pixels\n", scene_corners[0].x, scene_corners[0].y, scene_corners[1].x, scene_corners[1].y, \
				scene_corners[2].x, scene_corners[2].y, scene_corners[3].x, scene_corners[3].y, \
				obj_center.x, obj_center.y);
		}
	}
	else
	{
		//object not detected
		detected_flag = false;
		putText(img_matches, "Object Not Found", Point2f(10, 50), FONT_HERSHEY_COMPLEX_SMALL, 2, Scalar(0, 0, 250), 1, CV_AA);
	}
	return img_matches;
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
		"This program is tested in OpenCV 3.1\n"
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