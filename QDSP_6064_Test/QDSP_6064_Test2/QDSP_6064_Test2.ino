// QDSP 6064 Pin Assignment
//    12      Anodes
//     -
//  9 | | 11
//     -   7
//  2 | |  3
//     - . 5
//     8      Cathodes: 8... 1; .8.. 10; ..8. 4; ...8 6
//

uint8_t latchPin = 8; // proto boeard: green
uint8_t clockPin = 12; // proto boeard: yellow
uint8_t dataPin = 11; // proto boeard: blue

// segments for each character
uint8_t segs[] = {
// 11 8 2 5
// 219 7 3
  B11110110, // 00 '0'
  B01000010, // 01 '1'
  B11011100, // 02 '2'
  B11011010, // 03 '3'
  B01101010, // 04 '4'
  B10111010, // 05 '5'
  B10111110, // 06 '6'
  B11000010, // 07 '7'
  B11111110, // 08 '8'
  B11111010, // 09 '9'
  B00000000, // 0a ' '
  B00000001, // 0b '.'
};

#define DISPLAY_DIGITS 4

//                         4321   bit 0 unused in prototype
uint8_t blink_digits = B11101011; // 1 steady; 0 blink
uint8_t halfb_digits = B11101111; // 1 half bright; 0 off

uint8_t displayChars[DISPLAY_DIGITS] = {0, 1, 2, 3};
uint8_t displaySegs[DISPLAY_DIGITS];

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
  
  // store segments for each character position
  for (uint8_t i = 0; i < DISPLAY_DIGITS; i++) {
    displaySegs[i] = segs[displayChars[i]];
  }
}

uint8_t pos = 0;
uint8_t digit = 0;
uint8_t on_digits = 0xff;
uint8_t hb_digits = 0xff;

unsigned long prevMillis = 0;
unsigned long currentMillis = 0;
unsigned long interval = 500; // ms

void loop()
{
  currentMillis = millis();
  
  if (pos == 0) { // only switch digits at start of cycle
    if (currentMillis - prevMillis > interval) {
      prevMillis = currentMillis;
      
      on_digits = ~on_digits;
    }
    
    hb_digits = ~hb_digits;
  }
  
  digit = (2 << pos) & (blink_digits | on_digits | (hb_digits & halfb_digits)); // bit 0 unused in prototype
    
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  shiftOut(dataPin, clockPin, MSBFIRST, displaySegs[pos]);
  digitalWrite(latchPin, HIGH);
  
  ++pos %= DISPLAY_DIGITS;
}
