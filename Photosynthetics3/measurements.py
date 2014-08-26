'''
'''

import numpy as np
from skimage.filter.rank import median
from skimage.morphology import disk


class Motion(object):
    
    def __init__(self, img):
        self.prev = np.int32(img.copy())
        self.image = np.int32(img.copy())
        
    def add(self, img):
        img = np.int32(img)
        self.image = np.abs(img - self.prev)
        self.prev = 0.9*self.prev + 0.1*img
        
class Foreground(object):
    
    def __init__(self, img):
        self.last_img = img.copy()
        self.background = img.copy()
        self.image = img.copy()
        
        self.count = 1.0
    
    def add(self, img):
        self.last_img = img
        self.image = np.maximum(self.last_img, self.background) - self.last_img

    def reset(self):
        self.background = self.last_img.copy()
        self.count = 1.0
        
    def update(self):
        self.count += 1.0
        self.background = self.background * ((self.count-1.0)/self.count) + self.last_img.copy() / self.count
        
        print np.average(self.background)
