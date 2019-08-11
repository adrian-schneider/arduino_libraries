#include <SoftwareSerial.h>
#include <SerialLCD_BPP4X0.h>

SerialLCD_BPP4X0 lcd(12); // default speed

// Rotary encoder
const int pin_A = 2;
const int pin_B = 10;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev = 0;
volatile int cntr = 0;
volatile unsigned char moved = 0;

unsigned char hasMoved() {
  unsigned char _moved = moved;
  moved = 0;
  return _moved;
}

void readEncoder() {
  encoder_A = digitalRead(pin_A) && digitalRead(pin_A) && digitalRead(pin_A);
  encoder_B = digitalRead(pin_B) && digitalRead(pin_B) && digitalRead(pin_B);  
  if ((!encoder_A) && (encoder_A_prev)) {
    // A has gone from high to low 
    moved = 1;
    if (encoder_B) {
      // B is high so clockwise
      cntr++;
    }   
    else {
      // B is low so counter-clockwise
      cntr--;      
    }   
  }   
  encoder_A_prev = encoder_A;
}

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);   
  digitalWrite(13, LOW);
  
  pinMode(pin_A, INPUT);
  digitalWrite(pin_A, HIGH); // enable pull-up
  pinMode(pin_B, INPUT);
  digitalWrite(pin_B, HIGH); // enable pull-up
  
  attachInterrupt(0, readEncoder, CHANGE); // int-0: pin 2, int-1: pin 3
  
  lcd.clear();
  lcd.write("hello");
}

void loop() {
  digitalWrite(13, LOW);
  if (hasMoved()) {  
  digitalWrite(13, HIGH);
    lcd.cursorTo(1, 1);
    char buffer[10];
    itoa(cntr, buffer, 10);
    lcd.write(buffer); lcd.write("     ");
  }
}  
  
