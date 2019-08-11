// Usage
//
//   MAX7219 display(pin_din, pin_cs, pin_clk);
// 
//   display.init(...);
//
//   display.output(addr, data);
//   ...

#include <MAX7219.h>

#define MAX7219_DIN 2
#define MAX7219_CS  3
#define MAX7219_CLK 4


class MAX7219_2: public MAX7219 {
  public:
    // Construct MAX7219 object.
    // pinDIN: data input pin.
    // pinCS:  chip select pin.
    // pinCLK: clock pin. 
    MAX7219_2(byte pinDIN, byte pinCS, byte pinCLK) : MAX7219(pinDIN, pinCS, pinCLK) {}
};


MAX7219_2 led8x7(MAX7219_DIN, MAX7219_CS, MAX7219_CLK);

void setup() {
/*led8x7.init(1000,
    MAX7219_INT_MAX >> 1, 
    MAX7219_DM_NO_DECODE
  );
*/  

  led8x7.init(1000,
    MAX7219_INT_MAX >> 1, 
    MAX7219_DM_CODE_B_ALL & ~(MAX7219_DM_CODE_B(0) | MAX7219_DM_CODE_B(1))
  );

  led8x7.output(MAX7219_R_DIGIT(0), 0);
  led8x7.output(MAX7219_R_DIGIT(1), 0);
  led8x7.output(MAX7219_R_DIGIT(7), 0);
}

byte str[] = { MAX7219_NDF_0, MAX7219_NDF_1, MAX7219_NDF_2, MAX7219_NDF_3 | MAX7219_SEG_DP, MAX7219_NDF_4 };
int slen = 5;
int ofst = 2;
byte mask = MAX7219_SEG_D /*| MAX7219_SEG_DP*/;
byte bmask = 1;

int dp = -1;
byte sdisp[8];
byte scode[] = MAX7219_NODECODE_FONT;
  
void loop() {
  /*
  for (byte k = 0; k < 5; k++) {
    bmask = 1 << k;
    for (int i = 0; i < 5; i++) {
      writeMask(str, slen, ofst, mask, bmask, MAX7219_WMODE_ON);
      delay(200);
      writeMask(str, slen, ofst, mask, bmask, MAX7219_WMODE_OFF);
      delay(200);
    }
    writeMask(str, slen, ofst, mask, bmask, MAX7219_WMODE_NEUTRAL);
    //delay(5000);
  }
  */
  
  for (int i = -1500; i <= 1500; i += 10) {
    // write direct: sdisp = 0, scode = 0.
    led8x7.writeVal(i, 5, 2, dp);

    // Write directly to display: sdisp = 0, encode with scode.
/*  led8x7.writeVal(i, 5, 0, dp, 0, scode);
*/
    // write to buffer: sdisp, encode with scode, then write buffer to display.
/*  led8x7.writeVal(i, 5, 0, dp, sdisp, scode);
    led8x7.writeMask(sdisp, 5, 2, 0, 0, 0);
*/    
    delay(abs(i) < 50 ? 1000 : 100);
  }
  dp = (dp < 0 ? 3 : -1);
  
  led8x7.output(MAX7219_R_SHUTDOWN, MAX7219_SD_SHUTDOWN);
  delay(1000);
  led8x7.output(MAX7219_R_SHUTDOWN, MAX7219_SD_NORMAL);
}
