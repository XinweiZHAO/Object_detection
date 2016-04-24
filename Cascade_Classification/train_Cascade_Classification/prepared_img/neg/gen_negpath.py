import time
import numpy as np
i=1
f = open(r"C:\Users\XinweiZHAO\Desktop\neg.dat", 'w')  
while i in range(1,801): # 1 ~ number of images + 1
	#change to the real neg folder
    f.write("...\Cascade_Classification\prepared_img\\neg\\" + str(i) + ".bmp" + "\n")
    i+=1
f.close()