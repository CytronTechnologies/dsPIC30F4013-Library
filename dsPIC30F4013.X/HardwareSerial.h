/*
 * File: HardwareSerial.h
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on Jan 28, 2015
 *
 * RX1 = RF2
 * TX1 = RF3
 */

#ifndef HARDWARESERIAL_H
#define	HARDWARESERIAL_H

#include "Arduino.h"

#define SERIAL_BUFFER_SIZE 64

typedef struct
{
  UINT8 buffer[SERIAL_BUFFER_SIZE];
  volatile UINT16 head;
  volatile UINT16 tail;
} ring_buffer;

extern ring_buffer rx_buffer;
extern ring_buffer tx_buffer;

int Serial_timedRead(void);
void Serial_setTimeout(unsigned long timeout);
void HardwareSerial(ring_buffer *, ring_buffer *);
void Serial_begin(unsigned long);
void Serial_end(void);
int Serial_available(void);
int Serial_peek(void);
int Serial_read(void);
size_t Serial_readBytes(char *buffer, size_t length);
size_t Serial_readBytesUntil(char terminator, char *buffer, size_t length);
void Serial_flush(void);
size_t Serial_write(UINT8);
void Serial_printString(const char *);
void Serial_printNumber(long, UINT8);
void Serial_printFloat(double, UINT8);

#endif	/* HARDWARESERIAL_H */

