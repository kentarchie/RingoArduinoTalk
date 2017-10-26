/*

The base code of this sketch is from:
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

The code that does all the functions is written by Kent Archie
*/

#include "RingoHardware.h"

#define TRUE 1
#define FALSE 0

#define PIXEL_DELAY 250
#define PIXEL_FLASH_DELAY 50
#define LOOP_DELAY 1000
#define LIGHT_SENSE_DELAY 1500
#define LIGHT_AVERAGE_DELAY 125
#define NUM_LIGHT_READINGS 4

#define NEW_RED 220
#define NEW_GREEN 30
#define NEW_BLUE 160

int LightSensorValue = 0; 
int InitialAverageLight = 0; 
int TooDarkValue = 0; 
int AreHeadLightsOn = FALSE; 

int LoopCounter = 0;   // used in log printing

void GetRearLightSensors()
{
   LightSensorValue = analogRead(LightSense_Rear); // read Rear light sensor
} // GetRearLightSensors

void GetInitialLightValue() 
{
   printString("Getting Light Average");
   digitalWrite(Source_Select, HIGH); //This selects the top light sensors
   for (int i = 0 ; i < NUM_LIGHT_READINGS; ++i) {
      InitialAverageLight += analogRead(LightSense_Rear);
      delay(LIGHT_AVERAGE_DELAY);
   }
   InitialAverageLight = InitialAverageLight / 4;
   TooDarkValue = InitialAverageLight * .20;
   printValue("Base Light Level = ", InitialAverageLight);
   printValue("TooDarkValue = ", TooDarkValue);
} // GetInitialLightValue

void LightsOnLoop()
{
  for (int pixel = 0 ; pixel < NUM_PIXELS; ++pixel) {
      if((AreHeadLightsOn && ((pixel == EYE_LEFT) || (pixel == EYE_RIGHT))))
         continue;
      SetPixelRGB( pixel, NEW_RED, NEW_GREEN, NEW_BLUE);
      delay(PIXEL_FLASH_DELAY);
      OffPixel(pixel);
      delay(PIXEL_DELAY);
  }
} // LightsOnLoop

void HeadLightsOn(int averageLightValue) 
{
   SwitchButtonToPixels();
   if((averageLightValue < InitialAverageLight) && 
      ((InitialAverageLight - averageLightValue) > TooDarkValue))
      {
         printString("Too Dark");
         OnEyes(100, 100, 100); // turn on front lights
         AreHeadLightsOn = TRUE;
         
      }
      else {
         printString("Not Dark");
         //OffEyes();
         SetPixelRGB( 4, 0, 0, 0);
         SetPixelRGB( 5, 0, 0, 0);
         AreHeadLightsOn = FALSE;
         GetRearLightSensors();
      }
} // HeadLightsOn

// check the front left and right light sensors
// if the values are more than 20% higher than the base value
// start the motor on that side to move the Ringo in that direction
void FollowLight() 
{
   int leftSensorValue = 0, rightSensorValue = 0;
   //OffEyes(); // need front lights off to take readings
   SetPixelRGB( 4, 0, 0, 0);
   SetPixelRGB( 5, 0, 0, 0);
   Serial.println("Eyes Off");

   leftSensorValue  = analogRead(LightSense_Left);
   printValue("Left Sensor = ", leftSensorValue);
   rightSensorValue = analogRead(LightSense_Right);
   printValue("FollowLight: Right Sensor = ", rightSensorValue);

   if(AreHeadLightsOn) {
      GetRearLightSensors();
      HeadLightsOn(LightSensorValue);
   }
   printString("FollowLight: head light check reset");

   if((leftSensorValue > InitialAverageLight) && 
      ((leftSensorValue - InitialAverageLight ) > TooDarkValue))
      {
         printString("Turning Left 90 degrees");
         RotateAccurate(-90, 1500);
         SwitchMotorsToSerial(); //Call before using Serial.print functions as motors & serial share a line
         
         printString("Left move complete");
         PlayLeftTune();
         printString("Left tune complete");
      }
   if((rightSensorValue > InitialAverageLight) && 
      ((rightSensorValue - InitialAverageLight ) > TooDarkValue))
      {
         printString("Turning Right 90 degrees");
         RotateAccurate(90, 1500);
         SwitchMotorsToSerial(); //Call before using Serial.print functions as motors & serial share a line
         
         printString("Right move complete");
         PlayRightTune();
         printString("Right tune complete");
      }
} // FollowLight

void PlayRightTune()
{
   PlaySweep(1000, 4000, 330);
   OffChirp();
} // PlayRightTune

void PlayLeftTune()
{
   PlaySweep(4000, 1000, 330);
   //OffChirp();
} // PlayLeftTune

void printValue(char * label,int value) {
   //SwitchMotorsToSerial(); //Call before using Serial.print functions as motors & serial share a line
   Serial.print(LoopCounter);
   Serial.print(": ");
   Serial.print(label);
   Serial.println(value);
} // printValue

void printString(char * label) {
   //SwitchMotorsToSerial(); //Call before using Serial.print functions as motors & serial share a line
   Serial.print(LoopCounter);
   Serial.print(": ");
   Serial.println(label);
} // printValue

void setup()
{
   Serial.begin(57600);
   while (! Serial); // Wait until Serial is ready - Leonardo

   HardwareBegin();        //initialize Ringo's brain to work with his circuitry
   SwitchButtonToPixels(); //set shared line to control NeoPixel lights
   PlayStartChirp();       //Play startup chirp and blink eyes
   RestartTimer();
   delay(1000);
   NavigationBegin();

   GetInitialLightValue();

   printString("Setup Done");
} //setup

void loop()
{ 
   LoopCounter++;
  // delay(LIGHT_SENSE_DELAY);

   LightsOnLoop();

   digitalWrite(Source_Select, HIGH); //This selects the top light sensors
   GetRearLightSensors();

   printValue("Rear Light Sensor = ", LightSensorValue);

   HeadLightsOn(LightSensorValue);
   FollowLight();
   
   printString("Loop Done");
} // loop
