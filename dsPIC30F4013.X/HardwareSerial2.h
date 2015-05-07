/*
 * File: HardwareSerial.h
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on Jan 28, 2015
 *
 * RX2 = RF4
 * TX2 = RF5
 */

#ifndef HARDWARESERIAL_H
#define	HARDWARESERIAL_H

#include "Arduino.h"

#define SERIAL2_BUFFER_SIZE 64

typedef struct
{
  UINT8 buffer[SERIAL2_BUFFER_SIZE];
  volatile UINT16 head;
  volatile UINT16 tail;
} ring_buffer;

extern ring_buffer rx_buffer2;
extern ring_buffer tx_buffer2;

int Serial2_timedRead(void);
void Serial2_setTimeout(unsigned long timeout);
void HardwareSerial2(ring_buffer *, ring_buffer *);
void Serial2_begin(unsigned long);
void Serial2_end(void);
int Serial2_available(void);
int Serial2_peek(void);
int Serial2_read(void);
size_t Serial2_readBytes(char *buffer, size_t length);
size_t Serial2_readBytesUntil(char terminator, char *buffer, size_t length);
void Serial2_flush(void);
size_t Serial2_write(UINT8);
void Serial2_printString(const char *);
void Serial2_printNumber(long, UINT8);
void Serial2_printFloat(double, UINT8);

#endif	/* HARDWARESERIAL_H */

