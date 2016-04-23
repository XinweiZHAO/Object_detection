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
	bool detected_flag = false;//init detected flag
	SURF_Object_detector find_object_corord;
	imread("MGeo.bmp", CV_LOAD_IMAGE_GRAYSCALE).copyTo(find_object_corord._loadobjetimage);
	find_object_corord.ObjimgSURF();//clacluate the definded object feature points and buit up descriptor
	namedWindow("Good Matches", CV_WINDOW_AUTOSIZE);
	//Initialize video and display window
	VideoCapture cap(0);
	if (!cap.isOpened()){
		return -1;
	}
	char escapeKey = 'k';//initial key
	UMat frame;
	Point2f obj_center;
	for (;;)/*loop until happy*/
	{
		cap >> frame;
		obj_center = find_object_corord.SURF_Objdetector(frame, detected_flag);
		if (detected_flag)
		{
			//obj_center can be used
		}
		//escape the loop
		escapeKey = waitKey(10);
		if (escapeKey == 27 || escapeKey == 'q' || escapeKey == 'Q'){
			break;
		}
	}

}