/*
 * File:   matrix.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:44 PM
 */

#ifndef MATRIX_H
#define MATRIX_H

#define R0      RB5  
#define R1      RB6  
#define R2      RB7 

// Switch numbers based on schematic
#define C0      RB1  
#define C1      RB2
#define C2      RB3  
#define C3      RB4  

#define SW1     1
#define SW2     2
#define SW3     3
#define SW4     4
#define SW5     5
#define SW6     6
#define SW7     7
#define SW8     8
#define SW9     9
#define SW10    10
#define SW11    11
#define SW12    12

#define ALL_RELEASED 0XFF  // Value when all switches are released

#define EDGE    0  // Trigger on key press
#define LEVEL   1  // Trigger based on key state

void init_matrix_keypad(void);  // Initialize matrix keypad
unsigned char read_matrix_keypad(unsigned char trigger);  // Read key based on trigger mode
unsigned char scan_key(void);  // Scan keypad for a pressed key

#endif
