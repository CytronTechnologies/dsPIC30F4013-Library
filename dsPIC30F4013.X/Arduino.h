/* 
 * File:   Arduino.h
 * Author: Idris
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on January 9, 2015
 */

#ifndef ARDUINO_H
#define	ARDUINO_H

/***** Include files *****/
#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/***** Definitions *****/
#define _XTAL_FREQ 117920000UL // 7.37MHz x 16 = 117920000

#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define true  1
#define false 0

#define SERIAL  0
#define DISPLAY 1

#define LSBFIRST 0
#define MSBFIRST 1

#define FALLING 0
#define RISING  1

#define DEFAULT  0
#define EXTERNAL 1

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

#define NOT_A_PIN 0
#define NOT_A_PORT 0
#define NOT_ON_TIMER 0
#define TIMER2_OC1 1
#define TIMER2_OC2 2
#define TIMER2_OC3 3
#define TIMER2_OC4 4

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PF 6

#define _BV(P) (UINT16)(1 << P)

// dsPIC30F4013 pins mapping
#define A11 17

#define B0  2
#define B1  3
#define B2  4
#define B3  5
#define B4  6
#define B5  7
#define B6  8
#define B7  9
#define B8  10
#define B9  38
#define B10 37
#define B11 36
#define B12 35

#define C13 15
#define C14 16
#define C15 14

#define D0  34
#define D1  33
#define D2  22
#define D3  19
#define D8  23
#define D9  18

#define F0  30
#define F1  29
#define F2  26
#define F3  25
#define F4  28
#define F5  27
#define F6  24

#define LED1 D2
#define LED2 D3
#define SW1  A11
#define SW2  D8

#define A0  0
#define A1  1
#define A2  2
#define A3  3
#define A4  4
#define A5  5
#define A6  6
#define A7  7
#define A8  8
#define A9  9
#define A10 10
#define A11 11
#define A12 12

typedef enum _BIT {CLEAR = 0, SET} BIT;
typedef unsigned char UINT8;
typedef unsigned int UINT16;

/***** Function prototypes *****/
void init(void);
void setup(void);
void loop(void);

void pinMode(UINT8, UINT8);
void digitalWrite(UINT8, UINT8);
int digitalRead(UINT8);
void digitalToggle(UINT8);
void analogReference(UINT8);
UINT16 analogRead(UINT8); // return 0 - 4095
void analogWrite(UINT8, UINT16); // val 0 - 100

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(UINT16 us);
unsigned long pulseIn(UINT8 pin, UINT8 state, unsigned long timeout);

UINT8 shiftIn(UINT8 dataPin, UINT8 clockPin, UINT8 bitOrder);
void shiftOut(UINT8 dataPin, UINT8 clockPin, UINT8 bitOrder, UINT8 val);

void attachInterrupt(UINT8, void (*)(void), int mode);
void detachInterrupt(UINT8);

void tone(UINT8, unsigned int, unsigned long);
void disableTimer(void);
void noTone(UINT8);

#endif	/* ARDUINO_H */
