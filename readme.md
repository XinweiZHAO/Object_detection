#Object detection Repository
## 1. SURF(Speeded-Up Robust Features)
**SURF based object detection**

###1.1 /SURF
This folder contains some codes which are written and tested in OpenCV 2.4.9, an object image is needed before running this code

<img src="Object_Found.jpg" width="427" height="160" border="20" alt="">

    Step1 : detect all the scale invariant feature/interesting points for the prepared image
	Step2 : build up the descriptors for these points
	Step3 : use BFMatcher/FlannBasedMatcher find good matching for these feature points according to the descriptors
	Step4 : perspectiveTransform the points in the prepared image to the current screne
	Step5 : draw matches and the detected object(boudary corners and the center)
###1.2 /SURF_OpenCV3.1
These are also the SURF based object detection codes, however, it is based on the Opencv3.1 and [opencv_contrib](https://github.com/Itseez/opencv_contrib), considering the SURF now is moved out to opencv_contrib.

**Method 1**: use `knnMatch()` to find `std::vector<std::vector<DMatch> >& matches` , and if `matches[i][0].distance < 0.6*(matches[i][1].distance))`, then this match is added to `vector<DMatch> good_matches`
 
**Method 2**: use `std::sort(matches.begin(), matches.end())` to sort the matches and calculate the minimum distance by `double minDist = matches.front().distance`, then select the matches meeting `matches[i].distance < (1.2~1.6) * minDist` as good_matches

Both methods set a `thresholdGoodMatches` for the number of good_matches, if larger then 20~30, then we think the object is found. These parameters can be modified for perticular circumstance.

**Something should be paid attention to**:

	1. current captured screne frame may not always exist keypoints, for example the camera is blocked.

```cpp
if (!obj_descritor.empty() && !screne_descritor.empty())
{
	//details
}
```

    2. the Homography Matrix may not always be found 

```cpp
Mat H = findHomography(obj_SURFkp, scene_SURFkp, RANSAC);
if (!H.empty())//Homography Matrix may not be found
{
	//details
}
```


 
