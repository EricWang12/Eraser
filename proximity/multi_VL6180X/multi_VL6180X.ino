/*

*/

#include <Wire.h>
#include "VL6180X.h"


#define SET_SENSORS_ADDRESSES
// #define TEST_LATENCY

#define RANGE 1
#define NUM_OF_SENSORS 4
#define RANGE_PERIOD 30

/*  
List of adresses for each sensor - after reset the address can be configured 
The I2C address have to be gap of 2, according to I2C protocal
Note some module may have hard-coded addresses, be aware of that 
when using along with other modules/sensors 
    */
#define address0 0x20
#define address1 0x22
#define address2 0x24
#define address3 0x26
#define address4 0x30

int16_t addresses[] = {address0, address1, address2, address3};

/*  These Arduino pins must be wired to the IO0 pin of VL6180x
    Don't use 13 as any enable pins as the LED_BUILTIN is hardwired 
    with pin 13...(Stupid, right?) */
int enablePins[]  = {9,3,12,2};



/* Create a new instance for each sensor */
VL6180X sensors[4];


void initPins(){
#ifdef SET_SENSORS_ADDRESSES    
    for ( int i = 0 ; i < NUM_OF_SENSORS; i++){
        pinMode(enablePins[i], OUTPUT);
        digitalWrite(enablePins[i], LOW);
    }
#endif    
	pinMode(LED_BUILTIN, OUTPUT);
}


/*
Initialize the sensors with preset addresses
This is not functioning yet...
*/

void InitSensor(VL6180X* sensor, uint8_t address){
    sensor->address = address;
    delay(50);
    sensor->init();
    sensor->configureDefault();
    Serial.println(sensor->readReg(0x212), HEX); // read I2C address
    sensor->stopContinuous();
    delay(300);
    sensor->startRangeContinuous(RANGE_PERIOD);
}

void SetAddrInitSensor(VL6180X* sensor, uint8_t address, int enablePin ){

    Serial.println("Starting Sensor with Address");
    digitalWrite(enablePin, HIGH);
    delay(50);
    sensor->init();
    sensor->configureDefault();
    sensor->setAddress(address);
    Serial.println(sensor->readReg(0x212), HEX); // read I2C address
    sensor->writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 5);
    sensor->setTimeout(500);
    sensor->stopContinuous();
    delay(300);
    sensor->startRangeContinuous(RANGE_PERIOD);
    delay(100);
}

void printSensor(int num, VL6180X sensor){
    /*
    Serial.print("#"); 
    Serial.print(num);
    Serial.print(": ");
    */
    Serial.print(sensor.readRangeContinuous());
    if (sensor.timeoutOccurred())
    {
        Serial.print(" TIMEOUT");
    }
    Serial.print(" "); 

}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
	{
		delay(1);
	}

    Wire.begin();
    initPins();

    digitalWrite(LED_BUILTIN, LOW);
#ifdef SET_SENSORS_ADDRESSES
    for( int i = 0; i < NUM_OF_SENSORS; i++){
        SetAddrInitSensor(&(sensors[i]),addresses[i],enablePins[i] );
    }
#else
    for( int i = 0; i < NUM_OF_SENSORS; i++){
        InitSensor(&(sensors[i]),addresses[i]);
    }
#endif

    
    delay(500);

    Serial.println("Sensors ready! Start reading sensors in 1 seconds ...!");
    delay(1000);
}

void loop()
{


    Serial.println();
    
    // use the build-in LED as an indicator of distance
    // and a medium of measuring the latency
#ifdef TEST_LATENCY
    int data = sensors[0].readRangeContinuous();
    if (data < 50){
        digitalWrite(LED_BUILTIN, HIGH);
    }else{
        digitalWrite(LED_BUILTIN, LOW);
    }
#else
    for( int i = 0; i < NUM_OF_SENSORS; i++){
        printSensor(i, sensors[i]);
    }
#endif

    /*
        print out the time each line to get the bandwidth
    */
 	//Serial.println(millis() % 1000);
    
    
    //delay(1);
}