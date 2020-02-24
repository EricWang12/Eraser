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
	bias = [800,1100,0,300]
	slope = [1,1,0.55,0.65]
	def __init__(self):
		# Make sure the COM port is right with the Arduino!!
		self.COM = 'COM9'
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
			analog_data = list(map(int, raw_data.split()))
			force_data = [analog_data[i] * slope[i] + bias[i] for i in range(4) ]
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
	def getForces(self, fsrADC):
				
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
		

		return force




e = Eraser()
while True:
	print(e.get_separate_forces())