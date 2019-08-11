#include <SoftwareSerial.h>
#include <SerialLCD_TRM425.h>

SerialLCD_TRM425 lcd(2, 3, 9600);

void setup() {
  lcd.clear();
  lcd.write("hello");
  delay(2000);
  lcd.clear();
}

void loop() {
  char str[10];
  lcd.readStr(str, sizeof(str) - 1, '*', "#");
  lcd.write(TRM425::CR); lcd.write(TRM425::LF); lcd.write(str);
  delay(2000);
  lcd.clear();
}
