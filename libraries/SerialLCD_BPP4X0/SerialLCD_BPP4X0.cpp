/*
SerialLCD_BPP4X0.cpp
Library which provides functions to use Seetron serial LCD modules
like the BPP-420 or BPP-440.

To use, include:
  <SoftwareSerial.h>
  <SerialLCD_BPP4X0.h>
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "SerialLCD_BPP4X0.h"

SerialLCD_BPP4X0::SerialLCD_BPP4X0(int unused_rx_pin, int pin, long speed) :
  _lcd(unused_rx_pin, pin, true) // rx not used; invert signal.
{
  _lcd.begin(speed);
  digitalWrite(pin, 0); // required to get the first character sent correctly.
}

void SerialLCD_BPP4X0::write(char c)
{
  _lcd.write(c);
}

void SerialLCD_BPP4X0::write(const char *str)
{
  _lcd.write(str);
}

void SerialLCD_BPP4X0::println(const char *str)
{
  _lcd.println(str);
}

void SerialLCD_BPP4X0::clear()
{
  _lcd.write(BPP4X0::FF);
  delay(10);
}

void SerialLCD_BPP4X0::cursorTo(char x, char y)
{
  _lcd.write(BPP4X0::POSCSR);
  _lcd.write(20*y + x + 64);
}

void SerialLCD_BPP4X0::rightAlign(int n, const char *str)
{
  _lcd.write(BPP4X0::RALIGN);
  _lcd.write(n + '0');
  _lcd.write(str);
}

void SerialLCD_BPP4X0::defineCC(int n, const char *ccB0_B7)
{
  _lcd.write(BPP4X0::ESC);
  _lcd.write('D');
  _lcd.write(n);
  for (int i=0; i<8; i++) {
    _lcd.write(ccB0_B7[i]);
  }
}

void SerialLCD_BPP4X0::restoreCC()
{
  _lcd.write(BPP4X0::ESC);
  _lcd.write('E');
  _lcd.write(1);
}


SoftwareSerial &SerialLCD_BPP4X0::getSerial()
{
  return _lcd;
}
