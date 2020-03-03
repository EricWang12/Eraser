#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include "Adafruit_VCNL4010.h"
// #include "Adafruit_VCNL4040.h"
#include "Adafruit_APDS9960.h"


#define APDS9960
#define POLOLU


Adafruit_APDS9960 apds;
// Adafruit_VCNL4040 vcnl4040 = Adafruit_VCNL4040();

Adafruit_VCNL4010 vcnl;
Adafruit_VL6180X vl = Adafruit_VL6180X();

TwoWire I2Cone = TwoWire(0, A4, A5);
void setup()
{
	Serial.begin(115200);

	// wait for serial port to open on native usb devices
	while (!Serial)
	{
		delay(1);
	}

	TwoWire tw = TwoWire(0, A4, A5);
	Serial.println("Adafruit VL6180x test!");
	if (!vl.begin(&Wire))
	{
		Serial.println("Failed to find sensor VL6180x");
		while (1)
			;
	}
	Serial.println("VL6180x Sensor found!");

	Serial.println("VCNL4010 test");

	if (!vcnl.begin())
	{
		Serial.println("VCNL4010 Sensor not found :(");
		while (1)
			;
	}
	Serial.println("Found VCNL4010");


	#if defined(VCNL4040)
		Serial.println("Adafruit VCNL4040 Config demo");

		if (!vcnl4040.begin()) {
			Serial.println("Couldn't find VCNL4040 chip");
			while (1);
		}
		Serial.println("Found VCNL4040 chip");
	#endif
	#if defined(APDS9960)
		if(!apds.begin()){
			Serial.println("failed to initialize APDS9960 device! Please check your wiring.");
		}
		else Serial.println("APDS9960 Device initialized!");

		apds.setProxGain(APDS9960_PGAIN_1X);

		//enable proximity mode
		apds.enableProximity(true);

		//set the interrupt threshold to fire when proximity reading goes above 175
		apds.setProximityInterruptThreshold(0, 175);
	#endif

}

void loop()
{
	// float lux = vl.readLux(VL6180X_ALS_GAIN_5);

	// Serial.print("Lux: ");
	// Serial.println(lux);

	uint8_t range = vl.readRange();
	uint8_t status = vl.readRangeStatus();


	Serial.print("VL6180x Range: ");  Serial.print(range);

	Serial.print("   VCNL4010 Proximity: "); Serial.print(vcnl.readProximity());
	
	#if defined(VCNL4040)
	Serial.print("   VCNL4040 Proximity:"); Serial.print(vcnl4040.getProximity());
	#endif
	#if defined(APDS9960)
	Serial.print("   APDS9960 Proximity:"); Serial.print(apds.readProximity());
	#endif


	#if defined(POLOLU)
	Serial.print("   Pololu Proximity:");Serial.println(analogRead(A0));
	#endif
	// Some error occurred, print it out!

	delay(50);
}