///////////////////////////////////////////////////////////////////////////////
//// Course Number: CECS 347
//// Assignment: Example project for Hardware PWM controlled Car
//// Description: 
///////////////////////////////////////////////////////////////////////////////

////////////////////////1. Pre-processor Directives Section////////////////////
//#include "tm4c123gh6pm.h"
//#include "PWM.h"
//#include "GPIO.h"
//#include "SysTick.h"
//#include "PLL.h"
///////////////////////////////////////////////////////////////////////////////

////////////////////////2. Declarations Section////////////////////////////////
//volatile int paused = 1;  // 1 = Paused (robot OFF by default), 0 = Running

//////////// Local Global Variables //////////

//////////// Function Prototypes //////////
//void DisableInterrupts(void);
//void EnableInterrupts(void);
//void WaitForInterrupt(void);
//void Delay(void);

////////////////////////3. Subroutines Section/////////////////////////////////
//// MAIN: This main is meant for the command configuration of the hc05.
//int main(void){ 
//	DisableInterrupts();
//	PLL_Init();
//	Timer0A_Init();
//	PortB_Init();
//  LED_Init();
//  PWM_PB54_Init();
//  PWM_PB54_Duty(SPEED_50, SPEED_50);
//	Switch_Init();
//	EnableInterrupts();
//	//WHEEL_DIR = FORWARD;
//	PWM_PB54_Duty(STOP, STOP);
//	
//	done = 1;
//	
//  while(1){
//		//WaitForInterrupt();
//		if (!done){
//			if(!done){
//			// moving forward
//			LED = Green;
//			WHEEL_DIR = FORWARD;
//			PWM_PB54_Duty(SPEED_50,SPEED_50);
//			SysTick_Wait(16);
//		
//			// stop
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP);  // stop both wheels
//			SysTick_Wait(4);
//			}
//			if(!done){
//			// Enable blue LED,moving backward
//			LED = Blue;
//			WHEEL_DIR = BACKWARD;
//			PWM_PB54_Duty(SPEED_50,SPEED_50); // enable both wheels
//			SysTick_Wait(16);
//			
//			// stop
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP); // stop both wheels
//			SysTick_Wait(4);
//			}
//		
//			if(!done){
//			// Forward left turn
//			LED = Yellow;
//			WHEEL_DIR=FORWARD;
//			PWM_PB54_Duty(SPEED_50, 0);
//			SysTick_Wait(16);
//			
//			// stop
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP); // stop both wheels
//			SysTick_Wait(4);
//			}
//			
//			if(!done){
//			// Forward right turn
//			LED = Purple;
//			WHEEL_DIR=FORWARD;
//			PWM_PB54_Duty(0, SPEED_50);
//			SysTick_Wait(16);
//			
//			// stop
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP); // stop both wheels
//			SysTick_Wait(4);
//			}
//			
//			if(!done){
//			// Backward left turn
//			LED = Yellow;
//			WHEEL_DIR = BACKWARD;
//			PWM_PB54_Duty(SPEED_50, 0);
//			SysTick_Wait(16);
//				
//			// stop
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP); // stop both wheels
//			SysTick_Wait(4);
//			}
//		
//			if(!done){
//			// Backward right turn
//			LED = Purple;
//			WHEEL_DIR=BACKWARD;
//			PWM_PB54_Duty(0, SPEED_50);
//			SysTick_Wait(16);
//		
//			// stop
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP); // stop both wheels
//			SysTick_Wait(4);
//			}
//		
//			if(!done){
//			// Left pivot turn
//			LED = Cran;
//			WHEEL_DIR=LEFTPIVOT;
//			PWM_PB54_Duty(SPEED_50, 0);
//			SysTick_Wait(16);
//		
//			// stop
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP); // stop both wheels
//			SysTick_Wait(4);
//			}

//			if(!done){
//			// right pivot turn
//			LED = White;
//			WHEEL_DIR=RIGHTPIVOT;
//			PWM_PB54_Duty(0,SPEED_50); // Enable both wheels
//			SysTick_Wait(16);
//			
//			LED = Dark;
//			PWM_PB54_Duty(STOP, STOP); // stop both wheels
//			}
//		}
//	}	
//}

//void GPIOPortF_Handler(void) {
//		SysTick_Wait(1);
//    if (GPIO_PORTF_RIS_R & 0x10) {  // Check if SW1 (PF4) triggered the interrupt
//        GPIO_PORTF_ICR_R = 0x10;    // Clear the interrupt flag for PF4
//    }
//		if (done == 1) {
//			done = 0;
//			PWM_PB54_Duty(SPEED_50,SPEED_50);
//		} else if (done == 0) {
//			done = 1;
//			PWM_PB54_Duty(STOP, STOP);
//		}
//}