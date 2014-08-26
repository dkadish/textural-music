#!/usr/bin/env python
from freenect import sync_get_depth as get_depth, sync_get_video as get_video
import cv2 as cv
import numpy as np
import serial, time

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
    motion = None
    fg_max = [1, 1] #np.ones((3,3))
    motion_max = [1, 1] #np.ones((3,3))
    fg_min = [100, 100]
    motion_min = [100, 100]
    #fg_prev = np.zeros((3,3))
    #motion_prev = np.zeros((3,3))
    
    averages = [0, 0]
    light_periods = [2, 2]
    leaf_periods = [5, 5]

    #lights = [ # period, amplitude
    #    [10000, 75],
    #    [20000, 120]
    #]

    #leaves = [ # period, amplitude
    #    [20000, 75],
    #    [30000, 120]
    #]
    
    #MULTIPLIER = np.array([[ 51, 51, 51],
    #                       [102,102,102],
    #                       [153,153,153]])
    
    last_time = int(time.time())
    
    ser.write('ON\n')
    
    while True:
        # Get a fresh frame
        (depth,_), (rgb,_) = get_depth(), get_video()

        if foreground == None:
            foreground = Foreground(depth)
            
        if motion == None:
            motion = Motion(depth)
        
        foreground.add(depth)
        motion.add(depth)
        
        k = show(foreground.image)
        if k == 27:
            ser.write('OFF\n')
            break
        elif k == ord('s'):
            show = False
        elif k == ord('u'):
            foreground.update()
        elif k == ord('r'):
            foreground.reset()
            fg_max = [1, 1] #np.ones((3,3))
            motion_max = [1, 1] #np.ones((3,3))
            
        # Serial
        #fg_max[fg_max < fg_prev] = fg_prev[fg_max < fg_prev]
        #motion_max[motion_max < motion_prev] = motion_prev[motion_max < motion_prev]
        
        # divide the image into a 3x3 grid
        '''h, w = foreground.image.shape
        box_h, box_w = h/3, w/3
        for i in range(3):
            for j in range(3):
                fg_box = np.average(foreground.image[box_h*i:box_h*(i+1),box_w*j:box_w*(j+1)])
                motion_box = np.average(motion.image[box_h*i:box_h*(i+1),box_w*j:box_w*(j+1)])
                
                fg_prev[i,j] = fg_box
                motion_prev[i,j] = motion_box
        
        distances = np.sum(fg_prev/fg_max * MULTIPLIER, axis=0)
        motions = np.sum(motion_prev/motion_max * MULTIPLIER, axis=0)
        motions = (-motions * 30 / 256) + 31
        motions = motions * 1000 # to get to milliseconds'''
        
        h, w = foreground.image.shape
        box_h, box_w = h, w/2
        
        fg_box = [np.average(foreground.image[:,:box_w])]
        motion_box = [np.average(motion.image[:,:box_w])]
        fg_box.append(np.average(foreground.image[:,box_w:]))
        motion_box.append(np.average(motion.image[:,box_w:]))
        
        for i in range(2):
            if fg_box[i] > fg_max[i]: fg_max[i] = fg_box[i]
            if motion_box[i] > motion_max[i]: motion_max[i] = motion_box[i]
            if fg_box[i] < fg_min[i]: fg_min[i] = fg_box[i]
            if motion_box[i] < motion_min[i]: motion_min[i] = motion_box[i]
        
        '''print 
        print fg_box
        print motion_box
        print fg_max
        print motion_max'''
        
        fg = [0, 0]
        
        for i in range(2):
            #fg[i] = float(fg_box[i])/fg_max[i] # max is close, close is 1
            fg_box[i] = floor_ceil(1.0-((float(fg_box[i])-fg_min[i]*1.5)/(fg_max[i]*0.8-fg_min[i]*1.5))) # max is close, far is 1
            motion_box[i] = floor_ceil(1.0-(float(motion_box[i]-motion_min[i]*1.5)/(motion_max[i]*0.8-motion_min[i]*1.5))) # max is motion, 1 is motion
            
            
            
        # Related to fg
        # amplifications
        #amplifications = 
        averages = running_avg(averages, [fg_box[0]*30, fg_box[1]*30]) # Low when people are nearby, high when the are far
        
        # Related to motion
        light_periods = running_avg(light_periods, [(motion_box[0]*14)+1, (motion_box[1]*14)+1]) # between 2 and 20, high when low motion
        leaf_periods = running_avg(leaf_periods, [(fg_box[0]*10)+5, (fg_box[1]*10)+5])# between 5 and 40, related to presence, lower when closer
        
        if int(time.time()) != last_time:
            last_time = int(time.time())
            #print amplifications, averages, motions
            #for i,d in enumerate(amplifications):
            #    ser.write('AMP 0 %d %d 1000\n' %(i,d))
            #    
            #    print 'AMP 0 %d %d 1000\n' %(i,d)
                
            for i,d in enumerate(averages):
                ser.write('AVG 1 %d %d\n' %(i,d))
                #assert d >= 0, d
                #assert d <= 255, d
                print time.time(), 'AVG 1 %d %d' %(i,d)
                
            for i, m in enumerate(light_periods):
                ser.write('PER 1 %d %d\n' %(i,m*1000))
                #assert m >= 2, m
                #assert m <= 20, m
                print time.time(), 'PER 1 %d %d' %(i,m*1000)
                
            for i, m in enumerate(leaf_periods):
                ser.write('PER 0 %d %d\n' %(i,m*1000))
                #assert m >= 5, m
                #assert m <= 40, m
                print time.time(), 'PER 0 %d %d' %(i,m*1000)
                
            print
            
            #averages = [0, 0]
            #light_periods = [0, 0]
            #leaf_periods = [0, 0]
            
        
ser = serial.Serial('/dev/ttyACM0', 9600)
doloop()

'''
I need both a min and a max so that it gets the right range.
Also, I should do a cut off so that you see some action even when you are not quite at the extremes.
'''
