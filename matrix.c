#include <xc.h>
#include "matrix.h"

unsigned char scan_key(void)
{
    // Check which switch is pressed

    R0 = 0; 
    R1 = R2 = 1;  // Select first row

    if(C0 == 0)
        return SW1;  // Return SW1

    else if(C1 == 0)
        return SW4;  // Return SW4

    else if(C2 == 0)
        return SW7;  // Return SW7

    else if(C3 == 0)
        return SW10;  // Return SW10

    R1 = 0; 
    R0 = R2 = 1;  // Select second row

    if(C0 == 0)
        return SW2;  // Return SW2

    else if(C1 == 0)
        return SW5;  // Return SW5

    else if(C2 == 0)
        return SW8;  // Return SW8

    else if(C3 == 0)
        return SW11;  // Return SW11

    R2 = 0; 
    R0 = R1 = 1;  // Select third row

    if(C0 == 0)
        return SW3;  // Return SW3

    else if(C1 == 0)
        return SW6;  // Return SW6

    else if(C2 == 0)
        return SW9;  // Return SW9

    else if(C3 == 0)
        return SW12;  // Return SW12

    return ALL_RELEASED;  // Return when no switch is pressed
}

void init_matrix_keypad(void)
{
    TRISB = (TRISB & 0X1F) | 0X1E;  // Configure keypad row and column pins

    RBPU = 0;  // Enable internal pull-ups
}

unsigned char read_matrix_keypad(unsigned char trigger)
{
    static int once = 0;  // Track key release and press

    if(trigger == LEVEL)
    {
        return scan_key();  // Return the currently pressed key
    }
    else if(trigger == EDGE)
    {
        unsigned char key = scan_key();  // Read the pressed key

        if((key != ALL_RELEASED) && (once == 1))
        {
            once = 0;  // Reset flag after detecting key press
            return key;  // Return the key pressed
        }
        else if(key == ALL_RELEASED)
        {
            once = 1;  // Allow the next key press to be detected
        }

        return ALL_RELEASED;  // No new key press detected
    }
}

