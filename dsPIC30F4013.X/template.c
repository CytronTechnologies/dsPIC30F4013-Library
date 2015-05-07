/*
 * File:   template.c
 * Author: Idris
 * Software: MPLAB X IDE V2.26
 * Compiler: XC16 V1.21
 * Created on January 12, 2015
 */

#include "Arduino.h"

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
}

void loop()
{
  if(digitalRead(SW1) == LOW)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    while(digitalRead(SW1) == LOW);
    delay(100);
  }
  else if(digitalRead(SW2) == LOW)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    while(digitalRead(SW2) == LOW);
    delay(100);
  }
}
