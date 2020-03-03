import serial
import time
from serial import Serial

"""
  3                             4
  |-----------------------------|
  |                  |  | ///// |
  |                  |  | \\\\\ |
  |                  |  | ///// |
  |-----------------------------|
  2                             1
"""

class Eraser:
	force_data = [0]
	bias = [160,350,100,120]
	slope = [0.25,0.35,0.35,0.4]
	def __init__(self):
		# Make sure the COM port is right with the Arduino!!
		self.COM = 'COM9'
		self.serial = 9600
		self.arduino = serial.Serial(self.COM, self.serial, timeout=.1)
		time.sleep(1)  # give the connection a second to settle

	def calculate_force(self, analog_data):

		ret = list()

		for fsrADC in analog_data:
			force = 0

			if (fsrADC != 0): 
			
				VCC = 3.3
				R_DIV = 1000
				fsrV = fsrADC * VCC / 1023.0
				fsrR = R_DIV * (VCC / fsrV - 1.0)

				fsrG = 1.0 / fsrR
				if (fsrR <= 600):
					force = (fsrG - 0.00075) / 0.00000032639
				else:
					force =  fsrG / 0.0000003857142
			ret.append(int(force))
			

		return ret

	def get_forces(self):
		# arduino.write("Hello from Python!")
		while True:
			data = self.arduino.readline()
			if data:
				raw_data = data.decode().rstrip('\n')  # strip out the new lines for now
				break
				# (better to do .read() in the long run for this reason
		try:
			analog_data = self.calculate_force(list(map(int, raw_data.split())))
			self.force_data =  [ max(int(analog_data[i] * self.slope[i] - self.bias[i]),0) for i in range(4) ]
		except:
			print(raw_data)

		return

	def get_total_force(self):
		self.get_forces()
		try:
			if(self.force_data == None or len(self.force_data)!=0):
				return self.force_data[4]
		except NameError:
			return ""

	def get_separate_forces(self):
		self.get_forces()
		try:
			if(self.force_data == None or len(self.force_data)!=0):
				return self.force_data[0:4]
		except NameError:
			return ""
	


e = Eraser()
i = 0
while True:

	print(e.get_separate_forces())
