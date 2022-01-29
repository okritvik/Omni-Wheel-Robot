import serial
import time

ser = serial.Serial('/dev/ttyACM0')
time.sleep(2)
print(ser.name)
ser.baudrate = 115200
print(ser.baudrate)
while True:
	ser.write("ACW".encode())
	time.sleep(1.5)
	ser.write("CW".encode())
	time.sleep(1.5)
	#data = ser.readline().decode('latin-1')
	#print(data)
	'''
	s = input()
	ser.write(s.encode())
	'''
