

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
				//byte bu[9];
BLECharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite,512 );

void setup()
{
	Serial.begin(9600);
	while (!Serial)
		;


	// begin initialization
	if (!BLE.begin())
	{
		Serial.println("starting BLE failed!");

		while (1)
			;
	}

	// set advertised local name and service UUID:
	BLE.setLocalName("BLEReader");
	BLE.setAdvertisedService(ledService);

	// add the characteristic to the service
	ledService.addCharacteristic(switchCharacteristic);

	BLE.addService(ledService);

	switchCharacteristic.writeValue((int16_t)0);

	BLE.advertise();
	Serial.println("BLE Reader Peripheral");
}

void loop()
{

	BLEDevice central = BLE.central();


	if (central)
	{
		Serial.print("Connected to Device: ");
		Serial.println(central.address());

		while (central.connected())
		{

			if (switchCharacteristic.written())
			{
				int16_t countPlus[4];

				switchCharacteristic.readValue(countPlus,8);
				int totalForce = 0;
				for (int i = 0; i < 4 ; i++)
				{
					Serial.print(countPlus[i]);
					Serial.print(" ");
					totalForce+= countPlus[i];
				}
				Serial.println(totalForce);
			}
		}

		// when the central disconnects, print it out:
		Serial.print(F("Disconnected from central: "));
		Serial.println(central.address());
	}
}
