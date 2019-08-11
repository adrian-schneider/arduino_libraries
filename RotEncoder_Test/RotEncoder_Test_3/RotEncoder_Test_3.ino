#include <SoftwareSerial.h>
#include <SerialLCD_BPP4X0.h>
#include <rotary.h>

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

int counter = 0;

void loop() {
  digitalWrite(13, LOW);
  
  unsigned char result = rot.process();
  
  if (digitalRead(rot_pb_pin) == 0) {
    counter = 0;
    lcd.cursorTo(1, 1);
    char buffer[10];
    itoa(counter, buffer, 10);
    lcd.write(buffer); lcd.write("     ");
  }
  
  if (result) {  
    digitalWrite(13, HIGH);
    counter += (result == DIR_CW ? 1 : -1);
    lcd.cursorTo(1, 1);
    char buffer[10];
    itoa(counter, buffer, 10);
    lcd.write(buffer); lcd.write("     ");
  }
}  
  
