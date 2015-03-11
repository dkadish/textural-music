# vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2

import serial, time, joblib

import numpy as np, scipy

def save_values(port, fname, outport, pin):
  outser = None
  if outport != None:
    outser = serial.Serial(outport, 9600, timeout=0.01)
    print outser

  ser = serial.Serial(port, 9600, timeout=0.01)
  f = open(fname, 'w')
  ser.write('REC\n')
  value = 0
  
  xi = []
  yi = []

  print "WAITING..."

  while value != 'Record':
    v = ser.readline()
    #print "|%s|" %v
    try:
      t, value = v.split(',')
      value = int(value)
      if outser != None:
        outser.write('PWM %d %d\n' %(pin, value))
        #print outser, 'PWM %d %d\n' %(pin, value)
    except ValueError as e:
      value = v.strip()
      #print value
      #print 'VALUE: |%s|' %v, v.strip() == 'Record'
    time.sleep(0.01)

  print "RECORDING..."

  while value != 'End':
    v = ser.readline()
    try:
      t, value = v.split(',')
      t = int(t)
      value = int(value)
      f.write('%d, %d\n' %(t,value))
      
      xi.append(t)
      yi.append(value)
      
      if outser != None:
        outser.write('PWM %d %d\n' %(pin, value))
        #print 'PWM %d %d\n' %(pin, value)
    except ValueError as e:
      value = v.strip()
      #print value
    
    time.sleep(0.01)  
  print "DONE..."
  
  outser.write('PWM %d %d\n' %(pin, 0))
  
  pchip = scipy.interpolate.PchipInterpolator(np.array(xi), np.array(yi))
  joblib.dump(pchip, '%s.pchip' %fname[:-4])

  f.close()
  ser.close()
  outser.close()

if __name__ == '__main__':
	import argparse

	parser = argparse.ArgumentParser(description='Save values output by Arduino.')
	parser.add_argument('port')
	parser.add_argument('fname')
	parser.add_argument('outport', nargs='?', default=None)
	parser.add_argument('pin', nargs='?', default=0, type=int)

	args = parser.parse_args()

	save_values(args.port, args.fname, args.outport, args.pin)
