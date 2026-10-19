/*
 * File:   black_box.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:42 PM
 */

#ifndef BLACK_BOX_H
#define BLACK_BOX_H

#include <xc.h>  // Include PIC controller header

#define _XTAL_FREQ 20000000UL  // Set clock frequency to 20 MHz

#include "clcd.h"
#include "matrix.h"
#include "i2c.h"
#include "rtc.h"
#include "adc.h"
#include "uart.h"

// Application states
typedef enum
{
    e_dashboard,
    e_main_menu,
    e_view_log,
    e_set_time,
    e_download_log,
    e_clear_log
} State_t;

// Declare global application state
extern State_t state;

// Initialize peripherals
void init_config(void);

// Application functions
void view_dashboard(void);
void display_main_menu(void);
void view_log(void);
void set_time(void);
void download_log(void);
void clear_log(void);

// Store vehicle events
void event_store(unsigned char gear);

// Handle password operations
void password(void);
void event_reader(void);
void change_password(void);

#endif

