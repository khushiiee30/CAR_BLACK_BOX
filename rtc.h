/*
 * File:   rtc.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:44 PM
 */

#ifndef RTC_H
#define RTC_H

#include <xc.h>  // Include PIC controller header

#define RTC_WRITE_ADDRESS  0xD0  // RTC write address
#define RTC_READ_ADDRESS   0xD1  // RTC read address

void init_rtc(void);  // Initialize RTC

void write_rtc(unsigned char address, unsigned char data);  // Write data to RTC
unsigned char read_rtc(unsigned char address);  // Read data from RTC

void get_time(unsigned char *time);  // Get current time from RTC

#endif
