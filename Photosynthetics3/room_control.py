import pygame, serial
from pygame.locals import *

from sprites import *
from signals import StepFunction
#from camera import Camera
#from motion_detect import MotionDetect
from serial_persistence import connect, map_connections

#ARDUINO_USB = '/dev/ttyACM0'
#ARDUINO_XBEE = '/dev/tty.usbserial-A10139SD'
SERIAL = True

class App:

    def __init__(self):
        self._running = True
        self._display_surf = None
        self.size = self.weight, self.height = 640, 400
        
        self.lights = pygame.sprite.Group()
        self.leaves = pygame.sprite.Group()
        
        if SERIAL:
            #self.usb = serial.Serial(ARDUINO_USB, 9600)
            #self.xbee = serial.Serial(ARDUINO_XBEE, 9600)
            
            self.connections = []
            self.boards = {'0': None, '1': None}
        else:
            import tempfile
            #self.usb = tempfile.TemporaryFile()
            #self.xbee = tempfile.TemporaryFile()
            self.connections = []
            self.boards = {'0': tempfile.TemporaryFile(), '1': tempfile.TemporaryFile()}
            
        #self.camera = Camera(self)
        #self.motion_detect = MotionDetect(self)
        #self.subs = [self.camera, self.motion_detect]

    def on_init(self):
        '''When the room boots up.
        '''
        pygame.init()
        
        self.size = (640,480)
        self.display = pygame.display.set_mode(self.size, 0)
        
        # Initialize all of the leaves and lights here
        self.__init_leaves()
        self.__init_lights()
        
        #for sub in self.subs:
        #    sub.on_init()

#	import os
#	os.environ['SDL_VIDEODRIVER'] = 'dummy'
#	pygame.display.init()
#	screen = pygame.display.set_mode((1,1))
        
        #self._display_surf = pygame.display.set_mode(self.size, pygame.HWSURFACE | pygame.DOUBLEBUF)
        self._running = True
 
    def on_event(self, event):
        '''Handle custom events, such as people entering the room, people entering and leaving the zones.
        '''
        if event.type == pygame.QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            self._running = False
            
        #for sub in self.subs:
        #    try:
        #        sub.on_event(event)
        #    except AttributeError as e:
        #        pass
    
    def on_loop(self):
        '''Update the parameters for the lights and the leaves (and the sounds).
        '''
        
        # Call update for each leaf and light
        for obj in self.leaves.sprites() + self.lights.sprites():
            obj.update()
        
        #for sub in self.subs:
        #    sub.on_loop()
        #    
        #left = self.motion_detect.get_left_diff()
        #right = self.motion_detect.get_right_diff()
        #print left, right, len(self.leaves.sprites()[0].signal.signals), len(self.leaves.sprites()[1].signal.signals), self.leaves.sprites()[0].value, self.leaves.sprites()[1].value
        #if left > 4 and len(self.leaves.sprites()[0].signal.signals) == 3:
        #    self.leaves.sprites()[0].signal.signals.append(StepFunction(10, v_range=(0,0.5)))
        #    self.lights.sprites()[0].signal.signals.append(StepFunction(10, v_range=(0,0.5)))
        #if right > 4 and len(self.leaves.sprites()[1].signal.signals) == 3:
        #    self.leaves.sprites()[1].signal.signals.append(StepFunction(10, v_range=(0,0.5)))
        #    self.lights.sprites()[0].signal.signals.append(StepFunction(10, v_range=(0,0.5)))
        
    def on_render(self):
        '''Pass messages on via serial commands to the Arduinos
        '''
        # Update the serial connections
        if SERIAL:
            self.connections = connect(self.connections)
            self.connections, self.boards = map_connections(self.connections, self.boards) #TODO: does this get mainained in the sprites?
        
        for obj in self.leaves.sprites() + self.lights.sprites():
            obj.on_render()
            
        #print self.boards[self.leaves.sprites()[0].board]
        #print self.connections
        #print self.boards
            
        # Send serial message for each leaf and light
        #for sub in self.subs:
        #    sub.on_render()
        
    def on_cleanup(self):
        for conn in self.connections:
            conn.close()
    
        #for sub in self.subs:
        #    try:
        #        sub.on_cleanup()
        #    except AttributeError as e:
        #        pass
            
        pygame.quit()
 
    def on_execute(self):
        if self.on_init() == False:
            self._running = False
 
        while( self._running ):
            pygame.time.wait(10);
            for event in pygame.event.get():
                self.on_event(event)
            self.on_loop()
            self.on_render()
            pygame.time.wait(40);
        self.on_cleanup()
 
    def __init_lights(self):
        lights = [
            # connection, pin, longPeriod, longOffset, midPeriod, midOffset, ?maxValue?
            [self, '0', 9, 8.5*60, 19, 3.2*60, 0],
            [self, '1', 6, 8*60, 4*60, 2.5*60, 60], 
            [self, '1', 3, 10*60, 20, 1.4*60, 10],  
            [self, '1', 5, 5*60, 817, 0.75*60, 120],  
            [self, '1', 6, 8*60, 47/2, .89*60, 6],  
            [self, '1', 9, 5.5, 12, 1.12, 2],
            [self, '1', 13, 4.3*60, 1237, 0.6*60*2, 0], 
        ]

        for light in lights:
            self.lights.add(Light(*light))
 
    def __init_leaves(self):
        leaves = [
            # Purple Legs
            #PurpleBiLeaf(self.xbee, 3, short=False),
            #PurpleBiLeaf(self.xbee, 5, short=True),
            # White Leaf
            #WhiteLeaf(self.xbee, 6), 
            # White Legs
            WhiteBiLeaf(self, '1', 3, short=True),
            WhiteBiLeaf(self, '1', 5, short=False), 
            # White Scrunchie
            #WhiteScrunchie(self.usb, 11),
        ]

        for leaf in leaves:
            self.leaves.add(leaf)
            
    #def get_current_image(self):
    #    return self.camera.snapshot
        
if __name__ == "__main__" :
    theApp = App()
    theApp.on_execute()
