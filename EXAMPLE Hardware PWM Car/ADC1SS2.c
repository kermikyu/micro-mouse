// ADC1SS2 reference ADCSWTrigger 
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS2.h"

#define ADC1_PSSI_SS2 0x0004    // start sample sequencer 2
#define ADC1_ISC_SS2  0x0004   // acknowledge sample sequencer 3 interrupt
#define ADC1_RIS_SS2  0x08
#define ADC1_CLK      0x02
#define PORTE_MASK    0x07

void ADC1_SS2_Init(void) {
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R1;      // 1) Activate ADC1
        while((SYSCTL_PRADC_R & SYSCTL_RCGCADC_R1) == 0){};      // Wait for ADC1
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;    // 1) Activate clock for Port E
        while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R4) == 0){};      // Wait for Port E
        GPIO_PORTE_DIR_R &= ~PORTE_MASK;            // 2) make PE0-2 input
        GPIO_PORTE_AFSEL_R |= PORTE_MASK;             // 3) enable alternate function on PE0-2
        GPIO_PORTE_DEN_R &= ~PORTE_MASK;          // 4) disable digital I/O on PE0-2
        GPIO_PORTE_AMSEL_R |= PORTE_MASK;             // 5) enable analog function on PE0-2
            
        ADC1_PC_R &= ~ADC_PC_SR_M;            // 6) clear max sample rate field
    ADC1_PC_R |= ADC_PC_SR_125K;        // 7) configure for 125K samples/sec
            
    //ADC1_SSPRI_R = 0x3210;              // 8) Sequencer 3 is lowest priority
            
        ADC1_ACTSS_R &= ~ADC_ACTSS_ASEN2;  // 9) disable sample sequencer 2
        ADC1_SSMUX2_R = 0x0231;                         // 11) channels Ain1, Ain3, Ain2 (PE2, PE0, PE1)
        ADC1_SSCTL2_R = 0x0600;
        ADC1_ACTSS_R |= ADC_ACTSS_ASEN2;
}



// ADC1_InSeq2: Read all three sensors into uint16_t variables
void ADC1_InSeq2(uint16_t *ain2, uint16_t *ain3, uint16_t *ain1) {
    ADC1_PSSI_R |= 0x04;            // Initiate SS2
    while((ADC1_RIS_R & 0x04) == 0){}; // Wait for conversion
    *ain2 = ADC1_SSFIFO2_R & 0xFFF;   // First sample (AIN4)
    *ain3 = ADC1_SSFIFO2_R & 0xFFF;   // Second sample (AIN5)
    *ain1 = ADC1_SSFIFO2_R & 0xFFF;   // Third sample (AIN6)
    ADC1_ISC_R = 0x04;              // Clear interrupt
}