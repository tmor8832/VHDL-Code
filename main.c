/**************************************************
 * EE312 Lab 4
 *
 * Copyright 2015 University of Strathclyde
 *
 *
 **************************************************/
#include <msp430.h>
#include <driverlib.h>
#include "LedDriver.h"

#define P1IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P1IV_P1IFG0          (0x0002u)    /* P1IV P1IFG.0 */
#define P1IV_P1IFG1          (0x0004u)    /* P1IV P1IFG.1 */
#define P1IV_P1IFG2          (0x0006u)    /* P1IV P1IFG.2 */
#define P1IV_P1IFG3          (0x0008u)    /* P1IV P1IFG.3 */
#define P1IV_P1IFG4          (0x000Au)    /* P1IV P1IFG.4 */
#define P1IV_P1IFG5          (0x000Cu)    /* P1IV P1IFG.5 */
#define P1IV_P1IFG6          (0x000Eu)    /* P1IV P1IFG.6 */
#define P1IV_P1IFG7          (0x0010u)    /* P1IV P1IFG.7 */

bool SW1_interruptFlag_ = 0;
bool ADC_interruptFlag_ = 0;
int delayVariable = 1;
int ADCValue = 0;
bool dir;

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
  switch(__even_in_range(P1IV,P1IV_P1IFG7))
  {
    case P1IV_P1IFG3:               // it is SW1
      SW1_interruptFlag_ = 1;           //Set flag
      GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);             //Clear the interrupt
    break;
  }
}

// ADC interrupt service routine
#pragma vector=ADC_VECTOR           // these two lines are used by IAR and CCC
__interrupt void ADC_ISR(void)
{
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
  {
    case ADCIV_ADCIFG:              // conversion complete
        ADC_interruptFlag_ = 1;
        ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);
    break;
  }
}

int main(void)
{
  //Default MCLK = 1MHz
  
  unsigned int i = 0;
  unsigned char dialValue = 0x01;
  WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

  initialiseLedDial();
  initialiseSW1();
  initialiseADC();
  __enable_interrupt();
  
  // Disable the GPIO power-on default high-impedance mode
  // to activate previously configured port settings
  PMM_unlockLPM5();
  
  while(1)
  {
    ADCCTL0 |= ADCENC | ADCSC;
    
    if (SW1_interruptFlag_)
    {
        SW1_interruptFlag_ = 0;
        dir = !dir;
        __delay_cycles(10000);
    }
    
      if (ADC_interruptFlag_)
    {
       ADC_interruptFlag_ = 0;
       ADCValue = ADCMEM0;
       
       if (ADCValue >= 256)
         delayVariable = 1;
           
       if (ADCValue >= 512)
         delayVariable = 2;
           
       if (ADCValue >= 768)
         delayVariable = 3; 
     }
      
    if (dir == 0)     // anticlockwise
    {
      dialValue = dialValue * 0x02;
      
      if(0x00 == dialValue)
        dialValue = 0x01;
    
      //Set value
      setLedDial(dialValue);
    
      //Refresh display (10 times for each position)
      for(i = 0; i < 10; i++)
      refreshLedDial(delayVariable);
     }
      
     if (dir == 1)        // clockwise
     {
         dialValue = dialValue / 0x02;
        
          if(0x00 == dialValue)
            dialValue = 0x80;
        
          //Set value
          setLedDial(dialValue);
    
          //Refresh display (10 times for each position)
          for(i = 0; i < 10; i++)
          refreshLedDial(delayVariable);
     }
    
  }
}

