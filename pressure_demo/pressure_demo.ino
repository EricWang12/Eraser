
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const float VCC = 3.3; 
const float R_DIV = 1000.0; 
int LEDPin[] = {12, 11, 10, 9, 8, 7, 6, 5, 4}; // column pin (cathode) of LED Matrix
const int ledNum = sizeof(LEDPin) / sizeof(int);
const int maxForce = 2000;
const int caliberation = 600;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup()
{

  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("hello everyone");
  lcd.setCursor(1, 1);
  lcd.print("konichiwaa");
  Serial.begin(9600); // initialize serial communication
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.println("Bluetooth device active, waiting for connections...");
  for (int i = 0; i < ledNum; i++) {
    pinMode(LEDPin[i], OUTPUT);
  }
}

void loop()
{
  // wait for a BLE central
  //BLEDevice central = BLE.central();

  int force1 = (int)getForce(analogRead(A0));
  int force2 = (int)getForce(analogRead(A1));
  int totalForce = force1 + force2 < caliberation ? 0 : force1 + force2 - caliberation;
  int level;
  if (totalForce >= maxForce)level = ledNum;
  else level = map(totalForce, 0, maxForce, 0, ledNum);
  Serial.print("#1: " );
  Serial.print(force1);
  Serial.print("    #2: " );
  Serial.print(force2);
  Serial.print("    total: " );
  Serial.println(totalForce);
  int i = 0;
  for (; i < level; i++) {
    digitalWrite(LEDPin[i], HIGH);
  }
  for (; i <  ledNum; i++) {
    digitalWrite(LEDPin[i], LOW);
  }
  delay(200);
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
    Serial.println("Resistance: " + String(fsrR) + " ohms");
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
