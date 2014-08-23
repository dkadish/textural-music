#!/usr/bin/env python
from freenect import sync_get_depth as get_depth, sync_get_video as get_video
import cv2 as cv
import numpy as np

from measurements import Motion, Foreground

prev = None

def show(img):
    # Build a two panel color image
    d3 = np.dstack((img,img,img)).astype(np.uint8)
    
    # Simple Downsample
    cv.imshow('Depth', d3)

    return cv.waitKey(5) % 256
        
def doloop():
    global depth, rgb, prev
    motion = None
    while True:
        # Get a fresh frame
        (depth,_), (rgb,_) = get_depth(), get_video()

        if motion == None:
            motion = Foreground(depth)
        
        motion.add(depth)
        
        # Show
        k = show(motion.image)
        if k == 27:
            break
        elif k == ord('u'):
            motion.update()
        elif k == ord('r'):
            motion.reset()
            
        # Serial
        

doloop()
