#include <rotary.h>
#include <SoftwareSerial.h>
#include <SerialLCD_BPP4X0.h>

char buf[7];

const int rot_pb_pin = 9;

SerialLCD_BPP4X0 lcd(2, 12); // default speed

Rotary rot = Rotary(10, 11);

#define M_OUTSIDE 0x00
#define M_ENTERING 0x01
#define M_INSIDE 0x02
#define M_EXITING 0x03
#define M_MNUCHG 0x04
#define M_SELCHG 0x05

#define ME_NONE 0x00
#define ME_BUTTON 0x01
#define ME_ROTMVD 0x02
#define ME_EXIT 0x03

const unsigned char menu_state[6][4] = {
  // M_OUTSIDE
  {M_OUTSIDE, M_ENTERING, M_OUTSIDE, M_OUTSIDE},
  // M_ENTERING
  {M_INSIDE, M_INSIDE, M_INSIDE, M_INSIDE},
  // M_INSIDE
  {M_INSIDE, M_SELCHG, M_MNUCHG, M_INSIDE},
  // M_EXITING
  {M_OUTSIDE, M_OUTSIDE, M_OUTSIDE, M_OUTSIDE},
  // M_MNUCHG
  {M_INSIDE, M_INSIDE, M_INSIDE, M_INSIDE},
  // M_SELCHG
  {M_ENTERING, M_ENTERING, M_ENTERING, M_EXITING}
};

typedef struct {
  int from, to, incr;
} rangeDescr;

rangeDescr menu_1_range = {-100, 100, 1};

const char *menu_0[] = {"EXIT", "number", "color", "direction", NULL}; 
//const char *menu_1[] = {"one", "two", "three", "four", "five", "six", NULL};
const char *menu_1[] = {NULL, (char *)&menu_1_range};
const char *menu_2[] = {"red", "green", "blue", "yellow", "cyan", "magenta", "white", "black", "grey", NULL};
const char *menu_3[] = {"north", "east", "south", "west..", "UP", "RIGHT", "DOWN", "LEFT", NULL};
const char **menu_all[] = {menu_0, menu_1, menu_2, menu_3};

int menuSel[] = {0, 0, 0, 3};

char menu_mark = '*';
int menu_i = 0;
const char **menu = menu_all[menu_i];
int menuRet = 0;
const char *menuItem = NULL;
unsigned char menu_st = M_OUTSIDE;

const char* selectMenu(const char *menu[], int moved, int *menuRet) {
  if (menu[0] == NULL) {
    int from = ((rangeDescr *)(menu[1]))->from;
    int to   = ((rangeDescr *)(menu[1]))->to;
    int incr = ((rangeDescr *)(menu[1]))->incr;
    (*menuRet) += moved * incr;
    if (*menuRet < from) *menuRet = from;
    if (*menuRet > to)   *menuRet = to;
    return itoa(*menuRet, buf, 10);
  }
  else {
    (*menuRet) += moved;
    if (*menuRet < 0) *menuRet = 0;
    if (menu[*menuRet] == NULL) (*menuRet)--;
    return menu[*menuRet];
  }
}

const char *menuItemChr(const char *menu[], int i) {
  return (menu[0] == NULL) ? itoa(i, buf, 10) : menu[i];
}

void showMenu() {
  lcd.clear();
  lcd.write("Config...");
}

void showMenuItem(const char *item, char mark) {
  lcd.cursorTo(1, 1);
  lcd.write(item);
  if (mark) lcd.write(mark);
  lcd.write("    "); 
}

void showSettings() {
  lcd.clear();
  for (int i = 0; i < 4; i++) {
    lcd.cursorTo(0, i);
    lcd.write(menu_0[i]); lcd.write(": ");
    menu = menu_all[i];
    lcd.write(menuItemChr(menu, menuSel[i]));
  }
}

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
}

void loop() {
  // Rotation and button pushed considered mutually exclusive.  
  unsigned char mvd = rot.process();
  unsigned char menu_ev = mvd ? ME_ROTMVD :
    (digitalRead(rot_pb_pin) == 0) ? ME_BUTTON : ME_NONE;

  if (menu_st == M_ENTERING) {
    menu = menu_all[menu_i];
    menuRet = menuSel[menu_i];
    showMenu();
    showMenuItem(menuItemChr(menu, menuRet), menu_mark); 
    while (digitalRead(rot_pb_pin) == 0);
  }
  else if (menu_st == M_INSIDE) {
    int moved = (mvd != 0 ? (mvd == DIR_CW ? 1 : -1) : 0);
    menuItem = selectMenu(menu, moved, &menuRet);
  }
  else if (menu_st == M_MNUCHG) {
    // Show current and mark selected item.
    char mark = (menuRet == menuSel[menu_i]) ? menu_mark : 0;
    showMenuItem(menuItem, mark);
  }
  else if (menu_st == M_SELCHG) {
    if (menu_i == 0 && menuRet == 0) {
      menu_ev = ME_EXIT;
    }
    else {
      menuSel[menu_i] = menuRet;
      showMenuItem(menuItem, menu_mark);
      // Briefly show selected item.     
      delay(1000);
      // If menu is top level, next menu is the selected menu.
      // Otherwise, next menu is top level.
      menu_i = (menu_i == 0 ? menuRet : 0);
    }
  }
  else if (menu_st == M_EXITING) {
    showSettings();
    delay(250);
    // Wait until select button is released to avoid automaitc cylcing.
    while (digitalRead(rot_pb_pin) == 0);
  }
  menu_st = menu_state[menu_st][menu_ev];
}  

