#include <xc.h>

#define _XTAL_FREQ 20000000UL

#include "black_box.h"
#include "clcd.h"
#include "matrix.h"
#include "adc.h"
#include "rtc.h"
#include "event.h"

static unsigned char gear = 0;  // Store current gear

static const unsigned char gear_names[7][3] = {"GN", "G1", "G2", "G3", "G4", "G5", "GR"};  // Gear names

static unsigned char initial_state = 1;  // Track initial dashboard state

static unsigned char bcd_to_decimal(unsigned char value)
{
    return ((value >> 4) * 10) + (value & 0x0F);  // Convert BCD to decimal
}

static void display_two_digits(unsigned char value, unsigned char address)
{
    clcd_putch((value / 10) + '0', address);  // Display tens digit
    clcd_putch((value % 10) + '0', address + 1);  // Display units digit
}

static void display_speed(unsigned int speed, unsigned char address)
{
    clcd_putch((speed / 10) + '0', address);  // Display tens digit of speed
    clcd_putch((speed % 10) + '0', address + 1);  // Display units digit of speed
}

void view_dashboard(void)
{
    unsigned char time[3];
    unsigned char key;
    unsigned short adc_value;
    unsigned int speed;

    get_time(time);  // Read current time from RTC

    adc_value = read_adc(CH4);  // Read ADC value from channel 4

    speed = (unsigned int)(((unsigned long)adc_value * 99UL) / 1023UL);  // Convert ADC value to speed

    clcd_print((const unsigned char *)"TIME", LINE1(2));  // Display time heading
    clcd_print((const unsigned char *)"GEAR", LINE1(9));  // Display gear heading
    clcd_print((const unsigned char *)"SP", LINE1(14));  // Display speed heading

    display_two_digits(bcd_to_decimal(time[0]), LINE2(0));  // Display hours

    clcd_putch(':', LINE2(2));  // Display separator

    display_two_digits(bcd_to_decimal(time[1]), LINE2(3));  // Display minutes

    clcd_putch(':', LINE2(5));  // Display separator

    display_two_digits(bcd_to_decimal(time[2]), LINE2(6));  // Display seconds

    if (initial_state == 1)
    {
        clcd_print((const unsigned char *)"ON", LINE2(10));  // Display ON initially
    }
    else
    {
        clcd_print(gear_names[gear], LINE2(10));  // Display current gear
    }

    display_speed(speed, LINE2(14));  // Display current speed

    key = read_matrix_keypad(EDGE);  // Read keypad input

    switch (key)
    {
        case SW1:

            if (initial_state == 1)
            {
                initial_state = 0;  // Exit initial state
                gear = 0;  // Set gear to neutral
            }
            else if (gear < 6)
            {
                gear++;  // Increase gear
            }

            break;

        case SW2:

            if (initial_state == 0)
            {
                if (gear > 0)
                {
                    gear--;  // Decrease gear
                }
            }

            break;

        case SW3:

            event_store(gear);  // Store current gear event

            clcd_print((const unsigned char *)"_C", LINE2(10));  // Display collision indicator

            __delay_ms(2000);  // Wait for 2 seconds

            break;

        case SW11:

            CLEAR_DISP_SCREEN;  // Clear LCD
            state = e_main_menu;  // Switch to main menu

            break;

        default:
            break;
    }

    __delay_ms(50);  // Small delay for keypad handling
}

