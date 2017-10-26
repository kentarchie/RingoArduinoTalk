/*

Ringo Robot
Ringo_Base_Sketch_02
Version 2.0 8/2015

This is a basic sketch that can be used as a starting point
for various functionality of the Ringo robot.

This code was written by Plum Geek LLC with most
significant portions written by Dustin Soodak.
Portions from other open source projects where noted.
This code is licensed under:
Creative Commons Attribution-ShareAlike 2.0 Generic (CC BY-SA 2.0)
https://creativecommons.org/licenses/by-sa/2.0/
Visit http://www.plumgeek.com for Ringo information.
Visit http://www.arduino.cc to learn about the Arduino.

*/

#include "RingoHardware.h"

#define TRUE 1
#define FALSE 0

#define PIXEL_DELAY 250
#define PIXEL_FLASH_DELAY 50
#define LOOP_DELAY 1000
#define LIGHT_SENSE_DELAY 1500
#define LIGHT_AVERAGE_DELAY 125

#define NEW_RED 220
#define NEW_GREEN 30
#define NEW_BLUE 160

int RightSensorValue = 0; 
int LeftSensorValue = 0; 
int InitialAverageLight = 0; 
int TooDarkValue = 0; 
int AreHeadLightsOn = FALSE; 

void GetLightSensors()
{
   RightSensorValue = analogRead(LightSense_Right); // read right light sensor
   LeftSensorValue  = analogRead(LightSense_Left);  // read left light sensor
} // GetLightSensors

void LightsOn()
{
  for (int pixel = 0 ; pixel < NUM_PIXELS; ++pixel) {
      if((AreHeadLightsOn && ((pixel == EYE_LEFT) || (pixel == EYE_RIGHT))))
         continue;
      SetPixelRGB( pixel, NEW_RED, NEW_GREEN, NEW_BLUE);
      delay(PIXEL_FLASH_DELAY);
      OffPixel(pixel);
      delay(PIXEL_DELAY);
  }
} // LightsOn

void HeadLightsOn(int averageLightValue) 
{
   SwitchButtonToPixels();
   if((averageLightValue < InitialAverageLight) && 
      ((InitialAverageLight - averageLightValue) > TooDarkValue))
      {
         Serial.println("Too Dark");
         OnEyes(200, 200, 200);
         AreHeadLightsOn = TRUE;
         
      }
      else {
         Serial.println("Not Dark");
         OffEyes();
         AreHeadLightsOn = FALSE;
         GetLightSensors();
      }
} // HeadLightsOn

void PlayRightTune()
{
   PlaySweep(1000, 4000, 330);
   OffChirp();
} // PlayRightTune

void PlayLeftTune()
{
   PlaySweep(4000, 1000, 330);
   OffChirp();
} // PlayLeftTune

void printValue(char * label,int value) {
   Serial.print(label);
   Serial.println(value);
} // printValue

void setup()
{
   Serial.begin(57600);
   while (! Serial); // Wait until Serial is ready - Leonardo

   HardwareBegin();        //initialize Ringo's brain to work with his circuitry
   SwitchButtonToPixels(); //set shared line to control NeoPixel lights
   PlayStartChirp();       //Play startup chirp and blink eyes
   SwitchMotorsToSerial(); //Call before using Serial.print functions as motors & serial share a line
   RestartTimer();

   Serial.println("Getting Light Average");
   digitalWrite(Source_Select, HIGH); //This selects the top light sensors
   for (int i = 0 ; i < 4; ++i) {
      InitialAverageLight += (analogRead(LightSense_Right) + analogRead(LightSense_Left)) /2;
      delay(LIGHT_AVERAGE_DELAY);
   }
   InitialAverageLight = InitialAverageLight / 4;
   TooDarkValue = InitialAverageLight * .20;
   printValue("Base Light Level = ", InitialAverageLight);
   printValue("too Dark Light Level = ", TooDarkValue);

  Serial.println("Setup Done");
} //setup

void loop()
{ 
   int averageLightValue = 0;
   delay(LIGHT_SENSE_DELAY);

   //PlayLeftTune();
   //PlayRightTune();

   LightsOn();

   digitalWrite(Source_Select, HIGH); //This selects the top light sensors
   if(!AreHeadLightsOn) {
      GetLightSensors();
   }
   averageLightValue = (RightSensorValue + LeftSensorValue) / 2;

   printValue("Left Light Sensor = ", LeftSensorValue);
   printValue("Right Light Sensor = ", RightSensorValue);
   printValue("Average Light Sensor = ", averageLightValue);

   HeadLightsOn(averageLightValue);
   
   Serial.println("Loop Done");
} // loop
