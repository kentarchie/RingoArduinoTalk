/*
 Code to control the lights
*/

#ifndef CONTROLLIGHTS_H
#define CONTROLLIGHTS_H

#define PIXEL_DELAY 250
#define PIXEL_FLASH_DELAY 50
#define LIGHT_SENSE_DELAY 1500
#define LIGHT_AVERAGE_DELAY 125

#define NEW_RED 220
#define NEW_GREEN 30
#define NEW_BLUE 160

extern void LightsOnLoop();
extern void HeadLightsOn(int);
extern int GetRearLightSensor();

#endif
