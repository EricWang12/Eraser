
#include <ArduinoBLE.h>

void setup()
{
	BLE.begin();
	BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop()
{

	BLEDevice peripheral = BLE.available();

	if (peripheral)
	{

		BLE.stopScan();

		controlLed(peripheral);
		BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
	}
}

void controlLed(BLEDevice peripheral)
{

}