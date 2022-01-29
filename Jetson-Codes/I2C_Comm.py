import smbus
import time
bus = smbus.SMBus(1)
address = 0x64
while True:
	bus.write_byte(address,65)
	time.sleep(1)
	bus.write_byte(address,67)
	time.sleep(1)
