/*
SerialLCD_BPI2XX.h
Library which provides functions to use Seetron serial LCD modules
like the BPI-216 or displays equipped with the Serial Backpack BPK000.

To use, include:
  <SoftwareSerial.h>
  <SerialLCD_BPI2XX.h>
*/
#ifndef SerialLCD_BPI2XX_h
#define SerialLCD_BPI2XX_h

#include "Arduino.h"

class BPI2XX
{
  public:
    static const char CLS      = 0x01; // requires 1ms delay at 9600bps
    static const char HOME     = 0x02; // requires 1ms delay at 9600bps
    static const char BLANK    = 0x08;
    static const char RESTORE  = 12;
    static const char SCROLL   = 24;
    static const char SCROLR   = 28;
    static const char HIDECSR  = 12;
    static const char BLKCSR   = 13;
    static const char ULCSR    = 14;
    static const char CGRAM    = 64;
    static const char POSCSR   = 128;
    static const char INSTR    = 0xfe;
};

class SerialLCD_BPI2XX
{
  public:
    /*
    Currently, the Seetron serial displays support 2400 or 9600 bps.
    The rx pin is unused but has to be defined.
    */
    SerialLCD_BPI2XX(int unused_rx_pin, int pin, long speed=9600);
    
    // General output
    
    void write(char c);
    void write(const char *str);
    // println ist not supported on these displays.
    
    // Display control
    
    void clear();
    
    void home();
    
    /*
    Move the cursor.
    You may specify a position as (x, y) or directly as (0, n) using
    the following layouts:
               1 Line LCD
         01234...
         0
       
               2 Lines LCD         4 Lines LCD
         01234...            01234...  
         0                   0   
        64                  64
                            20
                            84
    */
    void cursorTo(char x, char y);
    
    /*
    Send an instruction c to the display.
    */
    void instr(char c);
    
    // Custom characters
    
    /*
    Define custom character n (0-7, ASCII 0 to 7) using bit pattern
    bytes B0-B7 with bits 0 to 4.
      bit     4 3 2 1 0
      value  16 8 4 2 1
      byte 0  x x x x x 
      byte 1  x x x x x
      ...
      byte 7  x x x x x
    */
    void defineCC(int n, const char *ccB0_B7);
    
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

#endif // SerialLCD_BPI2XX_h
