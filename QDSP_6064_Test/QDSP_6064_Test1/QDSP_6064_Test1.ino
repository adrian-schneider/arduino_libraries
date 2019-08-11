// QDSP 6064 Pin Assignment
//    12      Anodes
//     -
//  9 | | 11
//     -   7
//  2 | |  3
//     - . 5
//     8      Cathodes: 8... 1; .8.. 10; ..8. 4; ...8 6
//
// Port/Pin Assignment
//     0      Anodes Port D
//     -
//  2 | |  1
//     -   4
//  5 | |  6
//     - . 7
//     3      CathodesPort B: 8... 8; .8.. 9; ..8. 10; ...8 11

void setup()
{
  DDRD = 0xff; // port d (pins 0..7) all outputs
  PORTD = 0xff; // port d all bits high
  
  DDRB |= 0x0f; // port b (pins 8..11) output
  PORTB &= 0x0f; // port b (pins 8..11) bits high
}


void loop()
{
  for (int i = 0; i < 256; i++) {
    PORTD = i;

    for (int j = 0; j < 100; j++) {
      for (int k = 1; k <= 8; k <<= 1) {
        PORTB = (0x0f & ~k);
        delayMicroseconds(1000);
      }
    }
  }
}
