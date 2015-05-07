/*
 * File:   wiring_digital.c
 * Author: Idris
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on January 12, 2015
 */

#include "Arduino.h"
#include "pins.h"

void pinMode(UINT8 pin, UINT8 mode)
{
  UINT16 _bit = digitalPinToBitMask[pin];
  UINT8 port = digitalPinToPort[pin];
  volatile unsigned int *reg;

  if(port == NOT_A_PORT) return;

  reg = portModeRegister[port];

  INTCON2bits.DISI = 1; // Disable all interrupts (priority < 7)
  if(mode == INPUT) *reg |= _bit;
  else *reg &= ~_bit;
  INTCON2bits.DISI = 0;
}

void turnOffPWM(UINT8 timer)
{
  T2CONbits.TON = 0; // Disable timer 2
  switch(timer)
  {
  case TIMER2_OC1:
    OC1CONbits.OCM = 0;
    break;

  case TIMER2_OC2:
    OC2CONbits.OCM = 0;
    break;

  case TIMER2_OC3:
    OC3CONbits.OCM = 0;
    break;

  case TIMER2_OC4:
    OC4CONbits.OCM = 0;
    break;
  }
}

void digitalWrite(UINT8 pin, UINT8 val)
{
  UINT8 timer = digitalPinToTimer[pin];
  UINT16 _bit = digitalPinToBitMask[pin];
  UINT8 port = digitalPinToPort[pin];
  volatile unsigned int *out;

  if(port == NOT_A_PORT) return;

  if(timer != NOT_ON_TIMER) turnOffPWM(timer);

  out = portOutputRegister[port];

  INTCON2bits.DISI = 1; // Disable all interrupts (priority < 7)
  if(val == LOW) *out &= ~_bit;
  else *out |= _bit;
  INTCON2bits.DISI = 0;
}

int digitalRead(UINT8 pin)
{
  UINT16 _bit = digitalPinToBitMask[pin];
  UINT8 port = digitalPinToPort[pin];

  if(port == NOT_A_PORT) return LOW;

  if(*portInputRegister[port] & _bit) return HIGH;
  else return LOW;
}

void digitalToggle(UINT8 pin)
{
  *portOutputRegister[digitalPinToPort[pin]] ^= digitalPinToBitMask[pin];
}
