# vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2

import serial, time, csv

def millis():
  return int(time.time() * 1000)

def play_values(port, fname):
	ser = serial.Serial(port, 9600)
	f = open(fname, 'r')
	reader = csv.reader(f, delimiter=',')
	
	start_t = millis()
	
	for t, val in reader:
	  while (millis()-start_t) < int(t):
	    ser.write('PLAY %d\n' %int(val))
	    time.sleep(0.001)
	
	f.close()

if __name__ == '__main__':
	import argparse

	parser = argparse.ArgumentParser(description='Save values output by Arduino.')
	parser.add_argument('port')
	parser.add_argument('fname')

	args = parser.parse_args()

	play_values(args.port, args.fname)
