/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

// Constant definitions based on the following hardware interface:
// System clock is 16MHz.
#define TOTAL_PERIOD 16000 //  16MHz/1000=16000

#define STOP 1
#define SPEED_15 TOTAL_PERIOD*.15
#define SPEED_20 TOTAL_PERIOD*0.2
#define SPEED_30 TOTAL_PERIOD*0.3
#define SPEED_35 TOTAL_PERIOD*.35
#define SPEED_40 TOTAL_PERIOD*0.4 //50%
#define SPEED_50 TOTAL_PERIOD*0.50 
#define SPEED_53 TOTAL_PERIOD*0.53
#define SPEED_80 TOTAL_PERIOD*0.8
#define TURN_SPEED 3200 // frequency * .2

static volatile uint8_t done;      

// PB5432 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.

//////////////////////1. Declarations Section////////////////////////////////
////////// Function Prototypes //////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
//	Initializes the PWM module 1 signals tied to PF321 on the Tiva Launchpad 
//		to allow for changing brightness of LEDs based on vehicle speed.
//	Initializes the PWM module 0 signals tied to PB76 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car.
void PWM_PB54_Init(void);

// Dependency: PWM_PB76_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB76 by changing the CMP registers
void PWM_PB54_Duty(unsigned long duty_L, unsigned long duty_R);

void Timer0A_Init(void);
/////////////////////////////////////////////////////////////////////////////