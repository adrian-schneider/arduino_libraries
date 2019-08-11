//
// MAX7219.cpp
//
// Usage: include <MAX7219.h>
//

#include "MAX7219.h"

MAX7219::MAX7219(byte pinDIN, byte pinCS, byte pinCLK) {
  _pinDIN = pinDIN;
  _pinCS  = pinCS;
  _pinCLK = pinCLK;
  digitalWrite(_pinCS, HIGH);
  pinMode(_pinDIN, OUTPUT);
  pinMode(_pinCS,  OUTPUT);
  pinMode(_pinCLK, OUTPUT);
}

void MAX7219::init(int test_delay, byte intensity, byte decode_mode, byte scan_limit) {
  output(MAX7219_R_SHUTDOWN,     MAX7219_SD_NORMAL);
  output(MAX7219_R_INTENSITY,    intensity);
  output(MAX7219_R_DECODE_MODE,  decode_mode);
  output(MAX7219_R_SCAN_LIMIT,   scan_limit);
  if (test_delay) {
    output(MAX7219_R_DISPLAY_TEST, MAX7219_DT_TEST);
    delay(test_delay);
  }
  output(MAX7219_R_DISPLAY_TEST, MAX7219_DT_NORMAL);
  for (byte i = 0; i < 8; i++) {
    output(MAX7219_R_DIGIT(i), MAX7219_B_BLANK);
  }
}

void MAX7219::output(byte address, byte data) {
  digitalWrite(_pinCS, LOW);
  shiftOut(_pinDIN, _pinCLK, MSBFIRST, address);
  shiftOut(_pinDIN, _pinCLK, MSBFIRST, data);
  digitalWrite(_pinCS, HIGH);
}

void MAX7219::writeMask(byte str[], int ssize, int offset, char mask, byte bmask, int mode) {
  for(int i = 0; i < ssize; i++) {
    char c = str[i];
    if ((1 << i) & bmask) {
      if (mode == MAX7219_WMODE_ON) {
        c |= mask;
      }
      else if (mode == MAX7219_WMODE_OFF) {
        c &= ~mask;
      }
    }
    output(MAX7219_R_DIGIT(i + offset), c);
  }
}


void MAX7219::writeVal(long value, int ssize, int offset, int dp, byte sdisp[], byte scode[]) {
  int i = offset, k = ssize;
  byte b = 0;
  dp += offset;
  // Display absolute value and set the '-' later if negative.
  bool neg = 0;
  if (value < 0) {
    value = -value;
    neg = 1;
  }

  // Display a 0 when no decimal point is desired, i.e dp < i.
  // Null-values with decimal point are handled below.
  if (value == 0 && dp < i) {
    b = 0;
    if (scode != NULL) {
      b = scode[b];
    }
    if (sdisp != NULL) {
      sdisp[i] = b;
    }
    else { 
      output(MAX7219_R_DIGIT(i), b);
    }
    i++;
    k--;
  }
  
  // Non-null values.
  else {
    while (value != 0 && k > 0) {
      b = (value % 10);
      if (scode != NULL) {
        // Set the decimal segment if needed.
        b = scode[b];
        b |= (i == dp ? MAX7219_SEG_DP : 0);
      }
      else {
        // Set the decimal point bit if needed.
        b |= (i == dp ? MAX7219_B_DP : 0);
      }
      if (sdisp != NULL) {
        sdisp[i] = b;
      }
      else {
        output(MAX7219_R_DIGIT(i), b);
      }
      value /= 10;
      i++;
      k--;
    }
  }
  
  // Fill blanks or zeroes as needed before the decimal piint.
  while (k-- > 0) {
    b = (i <= dp ? 0 : MAX7219_B_BLANK);
    if (scode != NULL) {
      b = scode[b];
      // Set the decimal point segment if needed.
      b |= (i == dp ? MAX7219_SEG_DP : 0);
    }
    else {
      // Set the decimal point bit if needed.
      b |= (i == dp ? MAX7219_B_DP : 0);
    }
    if (sdisp != NULL) {
      sdisp[i] = b;
    }
    else {
      output(MAX7219_R_DIGIT(i), b);
    }
    i++;
  }
  
  // Finally set the '-' signe if negative.
  if (neg) {
    b = MAX7219_B_DASH;
    if (scode != NULL) {
      b = scode[b];
    }
    if (sdisp != NULL) {
      sdisp[i - 1] = b;
    }
    else {
      output(MAX7219_R_DIGIT(i - 1), b);
    }
  }
}

