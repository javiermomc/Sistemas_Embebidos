import time
import grovepi
from grovepi import *
import math
from grove_rgb_lcd import *

button = 4		#Port for Button
sensor = 0
pinMode(button,"INPUT")

setText("Presiona el\nboton")
setRGB(255,0,0)

while True:
	time.sleep(0.01)
	try:
		button_status= grovepi.digitalRead(button)	#Read the Button status
		if not button_status:	#If the Button is in HIGH position, run the program
			setText_norefresh("Presiona el    \nboton     ")
                        setRGB(255,0,0)
		else:		#If Button is in Off position, print "Off" on the screen
			temp = grovepi.temp(sensor,'1.1')
			setText_norefresh("Temp = {}".format(str(temp)))
			setRGB(0,255,0)
	except KeyboardInterrupt:	# Stop the buzzer before stopping
		break
	except IOError:
	        print ("Error")
		break
	except:
		print("Error")
