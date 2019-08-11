/*
SimpleMenu.cpp
TBD

To use, include:
  <SimpleMenu.h>

Define SIMPLEMENU_DEBUG and initialize SoftwareSerial to enable default
implementations of virtual functions.
*/

#include <Arduino.h>
#ifdef SIMPLEMENU_DEBUG
#include <SoftwareSerial.h>
#endif
#include "SimpleMenu.h"

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

const unsigned char SimpleMenu::_menu_state[6][4] = {
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

SimpleMenu::SimpleMenu(const char ***menu_all, int *menuSel, int numExits) :
  _menu_all(menu_all),
  _menuSel(menuSel),
  _numExits(numExits),
  _menu_i(0),
  _menu(menu_all[_menu_i]),
  _menuRet(0),
  _menuItem(NULL),
  _menu_st(M_OUTSIDE),
  _first(true)
{
}

const char *SimpleMenu::ifptoa(int i, int width, int dec, char *buf) {
  buf[--width] = 0;
  memset(buf, ' ', --width);

  int m = (dec == -16) ? 16 : 10; // -16 also suppresses the decimal point.

  int sign = false;
  if (i < 0) {
    sign = true;
    i = -i; 
  }

  int d = 0;
  do {
    if (dec == d++) {
      buf[width--] = '.';
    } else {
      int j = i % m;
      buf[width--] = (j < 10 ? '0' : '\067') + j; // '\067' == 'A' - 10
      i /= m;
    }   
  } while (i || (d - 2) < dec);

  if (sign) buf[width] = '-';

  return buf;
}

void SimpleMenu::process(int moved, int button) {
  // Rotation and button pushed considered mutually exclusive.  
  unsigned char menu_ev = moved ? ME_ROTMVD :
    button ? ME_BUTTON : ME_NONE;

  if (_menu_st == M_ENTERING) {
    _menu = _menu_all[_menu_i];
    _menuRet = _menuSel[_menu_i];
    showMenu(_first, _menu_i);
    _first = false;
    showMenuItem(menuItemChr(_menu, _menuRet), true, _menuRet); 
    waitEnteringExiting(true);
  }
  else if (_menu_st == M_INSIDE) {
    _menuItem = selectMenu(_menu, moved, &_menuRet);
  }
  else if (_menu_st == M_MNUCHG) {
    // Show current and mark selected item.
    showMenuItem(_menuItem, _menuRet == _menuSel[_menu_i], _menuRet);
  }
  else if (_menu_st == M_SELCHG) {
    _menuSel[_menu_i] = _menuRet;
    showMenuItem(_menuItem, true, _menuRet);
    // Briefly show selected item.     
    delay(1000);
    // If menu is top level, next menu is the selected menu.
    // Otherwise, next menu is top level.
    if (_menu_i == 0 && _menuRet < _numExits) {
      menu_ev = ME_EXIT;
    }
    else {
      _menu_i = (_menu_i == 0 ? _menuRet - _numExits + 1 : 0);
    }
  }
  else if (_menu_st == M_EXITING) {
    _first = true;
    exitMenu();
    // Wait until select button is released to avoid automaitc cylcing.
    waitEnteringExiting(false);
  }
  _menu_st = _menu_state[_menu_st][menu_ev];
}

const char* SimpleMenu::selectMenu(const char *menu[], int moved, int *menuRet) {
  if (menu[0] == NULL) {
    int from  = ((SM_RangeDescr *)(menu[1]))->from;
    int to    = ((SM_RangeDescr *)(menu[1]))->to;
    int incr  = ((SM_RangeDescr *)(menu[1]))->incr;
    (*menuRet) += moved * incr;
    if (*menuRet < from) *menuRet = from;
    if (*menuRet > to)   *menuRet = to;
  }
  else {
    (*menuRet) += moved;
    if (*menuRet < 0) *menuRet = 0;
    if (menu[*menuRet] == NULL) (*menuRet)--;
  }
  return menuItemChr(menu, *menuRet);
}

const char *SimpleMenu::menuItemChr(const char *menu[], int i) {
  if (menu[0] == NULL) {
    int width = ((SM_RangeDescr *)(menu[1]))->width;
    int dec   = ((SM_RangeDescr *)(menu[1]))->dec;
    return ifptoa(i, width, dec, _buf);
  } else {
    return menu[i];
  }
}

void SimpleMenu::showMenu(int first, int mindex) {
  #ifdef SIMPLEMENU_DEBUG
  Serial.println("Menu");
  #endif 
}

void SimpleMenu::showMenuItem(const char *item, int sel, int iindex) {
  #ifdef SIMPLEMENU_DEBUG
  Serial.write(item); 
  if (sel) Serial.write('*');
  Serial.println();
  #endif 
}

void SimpleMenu::exitMenu() {
  #ifdef SIMPLEMENU_DEBUG
  for (int i = 0; i < 4; i++) {
    Serial.write(_menu_all[0][i]); Serial.write(": ");
    _menu = _menu_all[i];
    Serial.println(menuItemChr(_menu, _menuSel[i]));
  }
  #endif 
}

void SimpleMenu::waitEnteringExiting(int entering) {
  #ifdef SIMPLEMENU_DEBUG
  delay(1000);
  #endif 
}

