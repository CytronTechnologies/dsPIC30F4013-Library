/*
 * File: HardwareSerial.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on Jan 28, 2015
 */

#include "HardwareSerial.h"

ring_buffer rx_buffer = {{0}, 0, 0};
ring_buffer tx_buffer = {{0}, 0, 0};

ring_buffer *_rx_buffer;
ring_buffer *_tx_buffer;

BIT transmitting;
unsigned long _timeout;
unsigned long _startMillis;

inline void store_char(UINT8 c, ring_buffer *buffer)
{
  int i = (UINT16)(buffer->head + 1) % SERIAL_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if(i != buffer->tail)
  {
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
  }
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt()
{
  IFS0bits.U1RXIF = 0; // Clear the UART1 Receive Interrupt flag
  UINT8 c = U1RXREG;
  store_char(c, &rx_buffer);
  U1STAbits.OERR = 0; // Clear the Receive Overflow Flag
}

void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt()
{
  IFS0bits.U1TXIF = 0; // Clear the UART1 Transmit Interrupt flag
  if(tx_buffer.head == tx_buffer.tail)
  {
    // Buffer empty, so disable interrupts
    IEC0bits.U1TXIE = 0; // Disable UART1 Transmit Interrupt
  }
  else
  {
    // There is more data in the output buffer. Send the next byte
    UINT8 c = tx_buffer.buffer[tx_buffer.tail];
    tx_buffer.tail = (tx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;

    U1TXREG = c;
  }
}

int Serial_timedRead(void)
{
  int c;
  _startMillis = millis();
  do {
    c = Serial_read();
    if (c >= 0) return c;
  } while(millis() - _startMillis < _timeout);
  return -1;     // -1 indicates timeout
}

void Serial_setTimeout(unsigned long timeout)  // sets the maximum number of milliseconds to wait
{
  _timeout = timeout;
}

void HardwareSerial(ring_buffer *rx_buffer, ring_buffer *tx_buffer)
{
  _rx_buffer = rx_buffer;
  _tx_buffer = tx_buffer;
}

void Serial_begin(unsigned long baudrate)
{
  HardwareSerial(&rx_buffer, &tx_buffer);

  U1BRG = ((_XTAL_FREQ/4)/(16*baudrate)) - 0.5; // ((Fosc/4)/(16*Baud rate))-1
  
  IPC2bits.U1RXIP = 4; // UART1 Receive Interrupt Priority = 4/7
  U1STAbits.URXISEL = 0; // Interrupt flag bit is set for every data received
  IEC0bits.U1RXIE = 1; // Enable UART1 Receive Interrupt
  IFS0bits.U1RXIF = 0; // Clear the UART1 Receive Interrupt flag
  U1STAbits.OERR = 0; // Clear the Receive Overflow Flag

  IPC2bits.U1TXIP = 4; // UART1 Transmit Interrupt Priority = 4/7
  U1STAbits.UTXISEL = 0; // Interrupt flag bit is set for every data sent
  IEC0bits.U1TXIE = 0; // Disable UART1 Transmit Interrupt
  IFS0bits.U1TXIF = 0; // Clear the UART1 Transmit Interrupt flag
  U1MODEbits.ALTIO = 0; // Select UART1

  U1MODEbits.UARTEN = 1; // Enable UART1
  U1STAbits.UTXEN = 1; // Enable UART1 Transmit
}

void Serial_end(void)
{
  // Wait for transmission of outgoing data
  while(_tx_buffer->head != _tx_buffer->tail);

  IEC0bits.U1RXIE = 0; // Disable UART1 Receive Interrupt
  IEC0bits.U1TXIE = 0; // Disable UART1 Transmit Interrupt
  U1STAbits.UTXEN = 0; // Disable UART1 Transmit
  U1MODEbits.UARTEN = 0; // Disable UART1

  // Clear any received data
  _rx_buffer->head = _rx_buffer->tail;
}

int Serial_available(void)
{
  return (UINT16)(SERIAL_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % SERIAL_BUFFER_SIZE;
}

int Serial_peek(void)
{
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    return _rx_buffer->buffer[_rx_buffer->tail];
  }
}

int Serial_read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    UINT8 c = _rx_buffer->buffer[_rx_buffer->tail];
    _rx_buffer->tail = (UINT16)(_rx_buffer->tail + 1) % SERIAL_BUFFER_SIZE;
    return c;
  }
}

size_t Serial_readBytes(char *buffer, size_t length)
{
  size_t count = 0;
  while(count < length)
  {
    int c = Serial_timedRead();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
  }
  return count;
}

size_t Serial_readBytesUntil(char terminator, char *buffer, size_t length)
{
  if(length < 1) return 0;
  size_t index = 0;
  while(index < length)
  {
    int c = Serial_timedRead();
    if (c < 0 || c == terminator) break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}

void Serial_flush(void)
{
  // TSR is kept full while the buffer is not empty, so TXC triggers when EMPTY && SENT
  while(transmitting && !U1STAbits.TRMT);
  transmitting = false;
}

size_t Serial_write(UINT8 c)
{
  int i = (_tx_buffer->head + 1) % SERIAL_BUFFER_SIZE;

  while(i == _tx_buffer->tail);

  _tx_buffer->buffer[_tx_buffer->head] = c;
  _tx_buffer->head = i;

  IFS0bits.U1TXIF = 1; // Set UART1 Transmit Interrupt flag
  IEC0bits.U1TXIE = 1; // Enable UART1 Transmit Interrupt
  transmitting = true;

  return 1;
}

void Serial_printString(const char *s)
{
    while(*s) Serial_write(*s++);
}

void Serial_printNumber(long n, UINT8 base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *s = &buf[sizeof(buf) - 1];

  *s = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    unsigned long m = n;
    n /= base;
    char c = m - base * n;
    *--s = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  while(*s) Serial_write(*s++);

//  UINT8 i, j, di[10];
//  for(i = 0; i < 10; i++) di[i] = 0;
//  i = 0;
//
//  do {
//    di[i++] = number % base;
//    number = number / base;
//  } while(number);
//
//  for(j = digit; j > 0; j--)
//  {
//    if(di[j - 1] < 10) Serial_write(di[j - 1] + '0');
//    else Serial_write(di[j - 1] - 10 + 'A');
//  }
}

void Serial_printFloat(double number, UINT8 point)
{
  if(number > 4294967040.0) Serial_printString("ovf");  // constant determined empirically
  else if(number <-4294967040.0) Serial_printString ("ovf");  // constant determined empirically
  else
  {
    // Handle negative numbers
    if(number < 0.0)
    {
      Serial_write('-');
      number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    UINT8 i;
    for(i = 0; i < point; ++i)
      rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    Serial_printNumber(int_part, DEC);

    // Print the decimal point, but only if there are digits beyond
    if(point > 0) {
      Serial_write('.');
    }

    // Extract digits from the remainder one at a time
    while(point-- > 0)
    {
      remainder *= 10.0;
      int toPrint = (int)remainder;
      Serial_printNumber(toPrint, DEC);
      remainder -= toPrint;
    }
  }
}

