#include <xc.h>

#include "black_box.h"
#include "event.h"
#include "eeprom.h"
#include "rtc.h"
#include "adc.h"

#define _XTAL_FREQ 20000000UL

void event_store(unsigned char gear)
{
    unsigned char event_count;
    unsigned char address;
    unsigned char time[3];
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
    unsigned char speed;

    unsigned short adc_value;

    // Read the current event count
    event_count = read_external_eeprom(EVENT_COUNT_ADDR);

    // Stop if the event memory is full
    if (event_count >= MAX_EVENTS)
    {
        return;
    }

    // Read current RTC time
    get_time(time);

    // Convert BCD time into decimal values
    hours = (unsigned char)(((time[0] >> 4) * 10) + (time[0] & 0x0F));
    minutes = (unsigned char)(((time[1] >> 4) * 10) + (time[1] & 0x0F));
    seconds = (unsigned char)(((time[2] >> 4) * 10) + (time[2] & 0x0F));

    // Read speed from ADC channel 4
    adc_value = read_adc(CH4);

    // Convert ADC value into a speed from 0 to 99
    speed = (unsigned char)(((unsigned long)adc_value * 99UL) / 1023UL);

    // Calculate EEPROM address for the new event
    address = EVENT_START_ADDR + (event_count * EVENT_SIZE);

    // Store event information
    write_external_eeprom(address++, hours);
    write_external_eeprom(address++, minutes);
    write_external_eeprom(address++, seconds);
    write_external_eeprom(address++, speed);
    write_external_eeprom(address++, gear);
    write_external_eeprom(address++, COLLISION_EVENT);

    // Update the event count after storing the record
    write_external_eeprom(EVENT_COUNT_ADDR, event_count + 1);
}