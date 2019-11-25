import serial
import time
from serial import Serial


class Eraser:

	def __init__(self):
		# Make sure the COM port is right with the Arduino!!
		self.COM = 'COM3'
		self.serial = 9600
		self.arduino = serial.Serial(self.COM, self.serial, timeout=.1)
		time.sleep(1)  # give the connection a second to settle

	def get_forces(self):
		global force_data;

		# arduino.write("Hello from Python!")
		while True:
			data = self.arduino.readline()
			if data:
				raw_data = data.decode().rstrip('\n')  # strip out the new lines for now
				break;
				# (better to do .read() in the long run for this reason
		force_data = list(map(int, raw_data.split()));
		return force_data;

	def get_total_force(self):
		return self.get_forces()[4]

	def get_separate_forces(self):
		return self.get_forces()[0:4]


e = Eraser()
while True:
	print(e.get_separate_forces())