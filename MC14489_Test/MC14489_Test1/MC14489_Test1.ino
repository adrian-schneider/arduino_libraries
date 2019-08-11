// QDSP 6064 Pin Assignment
//    12      Anodes
//     -
//  9 | | 11
//     -   7
//  2 | |  3
//     - . 5
//     8      Cathodes: 8... 1; .8.. 10; ..8. 4; ...8 6
//

uint8_t latchPin = 8;    // proto board: green; /enable in the MC14489 data sheet.
uint8_t clockPin = 12;   // proto board: orange
uint8_t dataPin = 11;    // proto board: white

// Macros to manage the configuration register -- 1 byte.
#define MC14489_C_NORM_PWR(x)          ((x) |= 1)            // normal power mode
#define MC14489_C_LOW_PWR(x)           ((x) &= ~1)           // low power mode
#define MC14489_C_ALT_DECODE(x, b)     ((x) |= (1 << (b)))   // special or no decode, bank b=1..5
#define MC14489_C_HEX_DECODE(x, b)     ((x) &= ~(1 << (b)))  // hex decode bank b=1..5
#define MC14489_C_13_SPC_DECODE(x)     ((x) |= (1 << 6))     // special decode for banks 1..3
#define MC14489_C_13_NO_DECODE(x)      ((x) &= ~(1 << 6))    // no decode for banks 1..3
#define MC14489_C_45_SPC_DECODE(x)     ((x) |= (1 << 7))     // special decode for banks 4..5
#define MC14489_C_45_NO_DECODE(x)      ((x) &= ~(1 << 7))    // no decode for banks 4..5

// Macros to manage the data register -- 3 bytes.
#define MC14489_D_SET_0(px)            *(px) = 0; *((px) + 1) = 0; *((px) + 2) = 0              // set all data bytes 0
#define MC14489_D_SET_X(px, x, i)      *((px) + ((x) >> 1)) |= ((i & 0xf) << (((x) & 1) << 2))  // set nibble i at position x=0..5
#define MC14489_D_SET_X0(px, x)        *((px) + ((x) >> 1)) &= ~(0xf << (((x) & 1) << 2))       // clear nibble at position x=0..5

#define MC14489_D_SET_DEC1(px, i)      *(px) |= (i) % 10                                                  // set 1 digit unsigned decimal
#define MC14489_D_SET_DEC2(px, i)      MC14489_D_SET_DEC1(px, i); *(px) |= (((i) / 10) % 10) << 4         // set 2 digit unsigned decimal
#define MC14489_D_SET_DEC3(px, i)      MC14489_D_SET_DEC2(px, i); *((px) + 1) |= ((i) / 100) % 10         // set 3 digit unsigned decimal
#define MC14489_D_SET_DEC4(px, i)      MC14489_D_SET_DEC3(px, i); *((px) + 1) |= (((i) / 1000) % 10) << 4 // set 4 digit unsigned decimal
#define MC14489_D_SET_DEC5(px, i)      MC14489_D_SET_DEC4(px, i); *((px) + 2) |= ((i) / 10000) % 10       // set 5 digit unsigned decimal

#define MC14489_D_SET_HEX2(px, i)      *(px) = (i) & 0xff                                           // set 2 digit unsigned hex
#define MC14489_D_SET_HEX4(px, i)      MC14489_D_SET_HEX2(px, i); *((px) + 1) = ((i) >> 8) & 0xff   // set 4 digit unsigned hex 

#define MC14489_D_DIM(px)              *((px) + 2) &= 0x7f           // dim LEDs
#define MC14489_D_BRIGHT(px)           *((px) + 2) |= 0x80           // brighten LEDs
#define MC14489_D_SEGH(px, h)          *((px) + 2) |= ((h) & 7) << 4 // set h segments
#define MC14489_D_SEGH0(px)            *((px) + 2) &= 0x8f           // clear all h segments


void mc14489_writeConfig(uint8_t x) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, x);
  digitalWrite(latchPin, HIGH);
}


void mc14489_writeData(uint8_t *x) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, x[2]);
  shiftOut(dataPin, clockPin, MSBFIRST, x[1]);
  shiftOut(dataPin, clockPin, MSBFIRST, x[0]);
  digitalWrite(latchPin, HIGH);
}


void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  uint8_t creg = 0;
  MC14489_C_NORM_PWR(creg);
  MC14489_C_HEX_DECODE(creg, 1);
  MC14489_C_HEX_DECODE(creg, 2);
  MC14489_C_HEX_DECODE(creg, 3);
  MC14489_C_HEX_DECODE(creg, 4);
  MC14489_C_ALT_DECODE(creg, 5);
  MC14489_C_13_SPC_DECODE(creg);
  MC14489_C_45_SPC_DECODE(creg);
  mc14489_writeConfig(creg);
}

unsigned long prevMillis = 0;
unsigned long currentMillis = 0;
unsigned long interval = 100; // ms

uint16_t i = 0;
uint8_t dreg[3];

void loop()
{
  currentMillis = millis();
  
  if (currentMillis - prevMillis > interval) {
    prevMillis = currentMillis;

    MC14489_D_SET_0(dreg);
    //if (i & 0x80) MC14489_D_SEGH(dreg, 7);
    //MC14489_D_SET_HEX4(dreg, i);
    //MC14489_D_SET_DEC4(dreg, i * 100 + i % 100); // create some noise in the lowest 2 places.
    MC14489_D_SET_DEC4(dreg, i);
    //MC14489_D_SET_X0(dreg, (i / 100) % 4);
    //MC14489_D_SET_X0(dreg, 0); // delete the noise in the lowest 2 places
    //MC14489_D_SET_X0(dreg, 1); // dito
    //MC14489_D_SET_X(dreg, 0, 5);
    //MC14489_D_SET_X(dreg, 1, 6);
    mc14489_writeData(dreg);
    
    i++;
  }
}
