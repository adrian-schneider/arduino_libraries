#include <Arduino.h>
#include "define.h"
#include "dsky.h"

void setup() {
  // Clear any states from earlier software reboots.
  MCUSR = 0;
  
  IO_LED_RED_PM;
  IO_LED_GREEN_PM;
  pinMode(13, OUTPUT);
  
  Serial.begin(SERIAL_SPEED);
  
#ifdef DSKY_USE_TRM425
  dskySetup();
#else
  Serial.println("ready");
#endif // DSKY_USE_TRM425
}

void loop() {
  if (Serial.available()) {
    dskyProcess();
  }

  delay(250);
}
