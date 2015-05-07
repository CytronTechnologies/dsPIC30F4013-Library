/*
 * File: Tone.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on Jan 28, 2015
 */

#include "Arduino.h"

volatile long timer3_toggle_count;
volatile UINT8 *timer3_pin_port;
volatile UINT8 timer3_pin_mask;
volatile UINT16 ocr = 0;
UINT8 _pin;

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt()
{
  IFS0bits.T3IF = 0; // Clear Timer 3 Interrupt Flag
  TMR3 = ocr;
  if(timer3_toggle_count != 0)
  {
    // Toggle the pin
    digitalToggle(_pin);

    if(timer3_toggle_count > 0)
      timer3_toggle_count--;
  }
  else
  {
    disableTimer();
    digitalWrite(_pin, LOW);  // keep pin low after stop
  }
}

void toneBegin(void)
{
  IFS0bits.T3IF = 0; // Clear Timer 3 Interrupt Flag
  IPC1bits.T3IP = 1; // Interrupt priority
  PR3 = 0xFFFF; // Timer 3 Period Register
  T3CONbits.TCKPS = 1; // Prescaler 1:8
  T3CONbits.TON = 1; // Enable Timer 3
}

void tone(UINT8 pin, unsigned int frequency, unsigned long duration)
{
  long toggle_count = 0;

  _pin = pin;
  toneBegin();
  pinMode(_pin, OUTPUT);

  ocr = _XTAL_FREQ / 4 / 8 / frequency;
  ocr = 65535 - ocr;

  // Calculate the toggle count
  if (duration > 0)
  {
    toggle_count = 2 * frequency * duration / 1000;
  }
  else
  {
    toggle_count = -1;
  }

  TMR3 = ocr;
  timer3_toggle_count = toggle_count;
  IEC0bits.T3IE = 1; // Enable Timer3 interrupt
}

void disableTimer(void)
{
  IEC0bits.T3IE = 0; // Disable Timer 3 interrupt
  T3CONbits.TON = 0; // Disable Timer 3
}

void noTone(UINT8 pin)
{
  disableTimer();
  digitalWrite(_pin, LOW);
}
