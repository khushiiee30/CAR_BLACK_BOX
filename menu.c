#include "black_box.h"
#include "eeprom.h"
#include "event.h"
#include "uart.h"

/*
 * SW1  - Next menu item
 * SW2  - Previous menu item
 * SW11 - Select menu item
 * SW12 - Return to dashboard
 */

void display_main_menu(void)
{
    unsigned char key;
    static unsigned char menu_index = 0;

    const unsigned char *menu_items[] =
    {
        (const unsigned char *)"View Log",
        (const unsigned char *)"Download Log",
        (const unsigned char *)"Clear Log",
        (const unsigned char *)"Set Time"
    };

    CLEAR_DISP_SCREEN;

    while (1)
    {
        
        clcd_print((const unsigned char *)"MENU", LINE1(0));

        clcd_print((const unsigned char *)"->", LINE2(0));
        clcd_print(menu_items[menu_index], LINE2(3));

        key = read_matrix_keypad(EDGE);

        switch (key)
        {
            case SW1:
                
                if (menu_index < 3)
                {
                    menu_index++;
                }
                else
                {
                    menu_index = 0;
                }

                CLEAR_DISP_SCREEN;
                break;

            case SW2:
                
                if (menu_index > 0)
                {
                    menu_index--;
                }
                else
                {
                    menu_index = 3;
                }

                CLEAR_DISP_SCREEN;
                break;

            case SW11:
                
                CLEAR_DISP_SCREEN;

                if (menu_index == 0)
                {
                    state = e_view_log;
                }
                else if (menu_index == 1)
                {
                    state = e_download_log;
                }
                else if (menu_index == 2)
                {
                    state = e_clear_log;
                }
                else
                {
                    state = e_set_time;
                }

                return;

            case SW12:
                
                CLEAR_DISP_SCREEN;
                state = e_dashboard;
                return;

            default:
                break;
        }

        __delay_ms(150);
    }
}

void view_log(void)
{
    unsigned char key;
    unsigned char event_count;
    unsigned char event_index = 0;
    unsigned char event_number;
    //unsigned char event_type;

    unsigned char address;
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
    unsigned char speed;
    unsigned char gear_value;

    const unsigned char gear_names[7][3] ={
        "GN",
        "G1",
        "G2",
        "G3",
        "G4",
        "G5",
        "GR"
    };

    /*
     * Read number of stored events
     */
    event_count = read_external_eeprom(EVENT_COUNT_ADDR);

    if (event_count == 0)
    {
        CLEAR_DISP_SCREEN;

        clcd_print((const unsigned char *)"UNAVAILABLE EVENTS", LINE1(3));

        clcd_print((const unsigned char *)"EVENTS", LINE2(3));

        __delay_ms(1500);

        CLEAR_DISP_SCREEN;
        state = e_main_menu;
        return;
    }

    /*
     * Safety limit
     */
    if (event_count > MAX_EVENTS)
    {
        event_count = MAX_EVENTS;
    }

    while (1)
    {
        /*
         * Calculate record address
         */
        address = EVENT_START_ADDR + (event_index * EVENT_SIZE);

        /*
         * Read event record
         */
        hours      = read_external_eeprom(address++);
        minutes    = read_external_eeprom(address++);
        seconds    = read_external_eeprom(address++);
        speed      = read_external_eeprom(address++);
        gear_value = read_external_eeprom(address++);
        //event_type = read_external_eeprom(address++);

        CLEAR_DISP_SCREEN;

        /*
         * LINE 1
         * N  TIME GR SP
         */
        clcd_print((const unsigned char *)"N", LINE1(0));

        clcd_print((const unsigned char *)"TIME", LINE1(3));

        clcd_print((const unsigned char *)"GR",LINE1(11));

        clcd_print((const unsigned char *)"SP", LINE1(14));

        /*
         * Event number
         */

        event_number = event_index + 1;

        if (event_number < 10)
        {
            clcd_putch((unsigned char)(event_number + '0'), LINE2(0));
        }
        else
        {
            clcd_putch((unsigned char)((event_number / 10) + '0'), LINE2(0));

            clcd_putch((unsigned char)((event_number % 10) + '0'),LINE2(1));
        }

        /*
         * TIME
         */
        
        /* HOURS */
        clcd_putch((unsigned char)((hours / 10) + '0'),LINE2(2));

        clcd_putch((unsigned char)((hours % 10) + '0'), LINE2(3));

        clcd_putch(':', LINE2(4));

        /* MINUTES */
        clcd_putch((unsigned char)((minutes / 10) + '0'), LINE2(5));

        clcd_putch((unsigned char)((minutes % 10) + '0'),LINE2(6));

        clcd_putch(':', LINE2(7));

        /* SECONDS */
        clcd_putch((unsigned char)((seconds / 10) + '0'),LINE2(8));

        clcd_putch((unsigned char)((seconds % 10) + '0'),LINE2(9));

        /*
         * GEAR
         */
        if (gear_value <= 6)
        {
            clcd_print(
                gear_names[gear_value], LINE2(11)
            );
        }
        else
        {
            clcd_print(
                (const unsigned char *)"--",LINE2(11)
            );
        }

        /*
         * SPEED
         */
        clcd_putch((unsigned char)((speed / 10) + '0'),LINE2(14));

        clcd_putch((unsigned char)((speed % 10) + '0'),LINE2(15));

        /*
         * Key handling
         */
        key = read_matrix_keypad(EDGE);

        if (key == SW1)
        {
            /*
             * Next event
             */
            if (event_index < (event_count - 1))
            {
                event_index++;
            }
        }

        else if (key == SW2)
        {
            /*
             * Previous event
             */
            if (event_index > 0)
            {
                event_index--;
            }
        }

        else if (key == SW12)
        {
            CLEAR_DISP_SCREEN;
            state = e_main_menu;
            return;
        }

        __delay_ms(150);
    }
}

void download_log(void)
{
    unsigned char event_count;
    unsigned char event_index;
    unsigned char address;

    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
    unsigned char speed;
    unsigned char gear;
    unsigned char event_type;
    unsigned char event_number;

    const unsigned char gear_names[7][3] = {"GN","G1","G2","G3","G4","G5","GR"};

    /*
     * Read number of stored events
     */
    event_count = read_external_eeprom(EVENT_COUNT_ADDR);

    /*
     * LCD message
     */
    CLEAR_DISP_SCREEN;

    clcd_print((const unsigned char *)"Downloading....",LINE1(0));

    /*
     * UART heading
     */
    uart_str_transmit((const unsigned char *)"\r\nCAR BLACK BOX DATA\r\n\r\n");

    if (event_count == 0)
    {
        uart_str_transmit((const unsigned char *)"No Data  present\r\n\r\n");
    }

    else
    {
        /*
         * Table heading
         */
        uart_str_transmit((const unsigned char *)"N  Time     GR SPD\r\n");

        /*
         * Send all stored events
         */
        for (event_index = 0; event_index < event_count && event_index < MAX_EVENTS; event_index++)
        {
            address = EVENT_START_ADDR + (event_index * EVENT_SIZE);

            /*
             * Read event data
             */
            hours = read_external_eeprom(address++);
            minutes = read_external_eeprom(address++);
            seconds = read_external_eeprom(address++);
            speed = read_external_eeprom(address++);
            gear = read_external_eeprom(address++);
            event_type = read_external_eeprom(address++);

            event_number = event_index;

            if (event_number < 10)
            {
                uart_char_transmit((unsigned char)(event_number + '0'));

                uart_str_transmit((const unsigned char *)"  ");
            }
            else
            {
                uart_char_transmit((unsigned char)((event_number / 10) + '0'));

                uart_char_transmit((unsigned char)((event_number % 10) + '0'));

                uart_str_transmit((const unsigned char *)" ");
            }

            /*
             * TIME
             * Event time is stored as decimal.
             */
            uart_char_transmit((unsigned char)((hours / 10) + '0'));

            uart_char_transmit((unsigned char)((hours % 10) + '0'));

            uart_char_transmit(':');

            uart_char_transmit((unsigned char)((minutes / 10) + '0'));

            uart_char_transmit((unsigned char)((minutes % 10) + '0'));

            uart_char_transmit(':');

            uart_char_transmit((unsigned char)((seconds / 10) + '0'));

            uart_char_transmit((unsigned char)((seconds % 10) + '0'));

            uart_str_transmit((const unsigned char *)" ");

            /*
             * GEAR
             */
            if (gear <= 6)
            {
                uart_str_transmit(gear_names[gear]);
            }
            else
            {
                uart_str_transmit((const unsigned char *)"--");
            }
                        /*
             * SPEED
             */
            uart_char_transmit((unsigned char)((speed / 10) + '0'));

            uart_char_transmit((unsigned char)((speed % 10) + '0'));

            uart_str_transmit((const unsigned char *)"\r\n");
        }

        /*
         * Blank line after the last record
         */
        uart_str_transmit((const unsigned char *)"\r\n");
    }

    /*
     * Final message
     */
    uart_str_transmit((const unsigned char *)"DOWNLOADED\r\n");

    /*
     * LCD confirmation
     */
    CLEAR_DISP_SCREEN;

    clcd_print((const unsigned char *)"DOWNLOADED",LINE1(0));

    clcd_print((const unsigned char *)"CHECK UART",LINE2(0));

    __delay_ms(2000);

    CLEAR_DISP_SCREEN;

    state = e_main_menu;
}

void clear_log(void)
{
    unsigned char key;
    unsigned char address;

    CLEAR_DISP_SCREEN;

    clcd_print((const unsigned char *)"CLEAR DATA?", LINE1(0));

    clcd_print((const unsigned char *)"SW11:YES SW12:NO",LINE2(0));

    while (1)
    {
        key = read_matrix_keypad(EDGE);

        /* SW11 = YES */
        if (key == SW11)
        {
            /*
             * Clear all 15 event records
             * 15 x 6 = 90 bytes
             */
            for (address = EVENT_START_ADDR; address < (EVENT_START_ADDR + (MAX_EVENTS * EVENT_SIZE)); address++)
            {
                write_external_eeprom(address, 0x00);
            }

            /*
             * Reset event count
             */
            write_external_eeprom(EVENT_COUNT_ADDR, 0x00);

            CLEAR_DISP_SCREEN;

            clcd_print((const unsigned char *)"DATA CLEARED", LINE1(0));

            clcd_print((const unsigned char *)"SUCCESSFULLY", LINE2(0));

            __delay_ms(1500);

            CLEAR_DISP_SCREEN;
            state = e_main_menu;
            return;
        }

        /* SW12 = NO */
        else if (key == SW12)
        {
            CLEAR_DISP_SCREEN;
            state = e_main_menu;
            return;
        }

        __delay_ms(150);
    }
}

void set_time(void)
{
    unsigned char key;
    unsigned char field = 0;
    unsigned char blink_state = 1;

    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;

    /*
     * Read current RTC time in BCD format
     */
    hours = read_rtc(0x02);
    minutes = read_rtc(0x01);
    seconds = read_rtc(0x00);

    /*
     * Convert BCD to decimal
     */
    hours = ((hours >> 4) * 10) + (hours & 0x0F);
    minutes = ((minutes >> 4) * 10) + (minutes & 0x0F);
    seconds = ((seconds >> 4) * 10) + (seconds & 0x0F);

    CLEAR_DISP_SCREEN;

    while (1)
    {
        /*
         * LINE 1: Centered "SET TIME"
         * 8 characters -> starting position 4
         */
        clcd_print((const unsigned char *)"SET TIME",LINE1(4));

        /*
         * LINE 2: Centered HH:MM:SS
         * 8 characters -> starting position 4
         */
        clcd_putch('0' + (hours / 10),LINE2(4));

        clcd_putch('0' + (hours % 10),LINE2(5));

        clcd_putch(':',LINE2(6));

        clcd_putch('0' + (minutes / 10),LINE2(7));

        clcd_putch('0' + (minutes % 10),LINE2(8));

        clcd_putch(':',LINE2(9));

        clcd_putch('0' + (seconds / 10),LINE2(10));

        clcd_putch('0' + (seconds % 10),LINE2(11));

        /*
         * Blink the selected field
         */
        if (blink_state == 0)
        {
            if (field == 0)
            {
                clcd_putch(' ', LINE2(4));
                clcd_putch(' ', LINE2(5));
            }
            else if (field == 1)
            {
                clcd_putch(' ', LINE2(7));
                clcd_putch(' ', LINE2(8));
            }
            else
            {
                clcd_putch(' ', LINE2(10));
                clcd_putch(' ', LINE2(11));
            }
        }

        /*
         * Read keypad
         */
        key = read_matrix_keypad(EDGE);

        /*
         * SW1 = Increment selected field
         */
        if (key == SW1)
        {
            if (field == 0)
            {
                hours++;

                if (hours >= 24)
                    hours = 0;
            }
            else if (field == 1)
            {
                minutes++;

                if (minutes >= 60)
                    minutes = 0;
            }
            else
            {
                seconds++;

                if (seconds >= 60)
                    seconds = 0;
            }
        }

        /*
         * SW2 = Move to next field
         *
         * Hours -> Minutes -> Seconds
         */
        else if (key == SW2)
        {
            if (field < 2)
            {
                field++;
            }
        }

        /*
         * SW11 = Save and exit
         */
        else if (key == SW11)
        {
            /*
             * Convert decimal to BCD and save to RTC
             */
            write_rtc(0x02,(unsigned char)(((hours / 10) << 4) |(hours % 10)));

            write_rtc(0x01,(unsigned char)(((minutes / 10) << 4) |(minutes % 10)));

            write_rtc(0x00,(unsigned char)(((seconds / 10) << 4) |(seconds % 10)));

            /*
             * Confirmation message
             */
            CLEAR_DISP_SCREEN;

            clcd_print((const unsigned char *)"TIME SAVED", LINE1(3));

            __delay_ms(1500);

            CLEAR_DISP_SCREEN;
            state = e_main_menu;
            return;
        }

        /*
         * SW12 = Exit without saving
         */
        else if (key == SW12)
        {
            CLEAR_DISP_SCREEN;
            state = e_main_menu;
            return;
        }

        /*
         * Blink timing
         */
        blink_state = !blink_state;

        __delay_ms(250);
    }
}
