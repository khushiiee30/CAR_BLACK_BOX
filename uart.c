#include <xc.h>
#include "uart.h"

#define _XTAL_FREQ 20000000UL

void init_UART(void)
{
    TRISC6 = 0;  // Set TX pin as output
    TRISC7 = 1;  // Set RX pin as input

    TX9 = 0;     // Select 8-bit transmission
    TXEN = 1;    // Enable transmitter
    SYNC = 0;    // Select asynchronous mode
    SENDB = 0;   // Enable normal transmission
    BRGH = 1;    // Select high-speed baud rate

    SPEN = 1;    // Enable serial port
    RX9 = 0;     // Select 8-bit reception
    CREN = 1;    // Enable receiver

    BRG16 = 0;   // Select 8-bit baud rate generator
    WUE = 0;     // Disable wake-up on UART activity
    ABDEN = 0;   // Disable automatic baud rate detection

    SPBRG = 129;  // Set baud rate to 9600 for 20 MHz clock
}

void uart_char_transmit(unsigned char ch)
{
    while (!TXIF)  // Wait until transmitter is ready
    {
    }

    TXREG = ch;  // Transmit the character
}

void uart_str_transmit(const unsigned char *str)
{
    while (*str != '\0')  // Continue until the string ends
    {
        uart_char_transmit(*str);  // Transmit the current character
        str++;  // Move to the next character
    }
}

unsigned char uart_receive(void)
{
    while (!RCIF)  // Wait until data is received
    {
    }

    return RCREG;  // Return the received character
}
