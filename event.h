/*
 * File:   event.h
 * Author: KHUSHI
 *
 * Created on 4 October, 2026, 2:43 PM
 */

#ifndef EVENT_H
#define EVENT_H

// EEPROM event configuration
#define EVENT_SIZE       6   // Size of each event
#define MAX_EVENTS       15  // Maximum number of events

#define EVENT_COUNT_ADDR  0x00  // Address to store event count
#define EVENT_START_ADDR  0x01  // Starting address for events

// Event types
#define COLLISION_EVENT   1  // Collision event identifier

void event_store(unsigned char gear);  // Store vehicle event

#endif

