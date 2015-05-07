/*
 * File:   wiring.c
 * Author: Idris
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on January 12, 2015
 */

#include "Arduino.h"

#define clockCyclesPerMicrosecond()  (_XTAL_FREQ/4/1000000L)
#define clockCyclesToMicroseconds(a) ((a)/clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a)*clockCyclesPerMicrosecond())

#define MICROSECONDS_PER_TIMER1_OVERFLOW (clockCyclesToMicroseconds(0xFFFF))
#define MILLIS_INC (MICROSECONDS_PER_TIMER1_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER1_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

unsigned long timer1_overflow_count = 0;
unsigned long timer1_millis = 0;
static unsigned char timer1_fract;

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt()
{
  IFS0bits.T1IF = 0;

  unsigned long m = timer1_millis;
  unsigned char f = timer1_fract;

  m += MILLIS_INC;
  f += FRACT_INC;
  if(f >= FRACT_MAX)
  {
    f -= FRACT_MAX;
    m += 1;
  }

  timer1_fract = f;
  timer1_millis = m;
  timer1_overflow_count++;
}

unsigned long millis(void)
{
  unsigned long m;

  INTCON2bits.DISI = 1; // Disable all interrupts (priority < 7)
	m = timer1_millis;
	INTCON2bits.DISI = 0;

	return m;
}

unsigned long micros(void)
{
  unsigned long m;
  UINT8 t;

	INTCON2bits.DISI = 1; // Disable all interrupts (priority < 7)
	m = timer1_overflow_count;
  t = TMR1;

  if(IFS0bits.T1IF && (t < 255)) m++;

	INTCON2bits.DISI = 0;
  IFS0bits.T1IF = 0;

	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void delay(unsigned long ms)
{
  __delay32((_XTAL_FREQ/4000ULL) * ms);
}

void delayMicroseconds(UINT16 us)
{
  __delay32((_XTAL_FREQ/4000000ULL) * us);
}

void init()
{
  ADPCFG = 0xFFFF; // Set all analog pin to digital

  PORTA = 0;
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
  PORTF = 0;

  // At initial state, all IO are input (dsPIC30F4013)
  TRISAbits.TRISA11 = 1;
  TRISB = 0b1111111111111;
  TRISCbits.TRISC13 = 1;
  TRISCbits.TRISC14 = 1;
  TRISCbits.TRISC15 = 1;
  TRISD = 0b1111000011;
  TRISF = 0b1111111;

  // Timer 1 configuration for millis and micros
  PR1 = 0xFFFF;
  T1CONbits.TCKPS = 0; // Timer 1 prescaler 1
  IPC0bits.T1IP = 1; // Interrupt priority
  IFS0bits.T1IF = 0; // Clear Timer 1 interrupt flag
  T1CONbits.TON = 1; // Enable Timer 1
  IEC0bits.T1IE = 1; // Enable Timer 1 interrupt

  // Timer 2 configuration for PWM, freq = 9.353kHz
  PR2 = 100;
  T2CONbits.TCKPS = 0b01; // 1:8 prescale
}
