# vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2

import serial

def save_values(port, fname, outport, pin):
  outser = None
  if outport != None:
    outser = serial.Serial(outport, 9600)

  ser = serial.Serial(port, 9600)
  f = open(fname, 'w')
  ser.write('REC\n')
  value = 0
  while value != 'Record\n':
    v = ser.readline()
    try:
      t, value = v.split(',')
      value = int(value)
      if outser != None:
        outser.write('PWM %d %d' %(pin, value))
    except ValueError as e:
      value = v

  while value != 'End\n':
    v = ser.readline()
    try:
      t, value = v.split(',')
      t = int(t)
      value = int(value)
      f.write('%d, %d\n' %(t,value))
      
      if outser != None:
        outser.write('PWM %d %d' %(pin, value))
    except ValueError as e:
      value = v
      print v

  f.close()

if __name__ == '__main__':
	import argparse

	parser = argparse.ArgumentParser(description='Save values output by Arduino.')
	parser.add_argument('port')
	parser.add_argument('fname')
	parser.add_argument('outport', nargs='?', default=None)
	parser.add_argument('pin', nargs='?', default=0, type=int)

	args = parser.parse_args()

	save_values(args.port, args.fname, args.outport, args.outpin)
