#include <ArduinoBLE.h>


void setup()
{

	BLE.begin();
	BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop()
{

	BLEDevice peripheral = BLE.available();

	//Serial.println(convertToString(buf, count.length()+1));

	if (peripheral)
	{
		// discovered a peripheral, print out address, local name, and advertised service

		BLE.stopScan();

		controlLed(peripheral);
		BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
	}
}

void controlLed(BLEDevice peripheral)
{
	// connect to the peripheral

	if (!peripheral.connect())
	{
		return;
	}

	// discover peripheral attributes
	if (!peripheral.discoverAttributes())
	{
		peripheral.disconnect();
		return;
	}

	// retrieve the LED characteristic
	BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
	//ledCharacteristic.valueSize = 100;

	while (peripheral.connected())
	{


		int16_t count[] = {105, 251, 400, 600};
		ledCharacteristic.writeValue(count, 8);

		delay(100);
	}
}
