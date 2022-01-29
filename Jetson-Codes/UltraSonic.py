
import time
import Jetson.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(12,GPIO.OUT,initial=GPIO.LOW)
GPIO.setup(18,GPIO.IN)

start_time = time.time()
stop_time = time.time()
#Pin 12 - Trigger
# Pin 18 - Echo
while True:
	GPIO.output(12,GPIO.LOW)
	time.sleep(0.00001)
	GPIO.output(12,GPIO.HIGH)
	time.sleep(0.00001)
	GPIO.output(12,GPIO.LOW)
	while GPIO.input(18)==0:
		start_time=time.time()
	while GPIO.input(18)==1:
		stop_time=time.time()
	#17150 because 34300cm/sec and the time duration is to and fro. so we have to divide total distance by 2.
	print("Distance -",(stop_time-start_time)*17150)
	#print((stop_time-start_time)*17150)
	time.sleep(1)

