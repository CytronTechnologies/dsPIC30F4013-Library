/* 
 * File:   pins.h
 * Author: Idris
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on January 9, 2015
 */

#ifndef PINS_H
#define	PINS_H

#include "Arduino.h"

volatile unsigned int *portModeRegister[] = {
  NOT_A_PORT,
  &TRISA,
  &TRISB,
  &TRISC,
  &TRISD,
  NOT_A_PORT,
  &TRISF
};

volatile unsigned int *portInputRegister[] = {
  NOT_A_PORT,
  &PORTA,
  &PORTB,
  &PORTC,
  &PORTD,
  NOT_A_PORT,
  &PORTF
};

volatile unsigned int *portOutputRegister[] = {
  NOT_A_PORT,
  &LATA,
  &LATB,
  &LATC,
  &LATD,
  NOT_A_PORT,
  &LATF
};

const UINT8 digitalPinToPort[] = {
  NOT_A_PIN, // 0

  NOT_A_PIN, // Pin 1 - MCLR
  PB, // Pin 2 - RB0
  PB, // Pin 3 - RB1
  PB, // Pin 4 - RB2
  PB, // Pin 5 - RB3
  PB, // Pin 6 - RB4
  PB, // Pin 7 - RB5
  PB, // Pin 8 - RB6
  PB, // Pin 9 - RB7
  PB, // Pin 10 - RB8

  NOT_A_PIN, // Pin 11 - VDD
  NOT_A_PIN, // Pin 12 - VSS
  NOT_A_PIN, // Pin 13 - OSC1
  PC, // Pin 14 - RC15/OSC2
  PC, // Pin 15 - RC13
  PC, // Pin 16 - RC14
  PA, // Pin 17 - RA11
  PD, // Pin 18 - RD9
  PD, // Pin 19 - RD3
  NOT_A_PIN, // Pin 20 - VSS

  NOT_A_PIN, // Pin 21 - VDD
  PD, // Pin 22 - RD2
  PD, // Pin 23 - RD8
  PF, // Pin 24 - RF6
  PF, // Pin 25 - RF3
  PF, // Pin 26 - RF2
  PF, // Pin 27 - RF5
  PF, // Pin 28 - RF4
  PF, // Pin 29 - RF1
  PF, // Pin 30 - RF0

  NOT_A_PIN, // Pin 31 - VSS
  NOT_A_PIN, // Pin 32 - VDD
  PD, // Pin 33 - RD1
  PD, // Pin 34 - RD0
  PB, // Pin 35 - RB12
  PB, // Pin 36 - RB11
  PB, // Pin 37 - RB10
  PB, // Pin 38 - RB9
  NOT_A_PIN, // Pin 39 - AVSS
  NOT_A_PIN, // Pin 40 - AVDD
};

const UINT16 digitalPinToBitMask[] = {
  NOT_A_PIN, // 0
  
  NOT_A_PIN, // Pin 1 - MCLR
  _BV(0), // Pin 2 - RB0
  _BV(1), // Pin 3 - RB1
  _BV(2), // Pin 4 - RB2
  _BV(3), // Pin 5 - RB3
  _BV(4), // Pin 6 - RB4
  _BV(5), // Pin 7 - RB5
  _BV(6), // Pin 8 - RB6
  _BV(7), // Pin 9 - RB7
  _BV(8), // Pin 10 - RB8

  NOT_A_PIN, // Pin 11 - VDD
  NOT_A_PIN, // Pin 12 - VSS
  NOT_A_PIN, // Pin 13 - OSC1
  _BV(15), // Pin 14 - RC15/OSC2
  _BV(13), // Pin 15 - RC13
  _BV(14), // Pin 16 - RC14
  _BV(11), // Pin 17 - RA11
  _BV(9), // Pin 18 - RD9
  _BV(3), // Pin 19 - RD3
  NOT_A_PIN, // Pin 20 - VSS

  NOT_A_PIN, // Pin 21 - VDD
  _BV(2), // Pin 22 - RD2
  _BV(8), // Pin 23 - RD8
  _BV(6), // Pin 24 - RF6
  _BV(3), // Pin 25 - RF3
  _BV(2), // Pin 26 - RF2
  _BV(5), // Pin 27 - RF5
  _BV(4), // Pin 28 - RF4
  _BV(1), // Pin 29 - RF1
  _BV(0), // Pin 30 - RF0

  NOT_A_PIN, // Pin 31 - VSS
  NOT_A_PIN, // Pin 32 - VDD
  _BV(1), // Pin 33 - RD1
  _BV(0), // Pin 34 - RD0
  _BV(12), // Pin 35 - RB12
  _BV(11), // Pin 36 - RB11
  _BV(10), // Pin 37 - RB10
  _BV(9), // Pin 38 - RB9
  NOT_A_PIN, // Pin 39 - AVSS
  NOT_A_PIN // Pin 40 - AVDD
};

const UINT8 digitalPinToTimer[] = {
  NOT_ON_TIMER, // 0

  NOT_ON_TIMER, // Pin 1 - MCLR
  NOT_ON_TIMER, // Pin 2 - RB0
  NOT_ON_TIMER, // Pin 3 - RB1
  NOT_ON_TIMER, // Pin 4 - RB2
  NOT_ON_TIMER, // Pin 5 - RB3
  NOT_ON_TIMER, // Pin 6 - RB4
  NOT_ON_TIMER, // Pin 7 - RB5
  NOT_ON_TIMER, // Pin 8 - RB6
  NOT_ON_TIMER, // Pin 9 - RB7
  NOT_ON_TIMER, // Pin 10 - RB8

  NOT_ON_TIMER, // Pin 11 - VDD
  NOT_ON_TIMER, // Pin 12 - VSS
  NOT_ON_TIMER, // Pin 13 - OSC1
  NOT_ON_TIMER, // Pin 14 - RC15/OSC2
  NOT_ON_TIMER, // Pin 15 - RC13
  NOT_ON_TIMER, // Pin 16 - RC14
  NOT_ON_TIMER, // Pin 17 - RA11
  NOT_ON_TIMER, // Pin 18 - RD9
  TIMER2_OC4, // Pin 19 - RD3
  NOT_ON_TIMER, // Pin 20 - VSS

  NOT_ON_TIMER, // Pin 21 - VDD
  TIMER2_OC3, // Pin 22 - RD2
  NOT_ON_TIMER, // Pin 23 - RD8
  NOT_ON_TIMER, // Pin 24 - RF6
  NOT_ON_TIMER, // Pin 25 - RF3
  NOT_ON_TIMER, // Pin 26 - RF2
  NOT_ON_TIMER, // Pin 27 - RF5
  NOT_ON_TIMER, // Pin 28 - RF4
  NOT_ON_TIMER, // Pin 29 - RF1
  NOT_ON_TIMER, // Pin 30 - RF0

  NOT_ON_TIMER, // Pin 31 - VSS
  NOT_ON_TIMER, // Pin 32 - VDD
  TIMER2_OC2, // Pin 33 - RD1
  TIMER2_OC1, // Pin 34 - RD0
  NOT_ON_TIMER, // Pin 35 - RB12
  NOT_ON_TIMER, // Pin 36 - RB11
  NOT_ON_TIMER, // Pin 37 - RB10
  NOT_ON_TIMER, // Pin 38 - RB9
  NOT_ON_TIMER, // Pin 39 - AVSS
  NOT_ON_TIMER, // Pin 40 - AVDD
};

#endif	/* PINS_H */
