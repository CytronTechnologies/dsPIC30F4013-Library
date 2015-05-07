/*
 * File: HardwareSerial.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on Jan 28, 2015
 */

#include "HardwareSerial2.h"

ring_buffer rx_buffer2 = {{0}, 0, 0};
ring_buffer tx_buffer2 = {{0}, 0, 0};

ring_buffer *_rx_buffer2;
ring_buffer *_tx_buffer2;

BIT transmitting2;
unsigned long _timeout2;
unsigned long _startMillis2;

inline void store_char2(UINT8 c, ring_buffer *buffer)
{
  int i = (UINT16)(buffer->head + 1) % SERIAL2_BUFFER_SIZE;

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

void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt()
{
  IFS1bits.U2RXIF = 0; // Clear the UART2 Receive Interrupt flag
  UINT8 c = U2RXREG;
  store_char2(c, &rx_buffer2);
  U2STAbits.OERR = 0; // Clear the Receive Overflow Flag
}

void __attribute__((__interrupt__, __auto_psv__)) _U2TXInterrupt()
{
  IFS1bits.U2TXIF = 0; // Clear the UART2 Transmit Interrupt flag
  if(tx_buffer2.head == tx_buffer2.tail)
  {
    // Buffer empty, so disable interrupts
    IEC1bits.U2TXIE = 0; // Disable UART2 Transmit Interrupt
  }
  else
  {
    // There is more data in the output buffer. Send the next byte
    UINT8 c = tx_buffer2.buffer[tx_buffer2.tail];
    tx_buffer2.tail = (tx_buffer2.tail + 1) % SERIAL2_BUFFER_SIZE;

    U2TXREG = c;
  }
}

int Serial2_timedRead(void)
{
  int c;
  _startMillis2 = millis();
  do {
    c = Serial2_read();
    if (c >= 0) return c;
  } while(millis() - _startMillis2 < _timeout2);
  return -1;     // -1 indicates timeout
}

void Serial2_setTimeout(unsigned long timeout)  // sets the maximum number of milliseconds to wait
{
  _timeout2 = timeout;
}

void HardwareSerial2(ring_buffer *rx_buffer2, ring_buffer *tx_buffer2)
{
  _rx_buffer2 = rx_buffer2;
  _tx_buffer2 = tx_buffer2;
}

void Serial2_begin(unsigned long baudrate)
{
  HardwareSerial2(&rx_buffer2, &tx_buffer2);

  U2BRG = ((_XTAL_FREQ/4)/(16*baudrate)) - 0.5; // ((Fosc/4)/(16*Baud rate))-1
  
  IPC6bits.U2RXIP = 4; // UART2 Receive Interrupt Priority = 4/7
  U2STAbits.URXISEL = 0; // Interrupt flag bit is set for every data received
  IEC1bits.U2RXIE = 1; // Enable UART2 Receive Interrupt
  IFS1bits.U2RXIF = 0; // Clear the UART2 Receive Interrupt flag
  U2STAbits.OERR = 0; // Clear the Receive Overflow Flag

  IPC6bits.U2TXIP = 4; // UART2 Transmit Interrupt Priority = 4/7
  U2STAbits.UTXISEL = 0; // Interrupt flag bit is set for every data sent
  IEC1bits.U2TXIE = 0; // Disable UART2 Transmit Interrupt
  IFS1bits.U2TXIF = 0; // Clear the UART2 Transmit Interrupt flag

  U2MODEbits.UARTEN = 1; // Enable UART2
  U2STAbits.UTXEN = 1; // Enable UART2 Transmit
}

void Serial2_end(void)
{
  // Wait for transmission of outgoing data
  while(_tx_buffer2->head != _tx_buffer2->tail);

  IEC1bits.U2RXIE = 0; // Disable UART2 Receive Interrupt
  IEC1bits.U2TXIE = 0; // Disable UART2 Transmit Interrupt
  U2STAbits.UTXEN = 0; // Disable UART2 Transmit
  U2MODEbits.UARTEN = 0; // Disable UART2

  // Clear any received data
  _rx_buffer2->head = _rx_buffer2->tail;
}

int Serial2_available(void)
{
  return (UINT16)(SERIAL2_BUFFER_SIZE + _rx_buffer2->head - _rx_buffer2->tail) % SERIAL2_BUFFER_SIZE;
}

int Serial2_peek(void)
{
  if(_rx_buffer2->head == _rx_buffer2->tail)
  {
    return -1;
  }
  else
  {
    return _rx_buffer2->buffer[_rx_buffer2->tail];
  }
}

int Serial2_read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if(_rx_buffer2->head == _rx_buffer2->tail)
  {
    return -1;
  }
  else
  {
    UINT8 c = _rx_buffer2->buffer[_rx_buffer2->tail];
    _rx_buffer2->tail = (UINT16)(_rx_buffer2->tail + 1) % SERIAL2_BUFFER_SIZE;
    return c;
  }
}

size_t Serial2_readBytes(char *buffer, size_t length)
{
  size_t count = 0;
  while(count < length)
  {
    int c = Serial2_timedRead();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
  }
  return count;
}

size_t Serial2_readBytesUntil(char terminator, char *buffer, size_t length)
{
  if(length < 1) return 0;
  size_t index = 0;
  while(index < length)
  {
    int c = Serial2_timedRead();
    if (c < 0 || c == terminator) break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}

void Serial2_flush(void)
{
  // TSR is kept full while the buffer is not empty, so TXC triggers when EMPTY && SENT
  while(transmitting2 && !U2STAbits.TRMT);
  transmitting2 = false;
}

size_t Serial2_write(UINT8 c)
{
  int i = (_tx_buffer2->head + 1) % SERIAL2_BUFFER_SIZE;

  while(i == _tx_buffer2->tail);

  _tx_buffer2->buffer[_tx_buffer2->head] = c;
  _tx_buffer2->head = i;

  IFS1bits.U2TXIF = 1; // Set UART2 Transmit Interrupt flag
  IEC1bits.U2TXIE = 1; // Enable UART2 Transmit Interrupt
  transmitting2 = true;

  return 1;
}

void Serial2_printString(const char *s)
{
    while(*s) Serial2_write(*s++);
}

void Serial2_printNumber(long n, UINT8 base)
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

  while(*s) Serial2_write(*s++);

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
//    if(di[j - 1] < 10) Serial2_write(di[j - 1] + '0');
//    else Serial2_write(di[j - 1] - 10 + 'A');
//  }
}

void Serial2_printFloat(double number, UINT8 point)
{
  if(number > 4294967040.0) Serial2_printString("ovf");  // constant determined empirically
  else if(number <-4294967040.0) Serial2_printString ("ovf");  // constant determined empirically
  else
  {
    // Handle negative numbers
    if(number < 0.0)
    {
      Serial2_write('-');
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
    Serial2_printNumber(int_part, DEC);

    // Print the decimal point, but only if there are digits beyond
    if(point > 0) {
      Serial2_write('.');
    }

    // Extract digits from the remainder one at a time
    while(point-- > 0)
    {
      remainder *= 10.0;
      int toPrint = (int)remainder;
      Serial2_printNumber(toPrint, DEC);
      remainder -= toPrint;
    }
  }
}

