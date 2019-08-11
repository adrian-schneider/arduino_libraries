/*
SimpleMenu.h
TBD

To use, include:
  <SimpleMenu.h>
*/
#ifndef SimpleMenu_h
#define SimpleMenu_h

typedef struct {
  int from, to, incr;
  int width, dec; // configure ASCII representation, see ifptoa().
} SM_RangeDescr;

class SimpleMenu
{
  public:
    // Declare a menu structure as in the example below. String item and integer range
    // items are supported.
    // The top level menu is the menu of menus, i.e. the exit items and the names of
    // a set of name-value pairs. The sub menus contain the respective value ranges. 
    // The first n items of the top level menu are exit items, selecting one of them
    // exits the menu, e.g. to provide save and discard chnages exit options.
    //
    // Integer range declaration from -100 to 100 with step size 1:
    //   SM_RangeDescr menu_1_range = {-100, 100, 1, ...};
    //
    // Top level menu if menus. First n items are exit-items:
    //   const char *menu_0[] = {"EXIT_0", ..., "EXIT_n", "menu_1", ..., "menu_n", NULL};
    // Integer range menu: 
    //   const char *menu_1[] = {NULL, (char *)&menu_1_range};
    // String item menu:
    //   const char *menu_2[] = {"item_0", ..., "item_n", NULL};
    //   const char *menu_n[] = ...
    //
    // Array of all menus: 
    //   const char **menu_all[] = {menu_0, menu_1, menu_2, ..., menu_n};
    //
    // Array to hold the selected items of each menu. Initialize with current settings:
    //   int menuSel[] = {0, 0, 0, 0};
    SimpleMenu(const char ***menu_all, int *menuSel, int numExits);

    // The menuItemChr() function returns a string representation of the 'i'-th element
    // of 'menu'.
    const char* menuItemChr(const char *menu[], int i);

    // Call the process() function from the loop() function to process menu events.
    // Set the 'moved' parameter to -1 to dial in the previous menu item, to 1 to dial
    // the next menu item and to 0 to leave the menu unchanged. If 'button' is true,
    // this is interpreted as to select the currently dialled menu item.
    void process(int moved, int button);

    // Integer or Fixed Point TO Ascii
    // Convert an integer 'i' to an integer or fixed point ASCII representation of
    // 'width' with 'dec' places after the decimal point. Set 'dec' to -1 to get 
    // a plain integer representation. Set 'dec' to -16 to get a hexadecimal
    // integer representation.
    // A sufficently sized buffer 'buf' has to be provided. The buffer is returned.
    const char *ifptoa(int i, int width, int dec, char *buf);

  protected:
    // Implement your own class derived from SimpleMenu and overwrite the functions
    //   showMenu()
    //   showMenuItem()
    //   exitMenu()
    //   waitEnteringExiting() [optional]

    // The showMenu() function is called whenever entering a menu. This is if 'button' is
    // true when process() is called. Setup the basic menu display here.
    // When the menu is entered the first time at all or after exitMenu(), 'first' is
    // true, false otherwise. The parameter 'mindex' contains the menu index of the
    // menu currently shown.
    virtual void showMenu(int first, int mindex);

    // The showMenuItem() function is called whenever a new menu item is dialled in interactively
    // or once when the menu is entered. Update the display to show the dialled menu item.
    // A string representation of the item is provided in 'item'. If the 'item' is the
    // currently selected one, 'sel' is true.
    // The parameter 'iindex' contains the array index of 'item'.
    virtual void showMenuItem(const char *item, int sel, int iindex);

    // The exitMenu() function function is called once when exiting the menu. Cleanup the 
    // menu display and process the selected items here. 
    virtual void exitMenu();

    // The waitEnteringExiting() function is called whenever entering the menu (after showMenu()
    // and showMenuItem()) and once before exiting the menu. The parameter 'entering' is true
    // on enetring and false on exiting. This can be used to avoid unintended cycling in and
    // out when 'button' is held true during several calls of process().
    virtual void waitEnteringExiting(int entering);

  private:
    const char* selectMenu(const char *menu[], int moved, int *menuRet);
  
    static const unsigned char _menu_state[6][4];
 
    char _buf[9]; // sufficient for "-0.12345\0"
    const char ***_menu_all;
    int *_menuSel;
    int _numExits;
    int _menu_i;
    const char **_menu;
    int _menuRet;
    const char *_menuItem;
    unsigned char _menu_st;
    int _first;
};

#endif // SimpleMenu_h
