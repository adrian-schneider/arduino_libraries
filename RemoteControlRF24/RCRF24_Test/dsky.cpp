#include <Arduino.h>
#include "define.h"
#include "dsky.h"
#include "utilities.h"

const int16_t hasParm[] = {
// nnvv
// 1010,  // reboot system
  1098,
  1099,
  1310,  // switch LED on IO 13
  1311,  // switch red LED
  1312,  // switch green LED
  1313,  // switch yellow LED
  0
};

void dskySetup() {
  // Write input mask static text
  Serial.write(DSKY_TRM425_FF);
  // TRM425 requires a small delay after a FF.
  delay(10);
  Serial.write(DSKY_TRM425_XY_TVERB
    DSKY_TRM425_XY_TNOUN
    DSKY_TRM425_XY_TP1
    DSKY_TRM425_XY_TP2
    DSKY_TRM425_XY_TP3);
}

int16_t dskyGetNounVerbParm(int16_t *noun, int16_t *verb, int16_t *parm, int16_t allowNV) {
  char c;
  int16_t sign = 1;
  int16_t done = 0;
  int16_t p2 = 0;
  int16_t *assgn = parm;
  int16_t i = *assgn;
  while (! done) {
    while (! Serial.available());
    c = Serial.read();
    // Enter key has been hit. Simply terminate the input.
    if (c == DSKY_C_ENTER) { //  ENTER
      done = 1;
    }
    // Separate noun and verb input.
    else if (allowNV && c == DSKY_C_NOUN || c == DSKY_C_VERB) {
      assgn = (c == DSKY_C_NOUN ? noun : verb);
#ifdef DSKY_NOUN_UNDEFINES_VERB
      if (c == DSKY_C_NOUN) *verb = UNDEFINED;
#elif defined DSKY_VERB_UNDEFINES_NOUN
      if (c == DSKY_C_VERB) *noun = UNDEFINED;
#endif // DSKY_VERB_UNDEFINES_NOUN
      p2 = 1;
      i = UNDEFINED;
      sign = 1;
    }
    // Add a digit. Input is limited to <= 32767 and can be limited to
    // 2 places (p2), extra digits are shifted left and truncated.
    else if (c >= '0' && c <= '9') {
      if (i == UNDEFINED) i = 0;
      if (p2 && i > 9) i -= 10 * (i / 10);
      // Safely compute i = i * 10 + c - '0', avoiding overflow
      if (i <= MAXINTD10) {
        int16_t j = i * 10 - '0';
        if ( j <= MAXINT - c) i = j + c;
      }
    }
    // Cancel back spaces until value is 0. Once more cancels
    // the input at all.
    else if (c == DSKY_C_CLEAR) {
      if (i == 0 || i == UNDEFINED) done = 2;
      else {
#ifdef DSKY_CANCEL_IS_BACKSPACE
        i /= 10;
#else // DSKY_CANCEL_IS_BACKSPACE
        i = 0;
#endif // DSKY_CANCEL_IS_BACKSPACE
      }
    }
    // Sign change, only param is signed.
    else if (c == DSKY_C_MINUS && assgn == parm && i != UNDEFINED) {
      sign = -sign;
    }
    // Assign value only if input has not been cancelled.
    *assgn = (done != 2 ? sign * i : UNDEFINED);
#ifdef DSKY_DEBUG
    Serial.write(c);
    Serial.write(assgn == noun ? 'n' : (assgn == verb ? 'v' : 'p'));
    Serial.write(':');
    if (assgn == parm) Serial.write(sign == 1 ? '+' : '-');
    putInt(i, 10000);
    Serial.write('\n');
#else
    if (assgn == parm) {
      Serial.write(DSKY_TRM425_XY_P1);
      if (i == UNDEFINED) {
        Serial.write("......");
      }
      else {
        Serial.write(sign > 0 ? '+' : '-');
        putInt(i, 10000);
      }
    }
    else {
      Serial.write(DSKY_TRM425_XY_NOUN);
      if (*noun == UNDEFINED) Serial.write("..");
      else putInt(*noun, 10);
      Serial.write(DSKY_TRM425_XY_VERB);
      if (*verb == UNDEFINED) Serial.write("..");
      else putInt(*verb, 10);
    }
#endif // DSKY_DEBUG
  }
  return done == 2;
}

void dskyProcess() {
  int16_t noun = UNDEFINED, verb = UNDEFINED;
  // Process DSKY until cancelled.
  while (1) {
    int16_t parm = UNDEFINED;
    if (dskyGetNounVerbParm(&noun, &verb, &parm, 1) != 0) {
#ifdef DSKY_DEBUG
      Serial.println("cancelled");
#endif // DSKY_DEBUG
      return;
    }
    else {
      int16_t askParm = 0;
      // No param supplied, may need to ask.
      if (parm == UNDEFINED) {
        int16_t nnvv = 100 * noun + verb;
        for (int16_t j = 0; ! askParm && j < hasParm[j]; j++) askParm = (hasParm[j] == nnvv);
      }
      if (askParm) {
        // Just hitting enter assume parm = 0.
        parm = 0;
#ifndef DSKY_DEBUG
        Serial.write(DSKY_TRM425_XY_P1);
#endif // DSKY_DEBUG
        Serial.write("?.....");
      }
      if (askParm && dskyGetNounVerbParm(&noun, &verb, &parm, 0) != 0) {
#ifdef DSKY_DEBUG
        Serial.println("cancelled");
#endif // DSKY_DEBUG
        return;
      }
      else {
        // Verb-noun-parm evaluation goes here.
        if (noun == 10) {
          if (verb == 10) doReboot();
        }
        else if (noun == 13) {
          if (verb == 10) digitalWrite(13, parm);
          else if (verb == 11) setLed(LED_RED, parm);
          else if (verb == 12) setLed(LED_GREEN, parm);
          else if (verb == 13) setLed(LED_YELLOW, parm);
        }
#ifdef DSKY_DEBUG    
        Serial.print("  n:"); Serial.print(noun); Serial.print(" v:"); Serial.println(verb);
        Serial.print("  p:"); Serial.println(parm);
        Serial.println("--");
#endif // DSKY_DEBUG
      }
    }
  }
}


