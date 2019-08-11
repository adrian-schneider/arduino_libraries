// Usage
//
//   MAX7219 display(pin_din, pin_cs, pin_clk);
// 
//   display.init(...);
//
//   display.output(addr, data);
//   ...

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
#define MAX7219_DT_NORMAL         0x00
#define MAX7219_DT_TEST           0x01

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

// Code B font vakues
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

class MAX7219 {
  public:
    MAX7219(byte pinDIN, byte pinCS, byte pinCLK);
    
    void output(byte address, byte data);
    
    void init(
      int  test_delay  = 0,
      byte intensity   = MAX7219_INT_MAX, 
      byte decode_mode = MAX7219_DM_CODE_B_ALL, 
      byte scan_limit  = 0x07
    );
    
    void write(long value, int size = 8, int offset = 0, int dp = -1);
    
  private:
    byte _pinDIN, _pinCS, _pinCLK; // pins to MAX7219 data, chip-select and clock
};

MAX7219::MAX7219(byte pinDIN, byte pinCS, byte pinCLK) {
  _pinDIN = pinDIN;
  _pinCS  = pinCS;
  _pinCLK = pinCLK;
  digitalWrite(_pinCS, HIGH);
  pinMode(_pinDIN, OUTPUT);
  pinMode(_pinCS,  OUTPUT);
  pinMode(_pinCLK, OUTPUT);
}

void MAX7219::output(byte address, byte data) {
  digitalWrite(_pinCS, LOW);
  shiftOut(_pinDIN, _pinCLK, MSBFIRST, address);
  shiftOut(_pinDIN, _pinCLK, MSBFIRST, data);
  digitalWrite(_pinCS, HIGH);
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

void MAX7219::write(long value, int size, int offset, int dp) {
  int i = offset, k = size;
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
    output(MAX7219_R_DIGIT(i), 0);
    i++;
    k--;
  }
  // Non-null values.
  else {
    while (value != 0 && k > 0) {
      output(MAX7219_R_DIGIT(i), (value % 10) | (i == dp ? MAX7219_B_DP : 0));
      value /= 10;
      i++;
      k--;
    }
  }
  // Fill blanks as needed or 0s before the decimal piint.
  while (k-- > 0) {
    output(MAX7219_R_DIGIT(i), (i <= dp ? 0 : MAX7219_B_BLANK) | (i == dp ? MAX7219_B_DP : 0));
    i++;
  }
  // Finally set the '-' signe if negative.
  if (neg) {
    output(MAX7219_R_DIGIT(i - 1), MAX7219_B_DASH);
  }
}

#define MAX7219_DIN 2
#define MAX7219_CS  3
#define MAX7219_CLK 4

MAX7219 led8x7(MAX7219_DIN, MAX7219_CS, MAX7219_CLK);

void setup() {
  led8x7.init(2000,
    MAX7219_INT_MAX >> 1, 
    MAX7219_DM_CODE_B_ALL & ~(MAX7219_DM_CODE_B(0) | MAX7219_DM_CODE_B(1))
  );
  
  led8x7.output(MAX7219_R_DIGIT(0), MAX7219_SEG_A | MAX7219_SEG_F | MAX7219_SEG_E | MAX7219_SEG_D);
  led8x7.output(MAX7219_R_DIGIT(1), MAX7219_SEG_A | MAX7219_SEG_B | MAX7219_SEG_G | MAX7219_SEG_F);
  //led8x7.output(MAX7219_R_DIGIT(2), 0x03);
  //led8x7.output(MAX7219_R_DIGIT(3), 0x04);

  //led8x7.output(MAX7219_R_DIGIT(4), MAX7219_B_P);
  //led8x7.output(MAX7219_R_DIGIT(5), MAX7219_B_L);
  //led8x7.output(MAX7219_R_DIGIT(6), MAX7219_B_E);
  //led8x7.output(MAX7219_R_DIGIT(7), MAX7219_B_H);
}

int dp = -1;

void loop() {
  for (int i = -1500; i <= 1500; i += 10) {
    led8x7.write(i, 5, 2, dp);
    delay(abs(i) < 50 ? 1000 : 100);
  }
  dp = (dp < 0 ? 3 : -1);
  delay(1000);
}
