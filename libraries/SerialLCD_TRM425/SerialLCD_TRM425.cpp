/*
SerialLCD_TRM425.cpp
Library which provides functions to use Seetron serial LCD modules
like the BPP-420 or BPP-440.

To use, include:
  <SoftwareSerial.h>
  <SerialLCD_TRM425.h>
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "SerialLCD_TRM425.h"

SerialLCD_TRM425::SerialLCD_TRM425(int pinRX, int pinTX, long speed) :
  _lcd(pinRX, pinTX, false) // non-invert signal.
{
  _lcd.begin(speed);
}

void SerialLCD_TRM425::write(char c)
{
  _lcd.write(c);
}

void SerialLCD_TRM425::write(const char *str)
{
  _lcd.write(str);
}

void SerialLCD_TRM425::println(const char *str)
{
  _lcd.println(str);
}

void SerialLCD_TRM425::clear()
{
  _lcd.write(TRM425::FF);
  delay(10);
}

void SerialLCD_TRM425::cursorTo(char x, char y)
{
  _lcd.write(TRM425::POSCSR);
  _lcd.write(20*y + x + 64);
}

void SerialLCD_TRM425::rightAlign(int n, const char *str)
{
  _lcd.write(TRM425::RALIGN);
  _lcd.write(n + '0');
  _lcd.write(str);
}

void SerialLCD_TRM425::defineCC(int n, const char *ccB0_B7)
{
  _lcd.write(TRM425::ESC);
  _lcd.write('D');
  _lcd.write(n);
  for (int i=0; i<8; i++) {
    _lcd.write(ccB0_B7[i]);
  }
}

void SerialLCD_TRM425::restoreCC()
{
  _lcd.write(TRM425::ESC);
  _lcd.write('E');
  _lcd.write(1);
}

char SerialLCD_TRM425::waitChar()
{
  char c = -1;
  int avail;
  
  _lcd.flush();
  while ((avail = _lcd.available()) == 0);
  if (avail > 0) {
    c = _lcd.read();
  }
  
  return c;
}

char SerialLCD_TRM425::readStr(char *buf, int len, char bsp, const char *ceoi)
{
  char c;
  int l = len - 1;
  
  while ((c = waitChar()) != -1 && ! isEOI(c, ceoi)) {
    if (c == bsp) {
      if (l < len - 1) {
        buf--; l++;
        _lcd.write(TRM425::BSP);
      }
    }
    else if (l > 0) {
      *buf++ = c; l--;
      _lcd.write(c);
    }
  }
  *buf = 0;
  
  return c;
}

SoftwareSerial &SerialLCD_TRM425::getSerial()
{
  return _lcd;
}

int SerialLCD_TRM425::isEOI(char c, const char *ceoi)
{
  int t = 0;
  while (*ceoi && !(t = (*ceoi++ == c)));
  return t;
}
