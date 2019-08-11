#ifndef _UTILITIES_H
#define _UTILITIES_H

// Turn LED_RED, LED_GREEN, LED_YELLOW on or off.
// led: LED number
// on:  1 turn on, 0 turn off
void setLed(int16_t led, int16_t on);

// Reboot the Arduino.
// Clear any states from earlier software reboots early on setup.
// e.g. MCUSR = 0;
void doReboot();

// Write an int to stdout using a specified number of places.
// i:  the int
// om: order of magnitude of output, e.g. 1000 for 4 places.
void putInt(int16_t i, int16_t om);

#endif // _UTILITIES_H
