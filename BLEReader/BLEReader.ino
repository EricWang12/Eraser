

#include <ArduinoBLE.h>

BLEService bleService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
/* 	
	IMPORTANT: 512 is the size (actually the maximum, don't set the size over 512 ) of the 
	BLEcharacteristic, think this like a bluetooth data container that shared by all device
	with same uuid, write value put/replace stuffs in and read value peeks (read) things in it

	The documentation of this constructor is both wrong and meaningless, dont read it (updated 12/18/2019)

	The right syntax is switchCharacteristic( string uuid, ? properties, iny size, bool fixedlength = false)

*/
BLECharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 512 );

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
	// Name is just a insurance for the other end to check
	// not really important here
	BLE.setLocalName("BLEReader");
	BLE.setAdvertisedService(bleService);

	// add the characteristic to the service
	bleService.addCharacteristic(switchCharacteristic);

	BLE.addService(bleService);

	// this initialize the connection, like a handshake right :)
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
				// Note the reason length is 8 here is because
				// it's the length of the total BYTES!!!!
				// and int16_t is 2 bytes each
				switchCharacteristic.readValue(countPlus,8);
				int totalForce = 0;

				// simply push the data to PC and let python handles the rest
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
