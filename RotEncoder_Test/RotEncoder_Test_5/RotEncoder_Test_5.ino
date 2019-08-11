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
  lcd.write(BPP4X0::BLON);
  lcd.write("hello");
}

#define M_OUTSIDE 0x00
#define M_ENTERING 0x01
#define M_INSIDE 0x02
#define M_EXITING 0x03
#define M_MNUCHG 0x04
#define M_SELCHG 0x05

#define ME_NONE 0x00
#define ME_BUTTON 0x01
#define ME_ROTMVD 0x02

const unsigned char menu_state[6][3] = {
  // M_OUTSIDE
  {M_OUTSIDE, M_ENTERING, M_OUTSIDE},
  // M_ENTERING
  {M_INSIDE, M_INSIDE, M_INSIDE},
  // M_INSIDE
  {M_INSIDE, M_SELCHG, M_MNUCHG},
  // M_EXITING
  {M_OUTSIDE, M_OUTSIDE, M_OUTSIDE},
  // M_MNUCHG
  {M_INSIDE, M_INSIDE, M_INSIDE},
  // M_SELCHG
  {M_EXITING, M_EXITING, M_EXITING}
};

char* selectMenu(char* menu[], int szMenu, int moved, int* menuRet) {
  *menuRet = *menuRet + moved;
  if (*menuRet < 0) *menuRet = 0;
  if (*menuRet >= szMenu) *menuRet = szMenu - 1;
  return menu[*menuRet];
}

void showMenuItem(char *item, int mark) {
  lcd.cursorTo(1, 1);
  lcd.write(item); 
  if (mark) lcd.write("*");
  lcd.write("    ");
}

char* menu[] = {"exit", "alfa", "beta", "gamma", "delta", "epsilon", "eta", "kappa"};
int szMenu = 8;
int menuSel = 2;
int menuRet = 0;
char *menuItem = 0;
unsigned char menu_st = M_OUTSIDE;

void loop() {
  // Rotation and button pushed considered mutually exclusive.  
  unsigned char mvd = rot.process();
  unsigned char menu_ev = mvd ? ME_ROTMVD :
    (digitalRead(rot_pb_pin) == 0) ? ME_BUTTON : ME_NONE;

  if (menu_st == M_ENTERING) {
    menuRet = menuSel;
    lcd.clear();
    lcd.write("Menu");
    showMenuItem(menu[menuSel], true);
    
    while (digitalRead(rot_pb_pin) == 0);
  }
  else if (menu_st == M_INSIDE) {
    menuItem = selectMenu(menu, szMenu, 
      (mvd == 0 ? 0 : (mvd == DIR_CW ? 1 : -1)), 
      &menuRet
    );
  }
  else if (menu_st == M_MNUCHG) {
    showMenuItem(menuItem, menuRet == menuSel);
  }
  else if (menu_st == M_SELCHG) {
    menuSel = menuRet;
    showMenuItem(menuItem, true);
  }
  else if (menu_st == M_EXITING) {
    lcd.clear();
    lcd.write(menu[menuSel]);

    while (digitalRead(rot_pb_pin) == 0);
  }

  menu_st = menu_state[menu_st][menu_ev];
}  
  
