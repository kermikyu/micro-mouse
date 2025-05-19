/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "GPIO.h"

////////// Local Global Variables //////////
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
// Initializes the PWM module 0 generator 0 outputs A&B tied to PB76 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car.
void PWM_PB54_Init(void){
    // PWM and GPIO clock initialization
    SYSCTL_RCGCPWM_R |= 0x01;     // activate PWM0
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
    while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
    
    SYSCTL_RCC_R &= ~0x001E0000;  // Clear PWM divider
    
    // Configure PB4-5 for PWM
    GPIO_PORTB_AFSEL_R |= 0x30;     // enable alt function on PB4-5
    GPIO_PORTB_PCTL_R &= ~0x00FF0000; // clear PCTL
    GPIO_PORTB_PCTL_R |= 0x00440000;  // PWM for PB4-5
    GPIO_PORTB_DEN_R |= 0x30;       // enable digital I/O on PB4-5
    
    // Configure PWM0 Generator 1
    PWM0_1_CTL_R = 0;              // stop counter
    PWM0_1_GENA_R = 0x0000008C;    // PB4 (M0PWM2) output set when reload, clear when match
    PWM0_1_GENB_R = 0x0000080C;    // PB5 (M0PWM3) output set when reload, clear when match
    PWM0_1_LOAD_R = TOTAL_PERIOD - 1; // cycles needed to count down to 0
    PWM0_1_CMPA_R = 0;             // count value when output rises
    PWM0_1_CMPB_R = 0;             // count value when output rises
    PWM0_1_CTL_R |= 0x00000001;    // start PWM0 Generator 1
    PWM0_ENABLE_R |= 0x0000000C;  // disable outputs on initialization
}


// Dependency: PWM_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB76 by changing the CMP registers
void PWM_PB54_Duty(unsigned long duty_L, unsigned long duty_R){
	PWM0_1_CMPA_R = TOTAL_PERIOD-duty_L - 1;	// PB4 count value when output rises COMPA
  PWM0_1_CMPB_R = TOTAL_PERIOD-duty_R - 1;	// PB5 count value when output rises COMPB
}