/* 
 * File:   adc.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:41 PM
 */

#ifndef ADC_H
#define ADC_H

// ADC channel numbers
#define CH0     0
#define CH1     1
#define CH2     2
#define CH3     3
#define CH4     4
#define CH5     5
#define CH6     6
#define CH7     7
#define CH8     8
#define CH9     9
#define CH10    10

void init_adc(void);                       // Initialize ADC
unsigned short read_adc(unsigned char channel);  // Read ADC value

#endif


