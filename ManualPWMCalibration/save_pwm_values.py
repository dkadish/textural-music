# vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2

import serial

def save_values(port, fname):
  ser = serial.Serial(port, 9600);
  f = open(fname, 'w')
  ser.write('REC\n');
  value = -1
  while value == -1:
    v = ser.readline()
    try:
      value = int(v.strip())
    except ValueError as e:
      value = 0

  while value >= 0:
    v = ser.readline()
    try:
      t, value = v.split(',')
      t = int(t)
      value = int(value)
      f.write('%d, %d\n' %(t,value))
    except ValueError as e:
      value = -1

  f.close()

if __name__ == '__main__':
	import argparse

	parser = argparse.ArgumentParser(description='Save values output by Arduino.')
	parser.add_argument('port')
	parser.add_argument('fname')

	args = parser.parse_args()

	save_values(args.port, args.fname)
