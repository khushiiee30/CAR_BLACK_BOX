/*
 * File:   uart.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:45 PM
 */

#ifndef UART_H
#define UART_H

void init_UART(void);  // Initialize UART

void uart_char_transmit(unsigned char ch);  // Transmit a character
void uart_str_transmit(const unsigned char *str);  // Transmit a string

unsigned char uart_receive(void);  // Receive a character

#endif
