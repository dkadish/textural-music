import serial
from serial.tools import list_ports_linux

def connect(connections):
    '''Find all non-connected serial ports and try to connect to them'''
    ports_in_use = map(lambda c: c.port, connections)
    disconnected_ports = filter(lambda cs: cs[0] not in ports_in_use, list_ports_linux.comports())
    
    for port, name, info in disconnected_ports:
        try:
            connections.append(serial.Serial(port, 9600, timeout=0.1))
        except serial.SerialException as e:
            pass
            
    return connections
    
def map_connections(connections, boards):
    '''Map the set of connections to a dict of boards'''
    for conn in connections:
        try:
            # Ask the board to identify itself
            conn.write('ID\n')
            reply = conn.readlines()
            
            if reply != []: # If there has been a reply
                # Check is this port still connected to its board?
                board = filter(lambda b: boards[b] == conn, boards)
                        
                if board == []:
                    # Which is it connected to?
                    for bid in boards:
                        if reply[-1].startswith(bid):
                            boards[bid] = conn
                            
        except serial.SerialException as e:
            conn.close()
            board = filter(lambda b: boards[b] == conn, boards)
            if board != []:
                boards[board[0]] = None
                
    # Get rid of closed connections
    connections = filter( lambda c: c.isOpen(), connections )
    
    return connections, boards

def main():
    import time
    
    conns = []
    brds = {'0': None, '1':None}
    
    while True:
        '''Test the functionality of the functions.'''
            
        # Connect
        conns = connect(conns)
        
        # Read/write
        conns, brds = map_connections(conns, brds)
        
        
        print [conn.port for conn in conns]
        print brds
        
        time.sleep(1)
        
if __name__ == '__main__':
    main()
