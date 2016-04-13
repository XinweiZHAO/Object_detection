#Objection detection Repository
## 1. SURF(Speeded-Up Robust Features)
SURF based object detection

This program is tested in OpenCV 2.4.9, an object image is needed before running this code

	Step1 : detect all the scale invariant feature/interesting points for the prepared image
	Step2 : build up the descriptors for these points
	Step3 : use FLANN find good matching for these feature points according to the descriptors
	Step4 : perspectiveTransform the points in the prepared image to the current screne
	Step5 : draw matches and the detected object(boudary corners and the center)
