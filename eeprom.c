#include <xc.h>

#include "i2c.h"
#include "eeprom.h"

#define _XTAL_FREQ 20000000UL

void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();

    /* EEPROM device address - write mode */
    i2c_write(EEPROM_WRITE_ADDRESS);

    /* EEPROM memory address */
    i2c_write(address);

    /* Data to be written */
    i2c_write(data);

    i2c_stop();

    /* Wait for the internal write cycle */
    __delay_ms(5);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();

    /* EEPROM device address - write mode */
    i2c_write(EEPROM_WRITE_ADDRESS);

    /* Select memory address */
    i2c_write(address);

    /* Repeated start for read operation */
    i2c_rep_start();

    /* EEPROM device address - read mode */
    i2c_write(EEPROM_READ_ADDRESS);

    /* Read data from EEPROM */
    data = i2c_read();

    i2c_stop();

    return data;
}

