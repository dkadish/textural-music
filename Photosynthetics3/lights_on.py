from serial_persistence import connect, map_connections

def main():
    import time
    
    conns = []
    brds = {'0': None, '1':None}
    lights = {'0': [6], '1': [5,9,11]}
    
    '''Test the functionality of the functions.'''
        
    # Connect
    conns = connect(conns)
    
    # Read/write
    conns, brds = map_connections(conns, brds)
    
    for b in brds:
        for i in lights[b]:
            brds[b].write('PWM %d 250\n' %i)
    
    print [conn.port for conn in conns]
    print brds
    
    time.sleep(1)
        
if __name__ == '__main__':
    main()
