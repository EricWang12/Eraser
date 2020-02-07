import serial
import time
from serial import Serial


class Eraser:
	force_data = [0]

	def __init__(self):
		# Make sure the COM port is right with the Arduino!!
		self.COM = 'COM8'
		self.serial = 9600
		self.arduino = serial.Serial(self.COM, self.serial, timeout=.1)
		time.sleep(1)  # give the connection a second to settle

	def get_forces(self):
		global force_data

		# arduino.write("Hello from Python!")
		while True:
			data = self.arduino.readline()
			if data:
				raw_data = data.decode().rstrip('\n')  # strip out the new lines for now
				break
				# (better to do .read() in the long run for this reason
		try:
			force_data = list(map(int, raw_data.split()))
		except:
			print(raw_data)

		return

	def get_total_force(self):
		self.get_forces()
		try:
			if(force_data == None or len(force_data)!=0):
				return force_data[4]
		except NameError:
			return ""

	def get_separate_forces(self):
		self.get_forces()
		try:
			if(force_data == None or len(force_data)!=0):
				return force_data[0:4]
		except NameError:
			return ""



e = Eraser()
while True:
	print(e.get_separate_forces())