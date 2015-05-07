/*
 * File:   wiring_analog.c
 * Author: Idris
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on January 13, 2015
 *
 * OC1 = RD0
 * OC2 = RD1
 * OC3 = RD2
 * OC4 = RD3
 */

#include "Arduino.h"

UINT16 analogPinToRegisterBitMask[] = {
  _BV(0), // RB0 - A0
  _BV(1), // RB1 - A1
  _BV(2), // RB2 - A2
  _BV(3), // RB3 - A3
  _BV(4), // RB4 - A4
  _BV(5), // RB5 - A5
  _BV(6), // RB6 - A6
  _BV(7), // RB7 - A7
  _BV(8), // RB8 - A8
  _BV(9), // RB9 - A9
  _BV(10), // RB10 - A10
  _BV(11), // RB11 - A11
  _BV(12), // RB12 - A12
};

const UINT8 digitalPinToPWM[] = {
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

UINT8 analog_reference = DEFAULT;
void analogReference(UINT8 mode)
{
  analog_reference = mode;
}

UINT16 analogRead(UINT8 pin)
{
  ADPCFG = ~analogPinToRegisterBitMask[pin]; // Set pin to analog
  ADCHS = pin; // Select channel

  if(analog_reference == DEFAULT)
    ADCON2bits.VCFG = 0b000; // +Vref = AVdd, -Vref = AVss
  else if(analog_reference == EXTERNAL)
    ADCON2bits.VCFG = 0b010; // +Vref = +Vref, -Vref = AVss
  
  ADCON1bits.SSRC = 0b111; // Auto conversion trigger
  ADCON1bits.ASAM = 1; // Auto sampling
  ADCON3bits.SAMC = 1; // Sampling = 1 Tad
  ADCON3bits.ADCS = 19; // Tad = 339ns (min 334ns)
  ADCON1bits.ADON = 1; // Enable ADC module

  ADCON1bits.SAMP = 1; // Start sampling and auto convert
  while(!ADCON1bits.DONE); // Wait for the conversion complete

  UINT16 val = ADCBUF0;

  ADCON1bits.ADON = 0; // Disable ADC module
  ADPCFG = 0xFFFF; // Set all analog pin to digital
  
  return val;
}

char OC1EnableFlag = 0;
char OC2EnableFlag = 0;
char OC3EnableFlag = 0;
char OC4EnableFlag = 0;
void analogWrite(UINT8 pin, UINT16 val)
{
  pinMode(pin, OUTPUT);

	if(val >= 100) val = 100;

  pin = digitalPinToPWM[pin];
  if(pin != NOT_ON_TIMER)
  {
    switch(pin)
    {
    case TIMER2_OC1:
      OC1RS = val;
      if(!OC1EnableFlag)
      {
        OC1EnableFlag = 1;
        OC1CONbits.OCM = 0b110; // Enable simple PWM mode
      }
      break;

    case TIMER2_OC2:
      OC2RS = val;
      if(!OC2EnableFlag)
      {
        OC2EnableFlag = 1;
        OC2CONbits.OCM = 0b110; // Enable simple PWM mode
      }
      break;

    case TIMER2_OC3:
      OC3RS = val;
      if(!OC3EnableFlag)
      {
        OC3EnableFlag = 1;
        OC3CONbits.OCM = 0b110; // Enable simple PWM mode
      }
      break;

    case TIMER2_OC4:
      OC4RS = val;
      if(!OC4EnableFlag)
      {
        OC4EnableFlag = 1;
        OC4CONbits.OCM = 0b110; // Enable simple PWM mode
      }
      break;
    }
    T2CONbits.TON = 1; // Enable timer 2
  }
}
