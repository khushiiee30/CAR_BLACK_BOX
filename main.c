/*
 * File:   main.c
 * Author: KHUSHI KUMARI
 * Reg No: 25048_075
 *
 * ================================================================
 *                       CAR BLACK BOX
 * ================================================================
 *
 * Project Description:
 *
 * This project is an embedded automotive event recording system
 * developed using the PIC18F4580 microcontroller. The main purpose
 * of the project is to monitor vehicle parameters and record
 * important information during collision events.
 *
 * The system collects vehicle-related information such as speed,
 * gear position and time. When a collision event is triggered,
 * the corresponding details are recorded in external EEPROM memory
 * for future reference.
 *
 * MICROCONTROLLER:
 *
 * The PIC18F4580 microcontroller acts as the main processing unit
 * of the system. It reads inputs, processes vehicle parameters,
 * communicates with the connected peripherals and manages the
 * storage and display of event information.
 *
 * ADC:
 *
 * The Analog-to-Digital Converter (ADC) is used to read analog
 * input values. The obtained digital value is processed and mapped
 * to a speed representation, which is displayed and stored during
 * event recording.
 *
 * RTC:
 *
 * The Real-Time Clock (RTC) provides the current time information,
 * including hours, minutes and seconds. This time information is
 * associated with the recorded collision events.
 *
 * EXTERNAL EEPROM:
 *
 * External EEPROM is used to store collision event records.
 * Each event record contains the event time, speed, gear position
 * and event type. The stored information is retained even when
 * the system is powered off.
 *
 * The system maintains an event count to keep track of the
 * recorded events. The stored records can be accessed later
 * through the system menu.
 *
 * CLCD AND MATRIX KEYPAD:
 *
 * The Character LCD (CLCD) is used to display vehicle parameters,
 * menu options and stored event information.
 *
 * The matrix keypad provides user input and allows navigation
 * through the available menu options and system functions.
 *
 * EVENT RECORDING:
 *
 * When a collision event is triggered, the system collects the
 * available time, speed and gear information. These details,
 * along with the event type, are stored in external EEPROM memory.
 *
 * The user can later retrieve and view the stored event records
 * through the display interface.
 *
 * Overall, the project demonstrates the practical application
 * of embedded systems in automotive monitoring and event
 * recording. It combines Embedded C programming, ADC input
 * processing, RTC interfacing, EEPROM data storage, CLCD
 * interfacing and keypad-based user interaction.
 *
 * ================================================================
 */


#include <xc.h>
#define _XTAL_FREQ 20000000UL

#include "black_box.h"

// Global state variable
State_t state;

void init_config(void)
{
    init_clcd();  // Initialize CLCD
    init_matrix_keypad();  // Initialize matrix keypad
    init_i2c();  // Initialize I2C communication
    init_rtc();  // Initialize RTC
    init_adc();  // Initialize ADC
    init_UART();  // Initialize UART communication

    clcd_print((const unsigned char *)"CAR BLACK BOX", LINE1(1));  // Display project name
    clcd_print((const unsigned char *)"STARTING.....", LINE2(1));  // Display startup message

    __delay_ms(2000);  // Wait for 2 seconds

    CLEAR_DISP_SCREEN;  // Clear CLCD

    state = e_dashboard;  // Set initial state to dashboard
}

void main(void)
{
    init_config();  // Initialize the system

    while (1)  // Run the application continuously
    {
        switch (state)
        {
            case e_dashboard:
                view_dashboard();  // Display dashboard
                break;

            case e_main_menu:
                display_main_menu();  // Display main menu
                break;

            case e_view_log:
                view_log();  // Display stored event logs
                break;

            case e_download_log:
                download_log();  // Download stored event logs
                break;

            case e_clear_log:
                clear_log();  // Clear stored event logs
                break;

            case e_set_time:
                set_time();  // Set RTC time
                break;

            default:
                state = e_dashboard;  // Return to dashboard for an invalid state
                break;
        }
    }
}
