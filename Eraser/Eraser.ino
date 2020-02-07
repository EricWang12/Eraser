
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
#include <ArduinoBLE.h>


#define ROW_1 8
#define ROW_2 11
#define ROW_3 6
#define ROW_4 12
#define ROW_5 A3
#define ROW_6 5
#define ROW_7 A1
#define ROW_8 2

#define COL_1 A2
#define COL_2 10
#define COL_3 9
#define COL_4 3
#define COL_5 7
#define COL_6 4
#define COL_7 A0
#define COL_8 13
/*
  3                             4
  |-----------------------------|
  |                  |  | ///// |
  |                  |  | \\\\\ |
  |                  |  | ///// |
  |-----------------------------|
  2                             1


*/
const byte rows[] = {
    ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte col[] = {
  COL_1,COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};



// The display buffer
// It's prefilled with a smiling face (1 = ON, 0 = OFF)
byte ALL[] = {B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
byte ZERO[] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
byte unsigned encode[] = {B00000000,B00000001,B00000011,B00000111,B00001111,B00011111,B00111111,B01111111, B11111111};


float timeCount = 0;

byte ret[8] = {};



const int bias[] = {800,1100,0,300};
const float slope[] = {1,1,0.55,0.65};

const float VCC = 3.3; 
const float R_DIV = 1000.0; 
int LEDPin[] = {12, 11, 10, 9, 8, 7, 6, 5, 4}; // column pin (cathode) of LED Matrix
const int ledNum = sizeof(LEDPin) / sizeof(int);
const int maxForce = 2000;
const int maxIndForce = 1300;
const int max_level = 7;
const int caliberation = 600;
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address



void getMatrix(int UL, int UR, int LL, int LR){
  ret[8] = {};
  int count = 0;
  byte unsigned Temp = 0xFF;
  
  for(int i = 0 ; i < 8; i++){
    ret[i] = 0;
  }
  //ret = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
  for(int i = UL ; i > 0; i--){   
    ret[count] |=  ~encode[8-i];
    count ++;
  }
  count = 0;
  for(int i = UR ; i > 0 ; i--){   
    ret[count] |=  encode[i];
    count ++;
  }

  count = 7;
  for(int i = LL ; i > 0 ; i--){   
    ret[count] |=  ~encode[8-i];
    count --;
  }
  count = 7;
  for(int i = LR ; i > 0 ; i--){   
    ret[count] |=  encode[i];
    count --;
  }
  for(int i = 0 ; i < 8; i++){
    ret[i] = ~ret[i];
  }
}


void setup()
{



    for (byte i = 2; i <= 13; i++)
      pinMode(i, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  // Initialize the BLE stuff
  BLE.begin();
	BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  getMatrix(2,1,6,3);
}

void loop()
{
  // wait for a BLE central
  //BLEDevice central = BLE.central();
  BLEDevice peripheral = BLE.available();
  int16_t force1 = (int16_t)getForce(analogRead(A4));
  int16_t force2 = (int16_t)getForce(analogRead(A5));
  int16_t force3 = (int16_t)getForce(analogRead(A6));
  int16_t force4 = (int16_t)getForce(analogRead(A7));
  
  force1 = slope[0]*force1 - bias[0] < 0? 0: slope[0]*force1 - bias[0];
  force2 = slope[1]*force2 - bias[1] < 0? 0: slope[1]*force2 - bias[1];
  force3 = slope[2]*force3 - bias[2] < 0? 0: slope[2]*force3 - bias[2];
  force4 = slope[3]*force4 - bias[3] < 0? 0: slope[3]*force4 - bias[3];
  
  int totalForce = force1 + force2 + force3 + force4;// < caliberation ? 0 : force1 + force2 + force3 + force4  - caliberation;
  int level;
  if (totalForce >= maxForce)level = ledNum;
  else level = map(totalForce, 0, maxForce, 0, ledNum);

  
	if (peripheral)
	{

		BLE.stopScan();
		SendData(peripheral, 105,28,36,192);
		BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
	}

/*
  Serial.print("" );
  Serial.print(force1);
  Serial.print(" " );
  Serial.print(force2);
    Serial.print(" " );
  //Serial.print("  #3: " );
  Serial.print(force3);
    Serial.print(" " );
  //Serial.print("  #4: " );
  Serial.print(force4);
   Serial.print(" " );
  //Serial.print("    total: " );
  Serial.println(totalForce);
*/
  int level_1 = force1 >= maxForce ? max_level : map(force1, 0, maxIndForce, 0, max_level);
  int level_2 = force2 >= maxForce ? max_level : map(force2, 0, maxIndForce, 0, max_level);
  int level_3 = force3 >= maxForce ? max_level :map(force3, 0, maxIndForce, 0, max_level);
  int level_4 = force4 >= maxForce ? max_level :map(force4, 0, maxIndForce, 0, max_level);
  
  
  getMatrix( level_2,level_3, level_1, level_4);
  drawScreen(ret);
  delay(5);
  // if a central is connected to the peripheral:
}


float getForce(int fsrADC ) {

  float force = 0;
  if (fsrADC != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC * VCC / 1023.0;
    // Use voltage and static resistor value to
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    // Serial.println("Resistance: " + String(fsrR) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    //float force;
    float fsrG = 1.0 / fsrR; // Calculate conductance
    // Break parabolic curve down into two linear slopes:
    if (fsrR <= 600)
      force = (fsrG - 0.00075) / 0.00000032639;
    else
      force =  fsrG / 0.0000003857142;
  }

  return force;
}
void  drawScreen(byte buffer2[])
 { 
   // Turn on each row in series
    for (byte i = 0; i < 8; i++)        // count next row
     {
        digitalWrite(rows[i], HIGH);    //initiate whole row
        for (byte a = 0; a < 8; a++)    // count next row
        {
          // if You set (~buffer2[i] >> a) then You will have positive
          digitalWrite(col[a], (buffer2[i] >> a) & 0x01); // initiate whole column
          
          delayMicroseconds(100);       // uncoment deley for diferent speed of display
          //delayMicroseconds(1000);
          //delay(10);
          //delay(100);
          
          //digitalWrite(col[a], 1);      // reset whole column
        }
        digitalWrite(rows[i], LOW);     // reset whole row
        // otherwise last row will intersect with next row
    }
}
void  undrawScreen(byte buffer2[])
 { 
   // Turn on each row in series
    for (byte i = 0; i < 8; i++)        // count next row
     {
        digitalWrite(rows[i], HIGH);    //initiate whole row
        for (byte a = 0; a < 8; a++)    // count next row
        {
          // if You set (~buffer2[i] >> a) then You will have positive
          digitalWrite(col[a], (buffer2[i] >> a) & 0x01); // initiate whole column
          
          delayMicroseconds(100);       
          
          digitalWrite(col[a], 1);      // reset whole column
        }
        digitalWrite(rows[i], LOW);     // reset whole row
        // otherwise last row will intersect with next row
    }
}
void SendData(BLEDevice peripheral, int16_t force1 , int16_t force2, int16_t force3, int16_t force4){
  
	if (!peripheral.connect())
	{
		return;
	}

	if (!peripheral.discoverAttributes())
	{
		peripheral.disconnect();
		return;
	}

	// retrieve the LED characteristic
	BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");


	while (peripheral.connected())
	{
		int16_t count[] = {force1 ,  force2,  force3,  force4};
		ledCharacteristic.writeValue(count, 8);
		delay(100);
	}

}