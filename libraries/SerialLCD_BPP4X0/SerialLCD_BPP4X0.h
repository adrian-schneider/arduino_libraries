/*
SerialLCD_BPP4X0.h
Library which provides functions to use Seetron serial LCD modules
like the BPP-420 or BPP-440.

To use, include:
  <SoftwareSerial.h>
  <SerialLCD_BPP4X0.h>
*/
#ifndef SerialLCD_BPP4X0_h
#define SerialLCD_BPP4X0_h

#include "Arduino.h"

class BPP4X0
{
  public:
    static const char HOME     = 0x01;
    static const char STARTBIG = 0x02;
    static const char ENDBIG   = 0x03;
    static const char HIDECSR  = 0x04;
    static const char ULCSR    = 0x05;
    static const char BLKCSR   = 0x06;
    static const char BELL     = 0x07;
    static const char BSP      = 0x08;
    static const char HT       = 0x09;
    static const char LF       = 0x0a;
    static const char VT       = 0x0b;
    static const char FF       = 0x0c;
    static const char CR       = 0x0d;
    static const char BLON     = 0x0e;
    static const char BLOFF    = 0x0f;
    static const char POSCSR   = 0x10;
    static const char CLRCOL   = 0x11;
    static const char RALIGN   = 0x18;
    static const char ESC      = 0x1b;
};

class SerialLCD_BPP4X0
{
  public:
    /*
    Currently, the Seetron serial displays support 2400 or 9600 bps.
    The rx pin is unused but has to be defined.
    */
    SerialLCD_BPP4X0(int unused_rx_pin, int pin, long speed=9600);
    
    // General output
    
    void write(char c);
    void write(const char *str);
    void println(const char *str);
    
    // Display control
    
    void clear();
    
    /*
    Move the cursor.
    You may specify a position as (x, y) or directly as (0, n) using
    the following layouts:
               4x20 LCD              4x40 LCD
       01234...  19         01234...   39
      20         39        40          79
      40         59        80         119
      60         79       120         159
    */
    void cursorTo(char x, char y);
    
    /*
    Print a string str right-aligned inside a field of width n (2-9).
    Nothing is printed until on of the following is received:
    - The specified number of characters.
    - A control character (ASCII 1-31).
    - A decimal point, i.e. the period (.), ASCII 46.
    */
    void rightAlign(int n, const char *str);
    
    // Custom characters
    
    /*
    Define custom character n (0-7, ASCII 128 to 135) using bit pattern
    bytes B0-B7 with bits 0 to 4.
      bit     4 3 2 1 0
      value  16 8 4 2 1
      byte 0  x x x x x 
      byte 1  x x x x x
      ...
      byte 7  x x x x x
    Big characters won't display correctly any more if you redefine any
    of the custom characters.
    */
    void defineCC(int n, const char *ccB0_B7);
    
    /*
    Restore the default custom characters.
    */
    void restoreCC();
    
    // Special functions.
    
    /*
    Return a reference to the serial object.
    E.g. you may this way use any stream function like:
    lcd.getSerial().print(...);
    */
    SoftwareSerial &getSerial();
    
  private:
    SoftwareSerial _lcd;
};

#endif // SerialLCD_BPP4X0_h
