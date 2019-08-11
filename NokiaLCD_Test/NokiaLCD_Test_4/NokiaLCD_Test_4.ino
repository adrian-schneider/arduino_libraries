//
//                       nc
//                       nc
//                       13 R1 - LCD-CLK prp *)
//                       nc
// nc                    11 R2 - LCD-DIN blu *)
// 3.3V LCD-VCC gry      nc
// nc                    nc
// GND LCD-GND  blk      nc
// GND LCD-BL   wht
// nc                     7 R1 - LCD-CLK prp
//                        6 R2 - LCD-DIN blu
// nc                     5 R3 - LCD-DC  org
// nc                     4 R4 - LCD-CE  red
// nc                     3 R5 - LCD-RST brn
// nc                    nc
// nc                    nc
// nc                    nc
//
// R1..R4 = 10k
// *) HW SPI

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

#define XPOS 0
#define YPOS 1
#define DELTAY 2

void setup()   {
  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  
  /*
  display.setCursor(0,0);
  display.println("Hello, world!");
  
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println("3.141592");

  display.display();
  delay(2000);
  
  display.clearDisplay();
  */
}

int i = 0;

void loop() {
  display.setCursor(5,7);
  display.print(i++);
  display.display();
  //delay(1000);
}

