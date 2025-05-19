// SysTick.c

#include <stdint.h>
#include "SysTick.h"
#include "tm4c123gh6pm.h"

#define ONE_SECOND 3125000

void SysTick_Wait(uint8_t delay){
	for (int i = 0; i < delay; i++){
		NVIC_ST_CTRL_R = 0;
		NVIC_ST_RELOAD_R = ONE_SECOND-1; // Counts to Wait
		NVIC_ST_CURRENT_R = 0;
		NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
    while((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0); // wait for count flag
    NVIC_ST_CTRL_R = 0;
	}
}