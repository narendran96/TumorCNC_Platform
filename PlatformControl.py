# Python interface with Platform
# Rotation in degrees and translation in inches
from serial import Serial
from serial import SerialException
import time

class ControlPlatform:
    def __init__(self, com=None, port="COM3"): #Default port is COM3 on my laptop
    # attempt to connect
	    while True:
	        try:
	            self.com = Serial(port, 115200)
	            break
	        except SerialException:
	            print('No device on ' + port)
	            print('Not connected to arduino. Continuing anyway...')
	            return
	    print('Connected to Arduino on ' + str(port))


    #rotate the platform
    def rotate(self, x, y, z):
    	self.com.write(str.encode("R"+","+str(x)+","+str(y)+","+str(z)))
    	#print(str.encode("R"+","+str(x)+","+str(y)+","+str(z)))
    	time.sleep(2)

    #translate the platform (z + predefined height in arduino code)
    def translate(self, x, y, z):
    	self.com.write(str.encode("T"+","+str(x)+","+str(y)+","+str(z)))
    	time.sleep(2)

    #translate and rotate the platform (z + predefined height in arduino code)
    def rotate_translate(self, x, y, z, a, b, c):
    	self.com.write(str.encode("D"+","+str(x)+","+str(y)+","+str(z)+","+str(a)+","+str(b)+","+str(c)))
    	#print(str.encode("D"+","+str(x)+","+str(y)+","+str(z)+","+str(a)+","+str(b)+","+str(c)))
    	time.sleep(2)

    def reset(self):
    	self.com.write(str.encode("r"))
    	#print(str.encode("r"))
    	time.sleep(2)

    def close_serial(self):
    	self.com.close()
