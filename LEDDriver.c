/**************************************************
 * LEDDriver.c
 * Drives LED display
 *
 * Copyright 2015 University of Strathclyde
 *
 *
 **************************************************/

#include "LedDriver.h"

//Current dial value
unsigned char ledValue_ = 0;

/**
Initialise LED Dial, setting GPIO parameters
*/
void initialiseLedDial()
{
  //GPIO 2.7
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
    
  //GPIO 5.1, 5.2, 5.3
  GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN1);
  GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN2);
  GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN3);
    
  //GPIO 8.0
  GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN0);
}

/**
Initialise Input Switch
*/
void initialiseInputSwitch()
{
  // Switch 1
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3);
}

// Configure button SW1 interrupt
void initialiseSW1()
{
  GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN3, GPIO_LOW_TO_HIGH_TRANSITION);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3);  

  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);

  GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN3);
}

// Configure ADC
void initialiseADC()
{
  GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P8, GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

  ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);
  ADC_enableInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);
  
  ADCCTL0 |= 0x10;
  ADCCTL1 |= 0x200;
  ADCCTL2 |= 0x04;
  ADCMCTL0 |= 0x09;
}

/** 
Refresh the display
*/
int refreshLedDial(int speed)
{
  //Refresh lower bank (LEDs 1-4)
  GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN7);       // sets bottom line for leds
  
  // For LED 1
  if (ledValue_& 0x01)  //0000 0001
    GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN0);    // high for on
  else
    GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN0);
  
  // For LED 2
  if (ledValue_& 0x02)  //0000 0010
    GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN1);    // high for on
  else
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN1);
  
  // For LED 3
  if (ledValue_& 0x04)  //0000 0100
    GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN2);    // high for on
  else
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN2);
  
  // For LED 4
  if (ledValue_& 0x08)  //0000 1000
    GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN3);     // high for on
  else
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN3);

  // delays
  switch(speed)
  {
    case 1:
      __delay_cycles(10000);
      break;
    
    case 2:
      __delay_cycles(5000);
      break;
    
    case 3:
      __delay_cycles(2500);
      break;
  }
    
  //Refresh lower bank (LEDs 5-9)
  GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN7);
  
  // For LED 8
  if (ledValue_& 0x10)  //0001 0000
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN3);     // low for on 
  else
    GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN3);
  
  // For LED 7
  if (ledValue_& 0x20)  //0010 0000
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN2);     // low for on
  else
    GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN2);
  
  // For LED 6
  if (ledValue_& 0x40)  //0100 0000
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN1);     // low for on
  else
    GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN1);
    
  // For LED 5
  if (ledValue_& 0x80)  //1000 0000
    GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN0);    // low for on
  else
    GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN0);

  // delays
  switch(speed)
  {
    case 1:
      __delay_cycles(10000);
      break;
    
    case 2:
      __delay_cycles(5000);
      break;
    
    case 3:
      __delay_cycles(2500);
      break;
  }
  
   return 0;
}

/** 
Set dial value
*/
void setLedDial(unsigned char value)
{
  ledValue_ = value;
}
