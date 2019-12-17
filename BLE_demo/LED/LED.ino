/*
  LED

  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
				//byte bu[9];
BLECharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite,512 );

const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup()
{
	Serial.begin(9600);
	while (!Serial)
		;

	// set LED pin to output mode
	pinMode(ledPin, OUTPUT);

	// begin initialization
	if (!BLE.begin())
	{
		Serial.println("starting BLE failed!");

		while (1)
			;
	}

	// set advertised local name and service UUID:
	BLE.setLocalName("LED");
	BLE.setAdvertisedService(ledService);

	// add the characteristic to the service
	ledService.addCharacteristic(switchCharacteristic);

	// add service
	BLE.addService(ledService);

	// set the initial value for the characeristic:
	switchCharacteristic.writeValue((int16_t)0);

	// start advertising
	BLE.advertise();

	Serial.println("BLE LED Peripheral");
}

void loop()
{
	// listen for BLE peripherals to connect:
	BLEDevice central = BLE.central();

	// if a central is connected to peripheral:
	if (central)
	{
		Serial.print("Connected to central: ");
		// print the central's MAC address:
		Serial.println(central.address());

		// while the central is still connected to peripheral:
		while (central.connected())
		{
			// if the remote device wrote to the characteristic,
			// use the value to control the LED:
			if (switchCharacteristic.written())
			{
				//int a = (switchCharacteristic.value());    // any value other than 0

				int16_t countPlus[4];
				byte buf[9];
				switchCharacteristic.readValue(countPlus,8);

				//convertToIntArray(countPlus, buf);
				for (int i = 0; i < 4 ; i++)
				{
					Serial.print(countPlus[i]);
					Serial.print("  ");
				}
				Serial.println(" ");
			}
			// int countPlus[4];
			// receiveData(countPlus);
			// for (int i = 0; i < 4; i++)
			// {
			// 	Serial.println(countPlus[i]);
			// }
		}

		// when the central disconnects, print it out:
		Serial.print(F("Disconnected from central: "));
		Serial.println(central.address());
	}
}
String convertToString(byte *a, int size)
{
	int i;
	String s = "";
	for (i = 0; i < size; i++)
	{
		String str(a[i]);
		s = s + (char)a[i];
	}
	return s;
}

int convertToByteArray(byte *buf, int16_t *Array)
{
	int L = 8;
	for (int i = 0; i < L; i += 2)
	{
		buf[i] = Array[i / 2];
		buf[i + 1] = Array[i / 2] >> 8;
	}
	return L;
}
int convertToIntArray(int16_t *Array, byte *buf)
{
	int L = 8;
	for (int i = 0; i < L; i += 2)
	{
		Array[i / 2] = buf[i];
		Array[i / 2] += buf[i + 1] << 8;
	}
	return L;
}

// void receiveData(int* receivedData){
// 	int currData = 0;
// 	int i = 0;
// 	while(currData >= 0 ){
// 		if (switchCharacteristic.written())
// 		{
// 			(switchCharacteristic.readValue(currData));    // any value other than 0
// 			if(currData<0)return;
// 			receivedData[i++] = currData;

// 		}
// 	}
// }