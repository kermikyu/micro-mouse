// FollowMe.c
// Runs on TM4C123
// This the starter test program for the object follower that implements basic features: 
// follow an object forward and backward, stop when the object is out of range.
// By Min He
// March 12, 2024

// This is an example program to show how to use hardware PWM on TM4C123.
//#include "tm4c123gh6pm.h"
//#include <stdint.h>
//#include "SysTick.h"
//#include "PWM.h"
//#include "GPIO.h"
//#include "ADC0SS3.h"
//#include "PLL.h"

// move the following constant definitions to ADC0SS3.h


//void follow_me(void);
//uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3);
//uint16_t ReadADCMedianFilter(void);
//void Delay(void);
//volatile uint16_t ADCvalue;

//int main(void){	
//	PLL_Init();
//	PortB_Init();		
//  
//	ADC0_InitSWTriggerSeq3_Ch1(); 
//	PWM_PB54_Init();	
//																			
//	while(1){
//			PWM_PB54_Duty(STOP, STOP);
//			follow_me();
//  }
//}

// Implement a simple algorithm to follow an object to move forward/backward/stop.
//void follow_me(void) {
//	uint8_t i;
//	
//	// Calibrate the sensor
//	for (i=0;i<10;i++) {
//    ADCvalue = ReadADCMedianFilter();  // read one value
//	} 
//	
//  // wait until an obstacle is in the right distant range.
// 	do {
//			ADCvalue = ReadADCMedianFilter();  // read one value
//	} while ((ADCvalue>TOO_CLOSE) || (ADCvalue<TOO_FAR));
//		
//  while ((ADCvalue<TOO_CLOSE) && (ADCvalue>TOO_FAR)) {								
//		ADCvalue = ReadADCMedianFilter();
//		
//		if (ADCvalue>FOLLOW_DIST) { // negative logic: too close, move back
//			WHEEL_DIR = BACKWARD;
//			PWM_PB54_Duty(SPEED_50,SPEED_50); // enable both wheels

//		}
//		else if (ADCvalue<FOLLOW_DIST){ // negative logic: too far, move forward
//			WHEEL_DIR = FORWARD;
//			PWM_PB54_Duty(SPEED_50,SPEED_50);
//		}
//		else { // right distance, stop
//			PWM_PB54_Duty(STOP, STOP);
//		}
//  }	
//}

//uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3){
//uint16_t result;
//	
//  if(u1>u2)
//    if(u2>u3)   result=u2;     // u1>u2,u2>u3       u1>u2>u3
//      else
//        if(u1>u3) result=u3;   // u1>u2,u3>u2,u1>u3 u1>u3>u2
//        else      result=u1;   // u1>u2,u3>u2,u3>u1 u3>u1>u2
//  else
//    if(u3>u2)   result=u2;     // u2>u1,u3>u2       u3>u2>u1
//      else
//        if(u1>u3) result=u1;   // u2>u1,u2>u3,u1>u3 u2>u1>u3
//        else      result=u3;   // u2>u1,u2>u3,u3>u1 u2>u3>u1
//  return(result);
//}

//uint16_t ReadADCMedianFilter(void){
//  static uint16_t oldest=0, middle=0;	
//  uint16_t newest;
//	uint16_t NewValue;
//	
//  newest = ADC0_InSeq3();  // read one value
//  NewValue = median(newest, middle, oldest);
//  oldest = middle; 
//  middle = newest; 
//	return NewValue;
//}