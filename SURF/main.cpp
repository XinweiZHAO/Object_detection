//===============================================================
//File name : main.cpp
//     Usage : Object detection by SURF
//     Create Date : 2016.4.13
//OpenCV Version 2.4.9
//Author : XinweiZHAO
/*===============================================================
%					include head files
===============================================================*/
#include "SURFdetector.h"

int main()
{
	SURF_Object_detector find_object_corord;
	find_object_corord.loadobjetimage = imread("MGeo.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	find_object_corord.ObjimgSURF();//clacluate the definded object feature points and buit up descriptor
	namedWindow("Good Matches", CV_WINDOW_AUTOSIZE);
	//Initialize video and display window
	VideoCapture cap(0);
	if (!cap.isOpened()){
		return -1;
	}
	char escapeKey = 'k';//initial key
	Mat frame;
	Point2f obj_center;
	for (;;)/*loop until happy*/
	{
		cap >> frame;
		try{
			obj_center = find_object_corord.SURF_Objdetector(frame);
		}
		catch (cv::Exception& e) {
			printf("exception: %s\n", e.what());
		}
		//escape the loop
		escapeKey = waitKey(10);
		if (escapeKey == 27 || escapeKey == 'q' || escapeKey == 'Q'){
			break;
		}
	}

}