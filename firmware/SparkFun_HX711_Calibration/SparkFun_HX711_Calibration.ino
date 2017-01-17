/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.
 
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch
 
 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).
 
 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/

#include "HX711.h"

void help(void);
void showScaleInfo(void);

#define DOUT  11
#define CLK  10

// default type is LBS (last option within if else macro
#define TYPE_KG 1
#define TYPE_LBS 2
#define SELECTED_TYPE TYPE_KG


HX711 scale(DOUT, CLK);

#if SELECTED_TYPE == TYPE_KG
  //-7050 / 0.453592 (1 lbs = 0.453592 kg). initial calibration factor
  float calibration_factor = -13080; // -12653 calibration factor found 
  char *weightUnitText = " kg";
#else
  float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup
  char *weightUnitText = " lbs";
#endif

// give time to read
int readdelay = 10000; // msec


void setup() {
  help();
  
  scale.set_scale();
  scale.tare();	//Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  Serial.print(scale.get_units(9), 3);
  Serial.print(weightUnitText); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == 's')
      calibration_factor += 100;
    else if(temp == 'd')
      calibration_factor += 1000;
    if(temp == '+' || temp == 'z')
      calibration_factor -= 10;
    else if(temp == 'x')
      calibration_factor -= 100;
    else if(temp == 'c')
      calibration_factor -= 1000;

    if (temp == 't') {
      // "resets" the scale to 0 (works also with weight added to scale)
      scale.tare();  
    } else if (temp == '?') {
      help();
    } else if (temp == 'i') {
      showScaleInfo();
    }
  }
}

void help() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press +(+10) or a(+10),s(+100),d(+1000) to increase calibration factor");
  Serial.println("Press -(-10) or z(-10),x(-100),c(-1000) to decrease calibration factor");
  Serial.println("Press t for resetting base");
  Serial.println("Press i for scale config info");
  Serial.println("press ? for help");
  
  delay(readdelay);
}

void showScaleInfo() {
      // scale value is used to return weight in grams, kg, ounces, whatever (is rawvalue/ ???)
      Serial.print("scaling val: ");
      Serial.println(scale.get_scale());

      Serial.print("current offset: ");
      Serial.println(scale.get_offset());

      Serial.print("current val : ");
      Serial.println(scale.read_average());

      Serial.print("current value (read_average() - Offset): ");
      Serial.println(scale.get_value());

      Serial.print("get units (get_value/OFFSET): ");
      Serial.println(scale.get_units());

      delay(readdelay);
}

