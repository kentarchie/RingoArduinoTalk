/*
 serial print functions
*/

#include "Utilities.h"

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

