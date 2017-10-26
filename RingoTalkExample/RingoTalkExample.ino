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
#include "Utilities.h"
#include "ControlLights.h"

#define TRUE 1
#define FALSE 0

#define LOOP_DELAY 1000
#define NUM_LIGHT_READINGS 4

int InitialAverageLight = 0; 
int TooDarkValue = 0; 
int AreHeadLightsOn = FALSE; 

int LoopCounter = 0;   // used in log printing

void GetInitialLightValue() 
{
   printString("Getting Light Average");
   digitalWrite(Source_Select, HIGH); //This selects the top light sensors
   for (int i = 0 ; i < NUM_LIGHT_READINGS; ++i) {
      InitialAverageLight += GetRearLightSensor();
      delay(LIGHT_AVERAGE_DELAY);
   }
   InitialAverageLight = InitialAverageLight / NUM_LIGHT_READINGS;
   TooDarkValue = InitialAverageLight * .20;
   printValue("Base Light Level = ", InitialAverageLight);
   printValue("TooDarkValue = ", TooDarkValue);
} // GetInitialLightValue


// check the front left and right light sensors
// if the values are more than 20% higher than the base value
// start the motor on that side to move the Ringo in that direction
void FollowLight() 
{
   int leftSensorValue = 0, rightSensorValue = 0;
   OffEyes(); // need front lights off to take readings
   //SetPixelRGB( 4, 0, 0, 0);
   //SetPixelRGB( 5, 0, 0, 0);
   printString("Eyes Off");

   leftSensorValue  = analogRead(LightSense_Left);
   printValue("FollowLight: Left Sensor = ", leftSensorValue);
   rightSensorValue = analogRead(LightSense_Right);
   printValue("FollowLight: Right Sensor = ", rightSensorValue);

   if(AreHeadLightsOn) {
      HeadLightsOn(GetRearLightSensor());
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
   int lightSensorValue = 0; 
   LoopCounter++;
  // delay(LIGHT_SENSE_DELAY);

   LightsOnLoop();

   digitalWrite(Source_Select, HIGH); //This selects the top light sensors
   lightSensorValue = GetRearLightSensor();

   printValue("Rear Light Sensor = ", lightSensorValue);

   HeadLightsOn(lightSensorValue);
   FollowLight();
   
   printString("Loop Done");
   /*
   if(LoopCounter > 30) {
      printString("Ringo Shutdown");
      exit(0);
   }
   */
} // loop
