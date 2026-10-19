#include <xc.h>
#include "clcd.h"

#define _XTAL_FREQ 20000000UL

void clcd_write(unsigned char byte, unsigned char control_bit)
{
    CLCD_RS = control_bit;  // Select command or data mode
    CLCD_RW = LO;  // Set LCD to write mode

    CLCD_PORT = byte;  // Send data to LCD

    CLCD_EN = HI;  // Enable LCD
    __delay_us(1);  // Wait for LCD to read data
    CLCD_EN = LO;  // Disable LCD

    if (control_bit == INSTRUCTION_COMMAND)
    {
        __delay_ms(2);  // Wait for command execution
    }
    else
    {
        __delay_us(50);  // Wait for data write
    }
}

void init_clcd(void)
{
    TRISD = 0x00;  // Set LCD data pins RD0-RD7 as output

    TRISC = TRISC & 0xF8;  // Set LCD control pins RC0, RC1, RC2 as output

    CLCD_RW = LO;  // Set LCD to write mode
    CLCD_EN = LO;  // Disable LCD
    CLCD_RS = LO;  // Select command mode

    __delay_ms(30);  // Wait for LCD power-up

    clcd_write(0x30, INSTRUCTION_COMMAND);  // Initialize LCD
    __delay_ms(5);  // Wait for initialization

    clcd_write(0x30, INSTRUCTION_COMMAND);  // Repeat initialization
    __delay_us(150);  // Wait for LCD response

    clcd_write(0x30, INSTRUCTION_COMMAND);  // Complete initialization sequence
    __delay_us(150);  // Wait for LCD response

    clcd_write(0x38, INSTRUCTION_COMMAND);  // Set 8-bit mode and 2-line display

    clcd_write(0x08, INSTRUCTION_COMMAND);  // Turn off display

    clcd_write(0x01, INSTRUCTION_COMMAND);  // Clear LCD display
    __delay_ms(2);  // Wait for display to clear

    clcd_write(0x06, INSTRUCTION_COMMAND);  // Move cursor to the right after each character

    clcd_write(0x0C, INSTRUCTION_COMMAND);  // Turn on display and hide cursor
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND);  // Set LCD cursor position

    while (*data != '\0')  // Continue until the string ends
    {
        clcd_write(*data++, DATA_COMMAND);  // Display the next character
    }
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND);  // Set LCD cursor position
    clcd_write(data, DATA_COMMAND);  // Display a single character
}

