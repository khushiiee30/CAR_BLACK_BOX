/*
 * File:   EEPROM.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:42 PM
 */

#ifndef EEPROM_H
#define EEPROM_H

#include <xc.h>  // Include PIC controller header

#define EEPROM_WRITE_ADDRESS  0xA0  // EEPROM write address
#define EEPROM_READ_ADDRESS   0xA1  // EEPROM read address

void write_external_eeprom(unsigned char address, unsigned char data);  // Write data to EEPROM
unsigned char read_external_eeprom(unsigned char address);  // Read data from EEPROM

#endif
