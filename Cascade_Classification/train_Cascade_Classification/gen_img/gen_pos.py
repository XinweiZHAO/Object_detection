import cv2
import time
import numpy as np
i=1
# path for store images 
path="C:\Users\XinweiZHAO\Desktop\calculator\\"
# create a VideoCapture object. Camera-0
cap = cv2.VideoCapture(0)
while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    l=str(i)+".bmp" # image name
    # color conversion BGR - Gray 
    # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # Display the resulting frame
    cv2.imshow('frame', frame)
    # save images wait for the key 's' 
    if cv2.waitKey(1) & 0xFF == ord('s'):
        cv2.imwrite(path+l, frame)
        i+=1
    # quit 
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()