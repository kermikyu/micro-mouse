// ADC1SS2.h 
#include <stdint.h>
void ADC1_SS2_Init(void);
void ADC1_InSeq2(uint16_t *ain4, uint16_t *ain5, uint16_t *ain6);
void ReadADCMedianFilter(uint16_t *left, uint16_t *middle, uint16_t *right);