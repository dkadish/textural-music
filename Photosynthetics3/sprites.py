#!/usr/bin/python
# coding: utf-8
import pygame, time, math

from signals import SineWave, SignalGroup

class PWMSprite(pygame.sprite.Sprite):
    
    def __init__(self, serial_connection, pin, period=30):
        pygame.sprite.Sprite.__init__(self)
        
        self.conn = serial_connection
        self.pin = pin
        self.value = 0
        
#        self.abs_range = (0, 255)
#        self.low_range = (0, 127)
#        self.signal = SineWave(period, 255)
                
        # Sprites could have a list of signals each one being more basic than the previous.
#        self.signals = [
#            SineWave(30*60, 255)
#        ]
        
    def update(self):
        self.value = self.signal.value(time.time())
        self.writeSerial()
        
    def writeSerial(self):
        #print '%s: PWM %d %d' %(self.conn, self.pin, int(self.value*255))
        self.conn.write("PWM %d %d\n" %(self.pin, int(self.value*255)))        
        
class PurpleBiLeaf(PWMSprite):

    def __init__(self, serial_connection, pin, short=False):
        super(PurpleBiLeaf, self).__init__(serial_connection, pin)
        
        if short:
            longLoop = SineWave(period = 30*60, v_range = (0.0, 1.0))
            midLoop = SineWave(period = 5*60, v_range = (-0.25, 1.0), offset = 20)
            shortLoop = SineWave(period = 60, v_range = (-0.5, 1.0), offset = 30)
        else:
            longLoop = SineWave(period = 30*60, v_range = (0.0, 1.0), offset = 30)
            midLoop = SineWave(period = 2*60, v_range = (-0.3, 1.0))
            shortLoop = SineWave(period = 1.5*60, v_range = (-0.5, 1.0), offset = 10)
        self.signal = SignalGroup([longLoop, midLoop, shortLoop], in_range=(0.0, 0.5))
    
class WhiteLeaf(PWMSprite):
    def __init__(self, serial_connection, pin):
        super(WhiteLeaf, self).__init__(serial_connection, pin)
        
        longLoop = SineWave(period = 24*60, v_range = (0.0, 1.0), offset = 4*60)
        midLoop = SineWave(period = 5*60, v_range = (-0.5, 1.0), offset = 60)
        shortLoop = SineWave(period = 4*60, v_range = (-0.5, 1.0), offset = 170)
        self.signal = SignalGroup([longLoop, midLoop, shortLoop], in_range=(-0.5, 1.0))

#    def update(self):
#        super(PWMSprite, self).update()
#        print '%s: PWM %d %d' %(self.conn, self.pin, int(self.value*255))
    
class WhiteBiLeaf(PWMSprite):
    def __init__(self, serial_connection, pin, short=False):
        super(WhiteBiLeaf, self).__init__(serial_connection, pin)
        
        if short:
            longLoop = SineWave(period = 16*60, v_range = (0.0, 1.0))
            midLoop = SineWave(period = 1*60, v_range = (-0.25, 1.0), offset = 12)
            shortLoop = SineWave(period = 42, v_range = (-0.5, 1.0), offset = 31)
        else:
            longLoop = SineWave(period = 16*60, v_range = (0.0, 1.0), offset = 47)
            midLoop = SineWave(period = 1.75*60, v_range = (-0.3, 1.0))
            shortLoop = SineWave(period = 34, v_range = (-0.5, 1.0), offset = 10)
        self.signal = SignalGroup([longLoop, midLoop, shortLoop], in_range=(0.0, 0.5))
    
class WhiteScrunchie(PWMSprite):
    def __init__(self, serial_connection, pin):
        super(WhiteScrunchie, self).__init__(serial_connection, pin)
        
        longLoop = SineWave(period = 43*60, v_range = (0.0, 1.0), offset = 1237)
        midLoop = SineWave(period = 3.2*60, v_range = (-0.2, 1.0), offset = 69)
        shortLoop = SineWave(period = 1.6*60, v_range = (-0.5, 1.0), offset = 2)
        self.signal = SignalGroup([longLoop, midLoop, shortLoop], in_range=(0.0, 0.5))

class Light(PWMSprite):
    def __init__(self, serial_connection, pin, longPeriod, longOffset, midPeriod, midOffset, maxVal=1.0):
        super(Light, self).__init__(serial_connection, pin)
        print serial_connection, pin, longPeriod, longOffset, midPeriod, midOffset, maxVal
        
        longLoop = SineWave(period = longPeriod, v_range = (-0.9, maxVal), offset = longOffset)
        midLoop = SineWave(period = midPeriod, v_range = (0.5, maxVal), offset = midOffset)
        self.signal = SignalGroup([longLoop, midLoop], in_range=(0.0,1.1))
        
class LightingGroup(pygame.sprite.Group):
    pass
    
class LeafGroup(pygame.sprite.Group):
    pass
