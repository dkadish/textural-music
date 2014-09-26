from Queue import LifoQueue
from threading import Thread
import serial, time, math

import signals

def loop(q):
    i = 0.0
    while True:
        time.sleep(0.001)
        i += 0.001
        q.put(i)
        
def communicator(ser, q):
<<<<<<< HEAD
    while True:
        time.sleep(0.01)
        data = 128*math.sin(q.get()) + 128

        ser.write('PWM 13 %d\n' %(data))
=======
>>>>>>> 100fc8cc60c3451d315907ff23f30adf9458bce3

    sinewave = signals.SineWave(10, 1);
    while True:
        time.sleep(0.25)
        
        data = sinewave.value(q.get())
        if data < 0:
            sinewave.period(5)
        else:
            sinewave.period(15)        
        
        print time.ctime(), data
        while not q.empty():
            q.get()

<<<<<<< HEAD
ser = serial.Serial('/dev/tty.usbserial-A10139SD', 9600)
=======
#ser = serial.Serial('/dev/ttyACM0', 9600)
>>>>>>> 100fc8cc60c3451d315907ff23f30adf9458bce3
q = LifoQueue()
t1 = Thread(target=loop, args=(q,))
t2 = Thread(target=communicator, args=(None, q))#ser, q,))
t1.start()
t2.start()        
        
'''
def sendCommand(ser, cmd, *args):
    #Send a command to Arduino
    
    t = args[0]

    i = args[0] + 1
    if i < 5:
        t = Timer(1.0, sendCommand, ['going', i])
        t.start() # after 30 seconds, "hello, world" will be printed



t = Timer(0.01, sendCommand, [ser, 'start', 1])
t.start() # after 30 seconds, "hello, world" will be printed
'''
