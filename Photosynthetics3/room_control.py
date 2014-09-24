import pygame, serial
from pygame.locals import *

from sprites import *

ARDUINO_USB = '/dev/cu.usbmodem2031'
ARDUINO_XBEE = '/dev/tty.usbserial-A10139SD'

class App:

    def __init__(self):
        self._running = True
        self._display_surf = None
        self.size = self.weight, self.height = 640, 400
        
        self.lights = pygame.sprite.Group()
        self.leaves = pygame.sprite.Group()
        
        #self.xbee = serial.Serial(ARDUINO_XBEE, 9600)
        self.usb = serial.Serial(ARDUINO_USB, 9600)
        self.xbee = serial.Serial(ARDUINO_XBEE, 9600)

    def on_init(self):
        '''When the room boots up.
        '''
        pygame.init()
        
        # Initialize all of the leaves and lights here
        self.__init_leaves()
        self.__init_lights()

	import os
	os.environ['SDL_VIDEODRIVER'] = 'dummy'
	pygame.display.init()
	screen = pygame.display.set_mode((1,1))
        
        #self._display_surf = pygame.display.set_mode(self.size, pygame.HWSURFACE | pygame.DOUBLEBUF)
        self._running = True
 
    def on_event(self, event):
        '''Handle custom events, such as people entering the room, people entering and leaving the zones.
        '''
        if event.type == pygame.QUIT:
            self._running = False
    
    def on_loop(self):
        '''Update the parameters for the lights and the leaves (and the sounds).
        '''
        
        # Call update for each leaf and light
        for obj in self.leaves.sprites() + self.lights.sprites():
            obj.update()
        
    def on_render(self):
        '''Pass messages on via serial commands to the Arduinos
        '''
        
        # Send serial message for each leaf and light
        
        pass
        
    def on_cleanup(self):
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
            pygame.time.wait(50);
        self.on_cleanup()
 
    def __init_lights(self):
        lights = [
            # connection, pin, longPeriod, longOffset, midPeriod, midOffset, ?maxValue?
            [self.xbee, 9, 8.5*60, 19, 3.2*60, 0],
            [self.xbee, 11, 8*60, 4*60, 2.5*60, 60], 
            [self.usb, 3, 10*60, 20, 1.4*60, 10],  
            [self.usb, 5, 5*60, 817, 0.75*60, 120],  
            [self.usb, 6, 8*60, 47/2, .89*60, 6],  
            [self.usb, 9, 5.5, 12, 1.12, 2],
            [self.usb, 13, 4.3*60, 1237, 0.6*60*2, 0], 
        ]

        for light in lights:
            self.lights.add(Light(*light))
 
    def __init_leaves(self):
        leaves = [
            # Purple Legs
            PurpleBiLeaf(self.xbee, 3, short=False),
            PurpleBiLeaf(self.xbee, 5, short=True),
            # White Leaf
            WhiteLeaf(self.xbee, 6), 
            # White Legs
            WhiteBiLeaf(self.xbee, 10, short=True),
            WhiteBiLeaf(self.xbee, 13, short=False), 
            # White Scrunchie
            WhiteScrunchie(self.usb, 11),
        ]

        for leaf in leaves:
            self.leaves.add(leaf)
        
if __name__ == "__main__" :
    theApp = App()
    theApp.on_execute()
