/*
 * File:   i2c.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:44 PM
 */

#ifndef I2C_H
#define I2C_H

#include <xc.h>  // Include PIC controller header

#define _XTAL_FREQ 20000000UL  // Set clock frequency to 20 MHz

void init_i2c(void);  // Initialize I2C
void i2c_idle(void);  // Wait until I2C becomes idle
void i2c_ack(void);  // Send acknowledgement
void i2c_start(void);  // Generate start condition
void i2c_stop(void);  // Generate stop condition
void i2c_rep_start(void);  // Generate repeated start condition
void i2c_write(unsigned char data);  // Send data through I2C
void i2c_rx_mode(void);  // Enable I2C receive mode
void i2c_no_ack(void);  // Send not-acknowledge signal
unsigned char i2c_read(void);  // Read data through I2C

#endif
