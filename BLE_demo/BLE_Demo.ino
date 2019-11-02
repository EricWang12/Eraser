g@uihao/*
  Battery Monitor

  This example creates a BLE peripheral with the standard battery service and
  level characteristic. The A0 pin is used to calculate the battery level.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

// BLE Battery Service
BLEService batteryService("180F");

const float VCC = 3.3; // Measured voltage of Ardunio 5V line
const float R_DIV = 1000.0; // Measured resistance of 3.3k resistor

// BLE Battery Level Characteristic
BLEUnsignedCharCharacteristic batteryLevelChar("2A19",               // standard 16-bit characteristic UUID
                                               BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

int oldBatteryLevel = 0; // last battery level reading from analog input
long previousMillis = 0; // last time the battery level was checked, in ms

void setup()
{
    Serial.begin(9600); // initialize serial communication
    while (!Serial)
        ;

    pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

    // begin initialization
    if (!BLE.begin())
    {
        Serial.println("starting BLE failed!");

        while (1)
            ;e will appear in advertising packets
    and can be used by remote devices to identify this BLE device
    The name can be changed but maybe be truncated based on space left in advertisement packet
*/
    BLE.setLocalName("EricBLETest");
    BLE.setAdvertisedService(batteryService);           // add the service UUID
    batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
    BLE.addService(batteryService);                     // Add the battery service
    batteryLevelChar.writeValue(oldBatteryLevel);       // set initial value for this characteristic

    /* Start advertising BLE.  It will start continuously transmitting BLE
    advertising packets and will be visible to remote BLE central devices
    until it receives a new connection */

    // start advertising
    BLE.advertise();

    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop()
{
    // wait for a BLE central
    BLEDevice central = BLE.central();

    // if a central is connected to the peripheral:
    if (central)
    {
        Serial.print("Connected to central: ");
        // print the central's BT address:
        Serial.println(central.address());
        // turn on the LED to indicate the connection:
        digitalWrite(LED_BUILTIN, HIGH);

        // check the battery level every 200ms
        // while the central is connected:
        while (central.connected())
        {
            long currentMillis = millis();
            // if 200ms have passed, check the battery level:
            if (currentMillis - previousMillis >= 200)
            {
                previousMillis = currentMillis;
                
            }
        }
        getForce();
        delay(200);
        // when the central disconnects, turn off the LED:
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
    }
}

void updateBatteryLevel()
{
    /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
    int battery = analogRead(A0);
    int batteryLevel = map(battery, 0, 1023, 0, 100);

    if (batteryLevel != oldBatteryLevel)
    {                                             // if the battery level has changed
        Serial.print("Battery Level % is now: "); // print it
        Serial.println(batteryLevel);
        batteryLevelChar.writeValue(batteryLevel); // and update the battery level characteristic
        oldBatteryLevel = batteryLevel;            // save the level for next comparison
    }
}


float getForce(){
  int VCC = 3.3;
  int fsrADC = analogRead(A0);
  float force = 0;
  if (fsrADC != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    Serial.println("Resistance: " + String(fsrR) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    //float force;
    float fsrG = 1.0 / fsrR; // Calculate conductance
    // Break parabolic curve down into two linear slopes:
    if (fsrR <= 600) 
      force = (fsrG - 0.00075) / 0.00000032639;
    else
      force =  fsrG / 0.000000642857;
  }
  Serial.print("The force % is now: "); // print it
  Serial.println(force);
  return force;
}
