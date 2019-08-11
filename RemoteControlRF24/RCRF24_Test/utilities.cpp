#include <Arduino.h>
#include <avr/wdt.h>
#include "define.h"

void setLed(int16_t led, int16_t on) {
  if (led == LED_RED) {
    IO_LED_GREEN_OFF;
    if (on) IO_LED_RED_ON; else IO_LED_RED_OFF;
  }
  else if (led == LED_GREEN) {
    IO_LED_RED_OFF;
    if (on) IO_LED_GREEN_ON; else IO_LED_GREEN_OFF;
  }
  else if (led == LED_YELLOW) {
    if (on) IO_LED_RED_ON; else IO_LED_RED_OFF;
    if (on) IO_LED_GREEN_ON; else IO_LED_GREEN_OFF;
  }
}

void doReboot() {
  wdt_enable(WDTO_1S /*WDTO_15MS*/);
  while (1);
}

void putInt(int16_t i, int16_t om) {
  if (i == UNDEFINED) return;
  int16_t j = (i == 0 ? 1 : i);
  if (i < 0) {
    i = -i;
    Serial.write('-');
  }
  while (om > j) {
      om /= 10;
      Serial.write(' ');
  }
  while (om) {
    j = i / om;
    Serial.write(j + '0');
    i -= om * j;
    om /= 10;
  }
}

