/*
 Code to control the lights
*/

#include "ControlLights.h"

int GetRearLightSensor()
{
   SetPixelRGB(TAIL_TOP,0,0,0); // turn off back light
   int lightValue = analogRead(LightSense_Rear); // read Rear light sensor
   return(lightValue);
} // GetRearLightSensor

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
         //GetRearLightSensors();
      }
} // HeadLightsOn

