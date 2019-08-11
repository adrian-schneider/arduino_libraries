//
// MAX7219.h
//
// Basic functions to support the MAX7219 display controller. This controller is
// commonly used with seven gement displays, but can be used with single LEDs or
// lamps too.
// Check the data sheet for details on how to use address and data values defined
// below.
//
// The MAX7219 chips can be cascaded to extend the number of digits displayed.
// This library is inteneded and tested with a single chip only.
//
// Usage: 
//   #include <MAX7219.h>
//
//   /* Construct object and assign signal pins. */
//   MAX7219 display(pin_din, pin_cs, pin_clk);
// 
//   /* Initialize the object with appropriate values. */
//   display.init(...);
//
//   /* Write to the display. */
//   display.output(addr, data);
//   ...
//

#ifndef MAX7219_h
#define MAX7219_h

#include "Arduino.h"

// Register address values
#define MAX7219_R_NOOP           0x00
#define MAX7219_R_DIGIT0         0x01
#define MAX7219_R_DIGIT(n)       (MAX7219_R_DIGIT0 + (n)) // digit n = 0..7
#define MAX7219_R_DECODE_MODE    0x09
#define MAX7219_R_INTENSITY      0x0a
#define MAX7219_R_SCAN_LIMIT     0x0b
#define MAX7219_R_SHUTDOWN       0x0c
#define MAX7219_R_DISPLAY_TEST   0x0f

// Display test mode values
#define MAX7219_DT_NORMAL        0x00
#define MAX7219_DT_TEST          0x01

// Intensity values
// 0 is lowest intensity, but not completely off.
#define MAX7219_INT_MAX          0x0f

// Shutdown modes
#define MAX7219_SD_SHUTDOWN      0x00
#define MAX7219_SD_NORMAL        0x01

// Decode mode values
#define MAX7219_DM_NO_DECODE     0x00
#define MAX7219_DM_CODE_B_ALL    0xff
#define MAX7219_DM_CODE_B(n)     (1 << (n)) // digit n = 0..7

// Code B font values
// '0'..'9' = 0x00..0x09
#define MAX7219_B_DASH           0x0a
#define MAX7219_B_E              0x0b
#define MAX7219_B_H              0x0c
#define MAX7219_B_L              0x0d
#define MAX7219_B_P              0x0e
#define MAX7219_B_BLANK          0x0f
#define MAX7219_B_DP             0x80 // binary add to other values

// No-decode mode data bits and segments
//        a
//       ---
//   f / g / b    dp a b c d e f g  segment
//     ---         7 6 5 4 3 2 1 0  data bit
// e /   / c
//   ---   o
//   d     dp
#define MAX7219_SEG_DP           0x80
#define MAX7219_SEG_A            0x40
#define MAX7219_SEG_B            0x20
#define MAX7219_SEG_C            0x10
#define MAX7219_SEG_D            0x08
#define MAX7219_SEG_E            0x04
#define MAX7219_SEG_F            0x02
#define MAX7219_SEG_G            0x01

// No-decode font characters.
#define MAX7219_NDF_0     (MAX7219_SEG_A | MAX7219_SEG_B | MAX7219_SEG_C | MAX7219_SEG_D | MAX7219_SEG_E | MAX7219_SEG_F) 
#define MAX7219_NDF_1     (MAX7219_SEG_B | MAX7219_SEG_C)
#define MAX7219_NDF_2     (MAX7219_SEG_A | MAX7219_SEG_B | MAX7219_SEG_E | MAX7219_SEG_D | MAX7219_SEG_G)
#define MAX7219_NDF_3     (MAX7219_SEG_A | MAX7219_SEG_B | MAX7219_SEG_C | MAX7219_SEG_D | MAX7219_SEG_G)
#define MAX7219_NDF_4     (MAX7219_SEG_B | MAX7219_SEG_C | MAX7219_SEG_F | MAX7219_SEG_G)
#define MAX7219_NDF_5     (MAX7219_SEG_A | MAX7219_SEG_C | MAX7219_SEG_D | MAX7219_SEG_F | MAX7219_SEG_G) 
#define MAX7219_NDF_6     (MAX7219_SEG_A | MAX7219_SEG_C | MAX7219_SEG_D | MAX7219_SEG_E | MAX7219_SEG_F | MAX7219_SEG_G) 
#define MAX7219_NDF_7     (MAX7219_SEG_A | MAX7219_SEG_B | MAX7219_SEG_C) 
#define MAX7219_NDF_8     (MAX7219_SEG_A | MAX7219_SEG_B | MAX7219_SEG_C | MAX7219_SEG_D | MAX7219_SEG_E | MAX7219_SEG_F | MAX7219_SEG_G) 
#define MAX7219_NDF_9     (MAX7219_SEG_A | MAX7219_SEG_B | MAX7219_SEG_C | MAX7219_SEG_D | MAX7219_SEG_F | MAX7219_SEG_G) 
#define MAX7219_NDF_DASH  (MAX7219_SEG_G)
#define MAX7219_NDF_BLANK 0

// No-decode font as array initialzer. 
// Character positions correspond with respective positions in b-decode mode.
#define MAX7219_NODECODE_FONT { MAX7219_NDF_0, MAX7219_NDF_1, MAX7219_NDF_2, MAX7219_NDF_3, MAX7219_NDF_4, MAX7219_NDF_5, \
                                MAX7219_NDF_6, MAX7219_NDF_7, MAX7219_NDF_8, MAX7219_NDF_9, MAX7219_NDF_DASH, 0, 0, 0, 0, \
                                MAX7219_NDF_BLANK }

#define MAX7219_WMODE_OFF        0
#define MAX7219_WMODE_ON         1
#define MAX7219_WMODE_NEUTRAL    2

class MAX7219 {
  public:
    // Construct MAX7219 object.
    // pinDIN: data input pin.
    // pinCS:  chip select pin.
    // pinCLK: clock pin. 
    MAX7219(byte pinDIN, byte pinCS, byte pinCLK);

    // Initialize MAX7219 object. Call this function before any other function after
    // constructing the object.
    // test_delay:  in mili seconds, show all segments lit at startup.
    // intensity:   display brightnes.
    // decode_mode: B-code to display build-in characters, no-decode to control segments.
    // scan_limit:  index of last active digit. Usually left at 7.
    void init(
      int  test_delay  = 0,
      byte intensity   = MAX7219_INT_MAX, 
      byte decode_mode = MAX7219_DM_CODE_B_ALL, 
      byte scan_limit  = 0x07
    );

    // Output address and data bytes to the display. This is the generic control primitive for
    // the MAX7219 chip.    
    void output(byte address, byte data);

    // Write segments using bit mask. Intended for use with no-decode mode to overlay a string
    // of characters with additional segments. This can be used to emulate a blinking cursor.
    // str:    string of bytes to store character segment patterns.
    // ssize:  length of str.
    // offset: start position in str to write at.
    // mask:   segment bit mask to apply to each active position in bmask.
    // bmask:  position bit mask, 1: mask is applied to that position, 0: position left unchanged.
    // mode:   MAX7219_WMODE_ON:      mask is or-ed with active positions in str.
    //         MAX7219_WMODE_OFF:     inverse mask is and-ed with active positions in str. 
    //         MAX7219_WMODE_NEUTRAL: str is left unaltered.
    // The user has to assert that boundaries are not overwritten.
    void writeMask(byte str[], int ssize, int offset, char mask, byte bmask, int mode);
    
    // Write a value to the display or to a string of bytes. The string can be written to the display
    // using writeMask.
    // value:  integer value to write.
    // ssize:  length of field to write.
    // offset: start position to write at.
    // dp:     position to display a decimal point. Set -1 for no decimal point.
    // sdisp:  string of bytes to write to. Set to 0 to write directly to the display
    // scode:  Segment pattern to encode each character if no-decode mode is used.
    //         Set 0 if b-decode mode is used. Character positions must be the same as
    //         for the b-decode mode. You may use the predefined no-decode font.
    void writeVal(long value, int ssize = 8, int offset = 0, int dp = -1, byte sdisp[] = 0, byte scode[] = 0);

  private:
    byte _pinDIN, _pinCS, _pinCLK; // pins to MAX7219 data, chip-select and clock
};

#endif // MAX7219_h
