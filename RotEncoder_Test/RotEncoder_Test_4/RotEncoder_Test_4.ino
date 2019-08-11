#include <SoftwareSerial.h>
#include <SerialLCD_BPP4X0.h>
#include <rotary.h>

int counter = 0;

const int rot_pb_pin = 9;

SerialLCD_BPP4X0 lcd(12); // default speed

Rotary rot = Rotary(10, 11);

void setup() {                
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);   
  digitalWrite(13, LOW);

  // Push button pin with pullup.
  pinMode(rot_pb_pin, INPUT);   
  digitalWrite(rot_pb_pin, HIGH);
  
  lcd.clear();
  lcd.write("hello");
}

char* selectMenu(char* menu[], int szMenu, int moved, int* menuRet) {
  *menuRet = *menuRet + moved;
  if (*menuRet < 0) *menuRet = 0;
  if (*menuRet >= szMenu) *menuRet = szMenu - 1;
  return menu[*menuRet];
}

char* menu[] = {"exit", "alfa", "beta", "gamma", "delta"};
int szMenu = 5;
int mvd;
int menuSel = 2;
int menuRet = menuSel;
int first = true;
int insideMenu = true;

void loop() {
  digitalWrite(13, insideMenu);

  if (digitalRead(rot_pb_pin) == 0) {
    //while (digitalRead(rot_pb_pin) == 0) delay(10);
    
    if (insideMenu) {
      if (menuRet == 0) {
        insideMenu = false;
      }
      else {
        menuSel = menuRet;
        first = true;
      }
    }
    else {
      menuRet = menuSel;
      insideMenu = true;
      first = true;
    }
  }
  
  if (insideMenu) {
    unsigned char result = rot.process();
    mvd = (result == 0 ? 0 : (result == DIR_CW ? 1 : -1));
    char* menuLbl = selectMenu(menu, szMenu, mvd, &menuRet);
    if (mvd || first) {
      first = false;
      lcd.cursorTo(1, 1);
      lcd.write(menuLbl); 
      if (menuRet == menuSel) lcd.write("*");
      lcd.write("    ");
    }
  }
  
  else {
    lcd.clear();
    lcd.write(menu[menuSel]);
    delay(1000);
  }
}  
  
