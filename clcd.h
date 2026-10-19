#ifndef CLCD_H
#define CLCD_H

// CLCD pin connections
#define CLCD_PORT           PORTD
#define CLCD_EN             RC2
#define CLCD_RS             RC1
#define CLCD_RW             RC0
#define CLCD_BUSY           RD7
#define PORT_DIR            TRISD7

// Logic levels
#define HI                  1
#define LO                  0

// Port direction settings
#define INPUT               0xFF
#define OUTPUT              0x00

// CLCD command types
#define DATA_COMMAND        1
#define INSTRUCTION_COMMAND 0

// CLCD line addresses
#define LINE1(x)            (0x80 + (x))
#define LINE2(x)            (0xC0 + (x))

// CLCD initialization and control commands
#define TWO_LINE_5x8_MATRIX_8_BIT  clcd_write(0x38, INSTRUCTION_COMMAND)
#define CLEAR_DISP_SCREEN          clcd_write(0x01, INSTRUCTION_COMMAND)
#define CURSOR_HOME                clcd_write(0x02, INSTRUCTION_COMMAND)
#define DISP_ON_AND_CURSOR_OFF     clcd_write(0x0C, INSTRUCTION_COMMAND)
#define EIGHT_BIT_MODE             0x33

// CLCD function declarations
void init_clcd(void);  // Initialize CLCD
void clcd_print(const unsigned char *data, unsigned char addr);  // Display a string
void clcd_putch(const unsigned char data, unsigned char addr);  // Display a character
void clcd_write(unsigned char bit_values, unsigned char control_bit);

#endif
