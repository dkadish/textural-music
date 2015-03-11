# -*- coding: utf-8 -*-
"""
Created on Tue Mar 10 12:37:28 2015

@author: dkadish
"""

# vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2

import serial, time, joblib, hashlib, numpy as np
from datetime import datetime

def millis():
  return int(time.time() * 1000)

def play_values(port, fname, pin=13):
	ser = serial.Serial(port, 9600)
	
 	player = Player()
	
	while player.play_from_pchip(fname):
	    ser.write('PWM %d %d\n' %(pin, player.value))
	    print 'PWM %d %d\n' %(pin, player.value)
	    time.sleep(0.01)     
     
	ser.write('PWM %d %d\n' %(pin, 0))
	
class Player(object):
     
     def play_from_pchip(self, pchip_filename):
            try:
                pchip = getattr(self, hashlib.md5(pchip_filename).hexdigest())
            except AttributeError:
                setattr(self, hashlib.md5(pchip_filename).hexdigest(),
                        joblib.load(pchip_filename))
                pchip = getattr(self, hashlib.md5(pchip_filename).hexdigest())
                        
            try:
                value = pchip(milliseconds(self.start_time), extrapolate=False)
                if np.isnan(value):
                    del self.start_time
                    return False
            except AttributeError:
                self.start_time = datetime.now()
                value = pchip(milliseconds(self.start_time), extrapolate=False)
                while np.isnan(value):
                    value = pchip(milliseconds(self.start_time), extrapolate=False)
            
            self.value = int(value)
            
            return True

def milliseconds(since):
    td = datetime.now() - since
    return int(td.total_seconds() * 1000)

if __name__ == '__main__':
	import argparse

	parser = argparse.ArgumentParser(description='Save values output by Arduino.')
	parser.add_argument('port')
	parser.add_argument('fname')
	parser.add_argument('pin', type=int)

	args = parser.parse_args()

	play_values(args.port, args.fname, args.pin)