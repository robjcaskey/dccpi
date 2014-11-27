from dccpi import *
from random import randint
from time import time

e = DCCRPiEncoder(pin_a=8,pin_b=9,pin_break=7)
c = DCCController(e)           # Create the DCC controller with the RPi encoder
l1 = DCCLocomotive("DCC6", 3)  # Create locos (see DCCLocomotive class)
c.register(l1)        # Register locos on the controller
c.start()            # Start controller. Remove brake signal
l1.reverse()                  # Change direction bit
x = 0
new_speed =4 
last_time = time()
while True:
	if time() - last_time > 5:
		new_speed += 1
		print 'changing speed to %s' % new_speed
		last_time = time()
	l1.speed = new_speed                 # Change speed
	if True:
		if new_speed % 2:
			l1.fl = True
			l1.f1 = True
			l1.f2 = True
			l1.f3 = True
			l1.f4 = True
		else:
			l1.fl = False
			l1.f1 = False
			l1.f2 = False
			l1.f3 = False
			l1.f4 = False

