/*
SerialLCD_BPI2XX.cpp
Library which provides functions to use Seetron serial LCD modules
like the BPI-216 or displays equipped with the Serial Backpack BPK000.

To use, include:
  <SoftwareSerial.h>
  <SerialLCD_BPI2XX.h>
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "SerialLCD_BPI2XX.h"

SerialLCD_BPI2XX::SerialLCD_BPI2XX(int pin, long speed) :
  _lcd(int unused_rx_pin, pin, true) // rx not used; invert signal.
{
  _lcd.begin(speed);
  digitalWrite(pin, 0); // required to get the first character sent correctly.
}

void SerialLCD_BPI2XX::write(char c)
{
  _lcd.write(c);
}

void SerialLCD_BPI2XX::write(const char *str)
{
  _lcd.write(str);
}

void SerialLCD_BPI2XX::clear()
{
  _lcd.write(BPI2XX::INSTR);
  _lcd.write(BPI2XX::CLS);
  delay(10);
}

void SerialLCD_BPI2XX::home()
{
  _lcd.write(BPI2XX::INSTR);
  _lcd.write(BPI2XX::HOME);
  delay(10);
}

void SerialLCD_BPI2XX::cursorTo(char x, char y)
{
  static const char pos0[] = {0,64,20,84}; 
  _lcd.write(BPI2XX::INSTR);
  _lcd.write(BPI2XX::POSCSR + pos0[y&3] + x);
}

void SerialLCD_BPI2XX::instr(char c)
{
  _lcd.write(BPI2XX::INSTR);
  _lcd.write(c);
}

void SerialLCD_BPI2XX::defineCC(int n, const char *ccB0_B7)
{
  _lcd.write(BPI2XX::INSTR);
  _lcd.write(8*n + BPI2XX::CGRAM);
  for (int i=0; i<8; i++) {
    _lcd.write(ccB0_B7[i]);
  }
}

SoftwareSerial &SerialLCD_BPI2XX::getSerial()
{
  return _lcd;
}
