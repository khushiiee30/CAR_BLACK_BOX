#include <xc.h>

#include "i2c.h"
#include "rtc.h"

void init_rtc(void)
{
    unsigned char seconds;

    seconds = read_rtc(0x00);  // Read current seconds

    seconds = seconds & 0x7F;  // Clear the clock halt bit

    write_rtc(0x00, seconds);  // Start the RTC
}

void write_rtc(unsigned char address, unsigned char data)
{
    i2c_start();  // Start I2C communication

    i2c_write(RTC_WRITE_ADDRESS);  // Send RTC write address

    i2c_write(address);  // Select RTC register

    i2c_write(data);  // Write data to RTC

    i2c_stop();  // Stop I2C communication
}

unsigned char read_rtc(unsigned char address)
{
    unsigned char data;

    i2c_start();  // Start I2C communication

    i2c_write(RTC_WRITE_ADDRESS);  // Send RTC write address

    i2c_write(address);  // Select RTC register

    i2c_rep_start();  // Generate repeated start condition

    i2c_write(RTC_READ_ADDRESS);  // Send RTC read address

    data = i2c_read();  // Read data from RTC

    i2c_stop();  // Stop I2C communication

    return data;  // Return the read data
}

void get_time(unsigned char *time)
{
    time[0] = read_rtc(0x02);  // Read hours
    time[1] = read_rtc(0x01);  // Read minutes
    time[2] = read_rtc(0x00);  // Read seconds
}

