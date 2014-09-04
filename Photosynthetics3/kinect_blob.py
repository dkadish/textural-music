#!/usr/bin/env python
from freenect import sync_get_depth as get_depth, sync_get_video as get_video
import cv2 as cv
import numpy as np
import serial, time
from skimage.feature import blob_doh
from SimpleCV.Features.BlobMaker import BlobMaker

from measurements import Foreground, Motion

prev = None
show = False

def show(img):
    # Build a two panel color image
    d3 = np.dstack((img,img,img)).astype(np.uint8)
    
    # Simple Downsample
    cv.imshow('Depth', d3)

    return cv.waitKey(5) % 256
    
def running_avg(orig, new, frac=0.9):
    return [orig[i]*frac + new[i]*(1.0-frac) for i in range(len(orig))]
    
def floor_ceil(num):
    if num < 0.0: return 0.0
    elif num > 1.0: return 1.0
    
    return num
        
def doloop():
    global depth, rgb, prev, show
    
    times = [True, True, True]
    foreground = None
    doblobs = False
    blobs = BlobMaker()
    
    while True:
        # Get a fresh frame
        (depth,_), (rgb,_) = get_depth(), get_video()

        if foreground == None:
            foreground = Foreground(depth)
            
        foreground.add(depth)
        
        img = foreground.image.copy()
        
        if doblobs:
            print foreground.fg.dtype
            blobs = blob_doh(foreground.fg, min_sigma=30, max_sigma=100, threshold=.05)
            for blob in blobs:
                y, x, r = blob
                cv.circle(img, (x,y), r, (1,0,0), thickness=2)
            #b = blobs.extract(img)
            #print b, blobs
        
        k = show(img)
        if k == 27:
            break
        elif k == ord('b'):
            doblobs = True
        elif k == ord('s'):
            foreground.image
        elif k == ord('u'):
            foreground.update()
        elif k == ord('r'):
            foreground.reset()
            
doloop()

'''
I need both a min and a max so that it gets the right range.
Also, I should do a cut off so that you see some action even when you are not quite at the extremes.
'''
