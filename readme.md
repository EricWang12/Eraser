# Eraser project for REACH lab

## Workflow
  The project has three parts:
  * Eraser body 
  * BLE reader 
  * PC python script

-----------------
### Eraser body

The main part of the Eraser project, the connection scehmatic is shown below:
![Eraser_bb](https://user-images.githubusercontent.com/22462126/71078133-642c1e00-214e-11ea-8bf2-e57755d9f96e.png)

 
The Arduino is hooked up with 4 pressure sensors (their number is listed in the comment on top of the Eraser  code) and a LED matrix to display the forces on 4 sensors. Due to the difference of each sensor ( not all sensors are prefectly the same after all) and the weight design of the eraser, all four sensor's data are slightly biased. 

The battery is laid under the breadboard and all you need to do to turn it one is to connect the red/black jumper wire to battery module.

The official example code and documentations for sensors can be find here:
https://learn.sparkfun.com/tutorials/force-sensitive-resistor-hookup-guide

In addition, The Eraser sends the pressure data to the BLE Reader through the build-in BLE module and the [ArduinoBLE API](https://www.arduino.cc/en/Reference/ArduinoBLE). please note that this api is very poorly documented, the comment in the code should give a better explanation. 

Basically, the Eraser will retrieve protocol from any device with uuid:
```Arduino
    uuid = "19B10000-E8F2-537E-4F6C-D104768A1214"
```
and send a buffer of 4 int16_t data of 4 forces



### BLE Reader

The BLE (Bluetooth) reader reads any BLE device's data with same uuid:
```Arduino
    uuid = "19B10000-E8F2-537E-4F6C-D104768A1214"
```
The rules are the same: ArduinoBLE is very poorly documented, to save your brain power, please read the comment in the code

### Python script

**it depends on serial package**, run:
```python
pip install serial 
```
to install the package

**IMPORTANT: make sure you know the right port to BLE Reader or Arduino device, the port is default to "COM3" but it could vary from device to device, change the port on line 11 to get the data from the right port**

##### What it does is simple: 

1. Takes in a serial string (format should be 4 int seperated by spaces)
2. Split by spaces
3. put 4 data(forces) into a list and take the sum as total force as the fifth item

if it can't split the serial string into integers, then it would display the string to user (usually for bluetooth status information)

two methods are supported:

```python
    e = Eraser()

    # returns a number, the total force of 4       
    e.get_total_force()

    # returns an list of 4 forces
    e.get_separate_forces()
```
#### *Updated Apr. 2020 :*
The calibration code have been removed to python code base and the arduino only send raw data to python
