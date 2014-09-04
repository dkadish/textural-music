'''
'''

import numpy as np
from skimage.filter.rank import median
from skimage.morphology import white_tophat
from skimage.morphology import disk, erosion, black_tophat

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
        self._background = img.copy()
        self._background = self._background - 0.1 * self._background
        self.image = img.copy()
        
        self.count = 1.0
        self.structuring_element = disk(5)
    
    def add(self, img):
        self.last_img = img
        #self.image = img
        self.image = np.ones(img.shape) * 2047
        #self.image[img < self.background] = img[img < self.background]
        
        spots = np.ones(img.shape)
        spots[img >= self.background] = 0
        spots = spots - white_tophat(np.uint8(spots), self.structuring_element)
        self.image[spots == 1] = img[spots == 1]
        
        self.fg = spots
        #self.image = self.image #- black_tophat(np.uint16(self.image), self.structuring_element)

    def reset(self):
        self._background = self.last_img.copy()
        self._background = self._background - 0.1 * self._background
        self.count = 1.0
        
    def update(self):
        self.count += 1.0
        self._background = self._background * ((self.count-1.0)/self.count) + self.last_img.copy() / self.count
        #FIXME: self._background = self._background - 0.1 * self._background
        
        print np.average(self._background)
        
    @property
    def background(self):
        #return self._background - white_tophat(self._background, self.structuring_element);
        return self._background
