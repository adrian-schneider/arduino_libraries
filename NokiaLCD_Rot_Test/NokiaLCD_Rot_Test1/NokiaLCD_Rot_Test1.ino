//
//                       nc
//                       nc
//                       13 R1 - LCD-CLK prp
//                       nc
// nc                    11 R2 - LCD-DIN blu
// 3.3V LCD-VCC gry      nc
// nc                    nc
// GND LCD-GND  blk       8 RotBtn       org
// GND LCD-BL   wht
// nc                     7 RotA         red
//                        6 RotB         brn
// nc                     5 R3 - LCD-DC  org
// nc                     4 R4 - LCD-CE  red
// nc                     3 R5 - LCD-RST brn
// nc                    nc
// nc                    nc
// nc                    nc
//
// R1..R5 = 10k

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SimpleMenu.h>
#include <rotary.h>

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

Rotary rot = Rotary(6, 7);
const int rot_pb_pin = 8;

SM_RangeDescr menu_1_range = {-100, 100, 1, 6,   2};
SM_RangeDescr menu_3_range = {   0, 255, 1, 6, -16};

const char *menu_0[] = {"ESC", "SAVE", "nmbr", "colr", "addr", NULL}; 
//const char *menu_1[] = {"one", "two", "three", "four", "five", "six", NULL};
const char *menu_1[] = {NULL, (char *)&menu_1_range};
const char *menu_2[] = {"red", "green", "blue", "yellow", "cyan", "magenta", "white", "black", "grey", NULL};
//const char *menu_3[] = {"north", "east", "south", "west", "UP", "RIGHT", "DOWN", "LEFT", NULL};
const char *menu_3[] = {NULL, (char *)&menu_3_range};
const char **menu_all[] = {menu_0, menu_1, menu_2, menu_3};

int menuSel[] = {0, 0, 0, 0};
int menuSaved[] = {0, 0, 0, 0};

class MySimpleMenu: public SimpleMenu {
  public:
    MySimpleMenu(const char ***menu_all, int *menuSel) : 
      SimpleMenu(menu_all, menuSel, 2)
    {};
  
  protected:
    void showMenu(int first, int mindex) {
      if (first) {
        memcpy(menuSel, menuSaved, sizeof(menuSel));
        display.clearDisplay();
        display.print("Setup");
        display.display();
      }
    };

    void showMenuItem(const char *item, int sel, int iindex) {
      display.setCursor(8, 8);
      display.print(item);
      if (sel) display.write('*');
      display.print("    "); 
      display.display();
    };

    void exitMenu() {
      display.clearDisplay();
      // If 'Save':
      if (menuSel[0] == 1) memcpy(menuSaved, menuSel, sizeof(menuSel));
      for (int i = 0; i < 4; i++) {
        display.setCursor(0, i * 8);
        display.print(menu_0[i + 1]); display.print(": ");
        display.print(menuItemChr(menu_all[i], (i == 0 ? menuSel[i] : menuSaved[i])));
      }
      display.display();
    };
    
    void waitEnteringExiting(int entering) {
      while (digitalRead(rot_pb_pin) == 0);
    }
};

MySimpleMenu menu((const char ***)menu_all, (int *)menuSel);

void setup()   {
  display.begin();

  // Push button pin with pullup.
  pinMode(rot_pb_pin, INPUT);   
  digitalWrite(rot_pb_pin, HIGH);

  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
}

void loop() {
  int button = digitalRead(rot_pb_pin) == 0;
  unsigned char mvd = rot.process();
  
  menu.process(
    mvd ? (mvd == DIR_CW ? 1 : -1) : 0, 
    button
  );
}

