
#Version 1 has communication delay and exits from loop due to pyserial.available error

#THIS IS VERSION 2

import jetson.inference
import jetson.utils
import serial
import time
import Jetson.GPIO as GPIO

try:
	GPIO.setmode(GPIO.BOARD)
	GPIO.setwarnings(False)
	GPIO.setup(12,GPIO.OUT,initial=GPIO.LOW)
	GPIO.setup(18,GPIO.IN)
	flag = False
	ser = serial.Serial('/dev/ttyACM0')
	print(ser.name)
	ser.baudrate=115200
	net = jetson.inference.detectNet("coco-bottle", threshold=0.5)
	camera = jetson.utils.videoSource("csi://0")      # '/dev/video0' for V4L2
	display = jetson.utils.videoOutput("display://0") # 'my_video.mp4' for file

	######## FUNCTION TO GET THE DISTANCE FROM ULTRASONIC SENSOR ######

	'''
	def getDistance():
		start_time = time.time()
		stop_time = time.time()
	# Pin 12 - Trigger
	# Pin 18 - Echo
		GPIO.output(12,GPIO.HIGH)
		time.sleep(0.00001)
		GPIO.output(12,GPIO.LOW)
		while GPIO.input(18)==0:
			start_time=time.time()
		while GPIO.input(18)==1:
			stop_time=time.time()
		#17150 because 34300cm/sec and the time duration is to and fro. so we have to divide total distance by 2.
		return (stop_time-start_time)*17150
	'''
	while display.IsStreaming():
		img = camera.Capture()
		print("Image shape is ",img.shape)

	#############       Calculating the center of the image   ################

		img_wc = img.shape[1]/2
		img_hc = img.shape[0]/2

	#################    Detecting the objects from the frame  ##################

		detections = net.Detect(img)
		print("Number of Detections : ",detections)

	#####If there are any objects in the camera frame... Then print the center of the bounding box####

		if(len(detections)!=0):
			for i in range (0,len(detections)):
				print("Class ID:",detections[i].ClassID,"\nCenter of the bounding box: ",detections[i].Center)

	################# TRACKING THE FIRST OBJECT ####################################

			print("Tracking first object ")
			obj_center = detections[0].Center
			obj_wc = obj_center[0]
			obj_hc = obj_center[1]
			print("Object width and height centers", obj_wc, obj_hc)
			object_area = detections[0].Area
			print("Area of the object : ",object_area)

	### Calculating the difference between the width of object and width of image for the rotation ###

			delta = obj_wc - img_wc
			print("Delta : ",delta)

	#### Delta is the difference in pixels ############
			if(delta>20):

	###### Rotate clock-wise direction #############

				print("Rotate Clockwise")
				ser.write("CW".encode())
				time.sleep(0.5)
			elif(delta<-20):

	######## Rotate anti clock-wise direction ########################

				print("Rotate Anti Clockwise")
				ser.write("ACW".encode())
				time.sleep(0.5)
			else:
				if((flag==False)):

	########## Stop the Rotation #################################

					print("Checking depth using arduino ultrasonic and going in the direction")
					ser.write("USS".encode())
					time.sleep(0.5)
					flag=True
				#if(ser.readline().rstrip().decode('latin-1')=="DONE"):
				#	print("ARM MOVEMENT NOW!!!")
				#	ser.write("S".encode())
				#	time.sleep(1.5)
				#GO FOR ARM MOVEMENT
				
				#ser.write("S".encode())
				#time.sleep(1.5)
	######## Compare the Object Area required for the movement of the robot in forward direction #####
	### Comparing the Bounding box area for depth is giving inaccurate results. Hence the distance should be calculated from depth sensor.
	### HCSR04 gives garbage values if we interface directly with the J41 Header of Jetson Nano. With Raspberry Pi, Ultrasonic was giving results but not accurate. The reason is that the GPIO pins are not PWM pins and using python to detect the pulses of a few milliseconds width is not feasible with both RPi and Jetson Nano.
	## Hence send a command to Arduino to calculate the distance and move accordingly.
		else:

	######### Rotate in the clock wise direction till you find an object in the frame. ###########

			print("Search for objects, Rotating Clockwise")
			if(flag==True):
				flag=False
				ser.write("BREAK".encode())
				time.sleep(0.5)
			ser.write("CW".encode())
			time.sleep(0.5)
			
		display.Render(img)
		display.SetStatus("Bottle Detection Detection | Network {:.0f} FPS".format(net.GetNetworkFPS()))
except KeyboardInterrupt:
	GPIO.cleanup()
