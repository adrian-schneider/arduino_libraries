//
//                       nc
//                       GND rot grn
//                       nc
//                       12  LCD wht
// nc                    11  rot org
// nc                    10  rot brn
// 5V  LCD red            9  rot ylw
// GND LCD blk           nc
// GND rot red
// nc                    nc
//                       nc
// nc                    nc
// nc                    nc
// nc                    nc
// nc                    nc
// nc                    nc
// nc                    nc
//

//#define SIMPLEMENU_DEBUG

#include <SimpleMenu.h>
#include <rotary.h>
#include <SoftwareSerial.h>
#include <SerialLCD_BPP4X0.h>

const int rot_pb_pin = 9;

SerialLCD_BPP4X0 lcd(2, 12); // default speed

Rotary rot = Rotary(10, 11);

SM_RangeDescr menu_1_range = {-100, 100, 1, 6,   2};
SM_RangeDescr menu_3_range = {   0, 255, 1, 6, -16};

const char *menu_0[] = {"DISCARD", "SAVE", "number", "color", "address", NULL}; 
//const char *menu_1[] = {"one", "two", "three", "four", "five", "six", NULL};
const char *menu_1[] = {NULL, (char *)&menu_1_range};
const char *menu_2[] = {"red", "green", "blue", "yellow", "cyan", "magenta", "white", "black", "grey", NULL};
//const char *menu_3[] = {"north", "east", "south", "west", "UP", "RIGHT", "DOWN", "LEFT", NULL};
const char *menu_3[] = {NULL, (char *)&menu_3_range};
const char **menu_all[] = {menu_0, menu_1, menu_2, menu_3};

int menuSel[] = {0, 0, 0, 0};
int menuSaved[] = {0, 1, 0, 0};

class MySimpleMenu: public SimpleMenu {
  public:
    MySimpleMenu(const char ***menu_all, int *menuSel) : 
      SimpleMenu(menu_all, menuSel, 2)
    {};
  
  protected:
    void showMenu(int first, int mindex) {
      if (first) {
        memcpy(menuSel, menuSaved, sizeof(menuSel));
        lcd.clear();
        lcd.write("Setup");
      }
    };

    void showMenuItem(const char *item, int sel) {
      lcd.cursorTo(1, 1);
      lcd.write(item);
      if (sel) lcd.write('*');
      lcd.write("    "); 
    };

    void exitMenu() {
      lcd.clear();
      // If 'Save':
      if (menuSel[0] == 1) memcpy(menuSaved, menuSel, sizeof(menuSel));
      for (int i = 0; i < 4; i++) {
        lcd.cursorTo(0, i);
        lcd.write(menu_0[i]); lcd.write(": ");
        lcd.write(menuItemChr(menu_all[i], menuSaved[i]));
      }
    };
    
    void waitEnteringExiting(int entering) {
      while (digitalRead(rot_pb_pin) == 0);
    }
};

MySimpleMenu sm((const char ***)menu_all, (int *)menuSel);
  
void setup() {
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);   
  digitalWrite(13, LOW);
  
  // Push button pin with pullup.
  pinMode(rot_pb_pin, INPUT);   
  digitalWrite(rot_pb_pin, HIGH);
  
  lcd.clear();
  lcd.write(BPP4X0::BLON);
  lcd.write("Hello");
  
  #ifdef SIMPLEMENU_DEBUG
  Serial.begin(9600); Serial.println("Hello");
  #endif
}

void loop() {
  int button = digitalRead(rot_pb_pin) == 0;
  unsigned char mvd = rot.process();
  
  sm.process(
    mvd ? (mvd == DIR_CW ? 1 : -1) : 0, 
    button
  );
}  

