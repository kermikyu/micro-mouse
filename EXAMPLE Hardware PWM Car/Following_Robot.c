// FollowingRobot.c
// Runs TM4C123
// Starter program CECS 347 project 2 - A Follwoing Robot
// by Min He, 03/17/2024
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS2.h"
#include "SysTick.h"
#include "GPIO.h"
#include "PWM.h"
#include "PLL.h"

#define SW_MASK 		0x11
#define LED_MASK		0x0E
#define SWLED_MASK	0x1F

#define LED 	(*((volatile unsigned long *)0x40025038))
#define RED 		0x02
#define GREEN 	0x08
#define BLUE 		0x04

enum robot_modes {OBJECT_FOLLOWER, WALL_FOLLOWER};
enum activity {ACTIVE, INACTIVE};
enum activity curr = INACTIVE;
enum robot_modes mode= OBJECT_FOLLOWER;
enum dir {LEFT, RIGHT};
// external functions
extern void DisableInterrupts(void);
extern void EnableInterrupts(void); // Enable interrupts
extern void WaitForInterrupt(void); // low power mode

// functions defined in this file
void System_Init(void);
void object_follower(void);
void wall_follower(void);
uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3);
void ReadADCMedianFilter(uint16_t *left, uint16_t *middle, uint16_t *right);
uint16_t left, middle, right;
volatile uint32_t mode_changed = 0;
unsigned long object;
uint8_t closest(unsigned long left, unsigned long middle, unsigned long right); // (ADDED)
// Definitons
#define TOO_FAR 		(1250)  // replace the zero with the ADC output value for maximum distance (~20cm)
#define FOLLOW_DIST (1593)  // replace the zero with the ADC output value for object following distance 
#define TOO_CLOSE 	(2501)  // replace the zero with the ADC output value for minimum distance (~5cm)
#define EXIT				(3000)
unsigned long object, i, count;
enum dir state = LEFT;

int main(void){	
	System_Init();
	count = 0;
	curr = INACTIVE;
	mode = OBJECT_FOLLOWER;
	LED = RED;
  while(1){
		if (curr == ACTIVE)
		{
			for (i=0;i<10;i++) {
				ReadADCMedianFilter(&left, &middle, &right);
			}
			switch (mode) {
				case OBJECT_FOLLOWER:
					LED = BLUE;
					object_follower();
					break;
				case WALL_FOLLOWER:
					LED = GREEN;
					wall_follower();
					break;
				default:
					WaitForInterrupt();
					break;				
			}
		}
		else
		{
			LED = RED;
			PWM_PB54_Duty(STOP, STOP);
			WaitForInterrupt();
		}
	}
}


void System_Init(void){
	DisableInterrupts();
	PLL_Init(); // set system clock to 16 MHz
	PortB_Init();
	Switch_Init();
	ADC1_SS2_Init(); // Initialize ADC1 Sample sequencer 1
	PWM_PB54_Init();
	EnableInterrupts();
}

void object_follower(void) {
    // Calibration
    for(uint8_t i=0; i<10; i++) {
        ReadADCMedianFilter(&left, &middle, &right);
    }
		PWM_PB54_Duty(STOP, STOP);
		do { ReadADCMedianFilter(&left, &middle, &right); }
		while (left > TOO_CLOSE || middle > TOO_CLOSE || right > TOO_CLOSE);

	object = closest(left, middle, right);
	
	switch(object)
	{
		case 0:	// Front Sensor
			if (middle > FOLLOW_DIST+250){
				WHEEL_DIR = BACKWARD;
			  PWM_PB54_Duty(SPEED_30, SPEED_30);
				
			} else if (middle < FOLLOW_DIST-250){
					WHEEL_DIR = FORWARD;
					PWM_PB54_Duty(SPEED_30, SPEED_30);
				
			}else{
				PWM_PB54_Duty(STOP,STOP); 
				
			} break;
			
		case 1: // Left Sensor
			if (left > FOLLOW_DIST) { // Too Close
					WHEEL_DIR = BACKWARD;
					PWM_PB54_Duty(SPEED_20, SPEED_30);
			}else if (left < FOLLOW_DIST){ // Too Far
					WHEEL_DIR = FORWARD;
					PWM_PB54_Duty(SPEED_15, SPEED_30);
			} break;
		case 2:	// Right Sensor
			if (right > FOLLOW_DIST) { // Too close
					WHEEL_DIR = BACKWARD;
					PWM_PB54_Duty(SPEED_30, SPEED_20);
			}else{
					WHEEL_DIR = FORWARD;
					PWM_PB54_Duty(SPEED_30, SPEED_15);
			} break;
		default:
			PWM_PB54_Duty(STOP, STOP); // Stop
			break;
	}
}
void wall_follower(void){
   // Calibration
   for(uint8_t i=0; i<10; i++) {
        ReadADCMedianFilter(&left, &middle, &right);
   }
		do { ReadADCMedianFilter(&left, &middle, &right); }
		while (left > TOO_CLOSE || middle > TOO_CLOSE || right > TOO_CLOSE);	
	if(middle > (FOLLOW_DIST-370)){ // if we are close to the wall, turn left/right depending on state. 
		if (state == LEFT){ // if we are on LEFT side 
			//PWM_PB54_Duty(STOP, STOP);
			WHEEL_DIR = RIGHTPIVOT;
			PWM_PB54_Duty(SPEED_50, SPEED_50);
			//for(i=0; i<40000; i=i+1);	
	 }else{ // if we are on RIGHT side
		 //	PWM_PB54_Duty(STOP, STOP);
			WHEEL_DIR = LEFTPIVOT;
			PWM_PB54_Duty(SPEED_50, SPEED_50);
			//for(i=0; i<40000; i=i+1);		
		}
	}
	
	else if ((state == LEFT)&&(left > TOO_CLOSE-400)){ // If we start on left and left sensor too close to wall
		PWM_PB54_Duty(SPEED_30, SPEED_20); // turn right
	}
	else if((state == RIGHT)&&(right > TOO_CLOSE-400)){ // if we start right and right sensor too close to wall
		PWM_PB54_Duty(SPEED_20, SPEED_30); // turn left
	}
	else if((left < TOO_FAR-300) && (right < TOO_FAR-300)){ // if no object infront of sensor, u-turn
		if(state == LEFT){ // u - turn based on which side we are on
			PWM_PB54_Duty(SPEED_20, SPEED_50);
		}else{
			PWM_PB54_Duty(SPEED_50, SPEED_20);
		}
	}
	else{ // move forwawrd by defualt.
		WHEEL_DIR= FORWARD;
		PWM_PB54_Duty(SPEED_50, SPEED_50);
	}        
}


uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3){
uint16_t result;
	
  if(u1>u2)
    if(u2>u3)   result=u2;     // u1>u2,u2>u3       u1>u2>u3
      else
        if(u1>u3) result=u3;   // u1>u2,u3>u2,u1>u3 u1>u3>u2
        else      result=u1;   // u1>u2,u3>u2,u3>u1 u3>u1>u2
  else
    if(u3>u2)   result=u2;     // u2>u1,u3>u2       u3>u2>u1
      else
        if(u1>u3) result=u1;   // u2>u1,u2>u3,u1>u3 u2>u1>u3
        else      result=u3;   // u2>u1,u2>u3,u3>u1 u2>u3>u1
  return(result);
}

void ReadADCMedianFilter(uint16_t *left, uint16_t *middle, uint16_t *right) {
    static uint16_t old_left = 0, mid_left = 0;
    static uint16_t old_mid = 0, mid_mid = 0;
    static uint16_t old_right = 0, mid_right = 0;
    uint16_t new_left, new_mid, new_right;

    // Get new raw values
    ADC1_InSeq2(&new_left, &new_mid, &new_right);

    // Apply median filter to each channel
    *left = median(new_left, mid_left, old_left);
    *middle = median(new_mid, mid_mid, old_mid);
    *right = median(new_right, mid_right, old_right);

    // Update history buffers
    old_left = mid_left;
    mid_left = new_left;
    
    old_mid = mid_mid;
    mid_mid = new_mid;
    
    old_right = mid_right;
    mid_right = new_right;
}

uint8_t closest(unsigned long left, unsigned long middle, unsigned long right)
{
	uint8_t largest; // 0 = front, 1 = left, 2 = right
	unsigned long current_largest = left;
	
	// Check if object is in front of front sensor
	if (current_largest < middle) {
		current_largest = middle;
	}
	// Check if object is in front of right sensor
	if (current_largest < right) {
		current_largest = right;
	}
	
	if (current_largest == middle) {
		largest = 0;
	}
	else if (current_largest == left) {
		largest = 1;
	}
	else{
		largest = 2;
	}
	return largest;
}

// Handle Button Press
void GPIOPortF_Handler(void){ // called on press of either SW1 or SW2
	int TENMS = 727240*20/91; 	//that's actually 20ms not 10
	int HUNDMS=0;
	while(TENMS) TENMS--;
	
  if((GPIO_PORTF_RIS_R&0x10) ){  // SW1 pressed
		// Turns car on or off 
		if (curr == INACTIVE)
		{
			curr = ACTIVE;
			if (mode == WALL_FOLLOWER)
			{
				for (i=0;i<10;i++) {
					ReadADCMedianFilter(&left, &middle, &right);
				}
				if(right > left){
					state = RIGHT;
				}
				else{
					state = LEFT;
				}
			}
		}
		else
		{
			curr = INACTIVE;
			LED = RED;
		}
		GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
  }
	else	// SW2 pressed
	{
		// Toggle Mode 
		if (mode == OBJECT_FOLLOWER)
		{
			mode = WALL_FOLLOWER;
			if (curr == INACTIVE) 
			{
				LED = GREEN;
				int HUNDMS = 7272400*20/91; 	//that's actually 20ms not 10
				while(HUNDMS) HUNDMS--;
				LED = RED;
			}
		}
		else
		{
			mode = OBJECT_FOLLOWER;
			LED = BLUE;
			int HUNDMS = 7272400*20/91; 	//that's actually 20ms not 10
			while(HUNDMS) HUNDMS--;
			LED = RED;
		}
		GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag4
	}
}

void SW_LED_Init(void){  
	unsigned long volatile delay;
  SYSCTL_RCGC2_R 			|= SYSCTL_RCGC2_GPIOF; 	// 1) activate clock for port F
  delay 							 = SYSCTL_RCGC2_R;
	
  GPIO_PORTF_LOCK_R  	 = GPIO_LOCK_KEY; 			// 2) unlock GPIO Port F
  GPIO_PORTF_CR_R 		|= SWLED_MASK;         	// 3) allow changes to PF0-4
  GPIO_PORTF_DIR_R 		&= ~SW_MASK;    				// 4) make PF0,4 in (built-in button)
  GPIO_PORTF_DIR_R 		|= LED_MASK;     				// 5) make PF1-3 out
  GPIO_PORTF_AFSEL_R 	&= ~SWLED_MASK;  				// 6) disable alt funct on PF0-4
  GPIO_PORTF_DEN_R 		|= SWLED_MASK;     			// 7) enable digital I/O on PF0-4
  GPIO_PORTF_PCTL_R 	&= ~0x000FFFFF; 				// 8) configure PF0-4 as GPIO
  GPIO_PORTF_AMSEL_R 	&= ~SWLED_MASK;  				// 9) disable analog functionality on PF0-4
  GPIO_PORTF_PUR_R 		|= SW_MASK;     				// 10) enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R 		&= ~SW_MASK;     				// 11) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R 		&= ~SW_MASK;    				// 12) PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R 		|= SW_MASK;    					// 13) PF4,PF0 rising edge event
  GPIO_PORTF_ICR_R 		 = SW_MASK;      				// 14) clear flags 4,0
  GPIO_PORTF_IM_R 		|= SW_MASK;      				// 15) arm interrupt on PF4,PF0
	
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|			// 16) bits:23-21 for PORTF, set priority to 5
								0x00400000; 
  NVIC_EN0_R |= 0x40000000;      							// 17) enable interrupt 30 in NVIC
}
