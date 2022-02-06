cdef extern void initSensor()
cdef extern void freeSensor()
cdef extern void setTX()
cdef extern void clearTX()
cdef extern int readRX()

class Sensor:
    def __init__(self):
        # Create memory mapped gpioMap object and set GPIO functions in C
        initSensor()
        print("initialize done")
    def __del__(self):
        # C deconstructor
        freeSensor()
        
    def TX_set(self):
 
        setTX()
            
    def TX_clear(self):

        clearTX()
    def RX_read(self):
        return readRX()
