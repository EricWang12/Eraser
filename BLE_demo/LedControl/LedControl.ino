
#include <ArduinoBLE.h>

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

void setup()
{
	Serial.begin(9600);
	while (!Serial)
		;
	BLE.begin();
	Serial.println("BLE Central - LED control");
	BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop()
{

	BLEDevice peripheral = BLE.available();

	//Serial.println(convertToString(buf, count.length()+1));

	if (peripheral)
	{
		// discovered a peripheral, print out address, local name, and advertised service
		Serial.print("Found ");
		Serial.print(peripheral.address());
		Serial.print(" '");
		Serial.print(peripheral.localName());
		Serial.print("' ");
		Serial.print(peripheral.advertisedServiceUuid());
		Serial.println();

		BLE.stopScan();

		controlLed(peripheral);
		BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
	}
}

void controlLed(BLEDevice peripheral)
{
	// connect to the peripheral
	Serial.println("Connecting ...");

	if (peripheral.connect())
	{
		Serial.println("Connected");
	}
	else
	{
		Serial.println("Failed to connect!");
		return;
	}

	// discover peripheral attributes
	Serial.println("Discovering attributes ...");
	if (peripheral.discoverAttributes())
	{
		Serial.println("Attributes discovered");
	}
	else
	{
		Serial.println("Attribute discovery failed!");
		peripheral.disconnect();
		return;
	}

	// retrieve the LED characteristic
	BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
	//ledCharacteristic.valueSize = 100;
	if (!ledCharacteristic)
	{
		Serial.println("Peripheral does not have LED characteristic!");
	}
	else if (!ledCharacteristic.canWrite())
	{
		Serial.println("Peripheral does not have a writable LED characteristic!");
	}

	while (peripheral.connected())
	{
		//      byte buf[4];
		//      buf = count;
		//      //buf[count.length()] = '\0';
		//      Serial.println(convertToString(buf, 4));
		//      Serial.println((int)buf);

		int16_t count[] = {105, 251, 400, 600};
		byte buf[9];
		convertToByteArray(buf, count);
		int16_t countPlus[4];
		//convertToIntArray(countPlus, buf);
		ledCharacteristic.writeValue(count, 8);
		// for (int i = 0; i < 4; i++)
		// {
		// 	Serial.println(count[i]);
		// 	int32_t g = count[i];

		// }
		//sendData(103, 250, 500, 600, ledCharacteristic);
		delay(100);
	}

	Serial.println("Peripheral disconnected");
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
void sendData(int force1, int force2, int force3, int force4, BLECharacteristic ledCharacteristic)
{
	ledCharacteristic.writeValue((int32_t)force1);
	ledCharacteristic.writeValue((int32_t)force1);
	ledCharacteristic.writeValue((int32_t)force1);
	ledCharacteristic.writeValue((int32_t)force1);
	ledCharacteristic.writeValue((int32_t)-1);
}