import serial
import time
import csv

ser = serial.Serial('COM7', "115200", timeout=.1)
ser.flushInput()

for _ in range(200):
    data = ""
    while True:
        data = ser.readline()
        if data:
            ser_bytes = data.decode().rstrip('\n')  # strip out the new lines for now
            break
    # ser_bytes.replace(' ','')
    print(ser_bytes)
    analog_data = []
    try:
        analog_data = list(map(int, ser_bytes.split()))
    except:
        print(analog_data)
    with open("test_data.csv","a") as f:
        writer = csv.writer(f,delimiter=",")
        writer.writerow(analog_data)
