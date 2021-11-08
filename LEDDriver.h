/**************************************************
 * LEDDriver.h
 * Drives LED display
 *
 * Copyright 2015 University of Strathclyde
 *
 *
 **************************************************/
#include <msp430.h>
#include <driverlib.h>
#define PORT1_VECTOR            (47 * 2u) /* 0xFFE Port 1 */

extern void initialiseLedDial();  //Initialise LED Dial, setting GPIO parameters
extern void initialiseInputSwitch(); // Input Switch
extern int refreshLedDial(int speed); //Refresh the display
extern void setLedDial(unsigned char value); //Set dial value
extern bool getDirection(bool direction); // Find the direction of the cycle
extern void initialiseSW1();  // Switch 1
extern void initialiseADC();    // ADC