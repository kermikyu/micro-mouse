/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "GPIO.h"
#include "PWM.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////
////////// Constants //////////
#define PORTF_MASK      0x1E  //PF 1-4
#define SW1_MASK        0x10
#define DISABLE         0x00
#define DIR_MASK        0xCC

////////// Local Global Variables //////////
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: Initializes PB5432 for use with L298N motor driver direction
void PortB_Init(void){
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; //activate B clock
    while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!= SYSCTL_RCGC2_GPIOB){} //wait for clk
    GPIO_PORTB_AMSEL_R &= ~DIR_MASK; //disable analog function
    GPIO_PORTB_PCTL_R &= ~0xFF00FF00; //GPIO clear bit PCTL
		GPIO_PORTB_PCTL_R |= 0x44000000; 
    GPIO_PORTB_DIR_R |= DIR_MASK; //PE0-3 output
    GPIO_PORTB_AFSEL_R &= ~DIR_MASK; //no alternate function
    GPIO_PORTB_DEN_R |= DIR_MASK; //enable digital pins PE0-3
}

// Port F Initialization for LEDs
void LED_Init(void){ 
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Activate F clocks
    while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
        
    GPIO_PORTF_AMSEL_R &= ~0x0E;      // disable analog function
    GPIO_PORTF_PCTL_R &= ~0x0000FFF0;  // GPIO clear bit PCTL  
    GPIO_PORTF_DIR_R |= 0x0E;          // PF1-PF3 output
    GPIO_PORTF_AFSEL_R &= ~0x0E;       // no alternate function     
    GPIO_PORTF_DEN_R |= 0x0E;          // enable digital pins PF3-PF1
    LED = 0;                         // Turn off all LEDs
}

// Initialize SW1 on PF4
void Switch_Init(void){
    unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // (a) activate clock for port F
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // unlock GPIO Port F
  GPIO_PORTF_CR_R |= 0x1F;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_DIR_R |= 0x0E;     // make PF1-3 out
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x1F;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x1F;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R |= 0x11;    //     PF4,PF0 rising edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // (g) bits:23-21 for PORTF, set priority to 5
  NVIC_EN0_R |= 0x40000000;      // (h) enable interrupt 30 in NVIC
}

// Switch Interrupt Handler
//void Switch_Handler(void) {
//    GPIO_PORTF_ICR_R = 0x10;  // Clear interrupt flag
//    done = 0;  // Reset done to 0 to restart the loop
//}

/////////////////////////////////////////////////////////////////////////////