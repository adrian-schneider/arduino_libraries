#include <SoftwareSerial.h>
#include <SerialLCD_BPP4X0.h>

// RotEncoder_Test_1

SerialLCD_BPP4X0 lcd(12); // default speed

const int pin_A = 11;
const int pin_B = 10;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);   
  
  pinMode(pin_A, INPUT);
  digitalWrite(pin_A, HIGH); // enable pull-up
  pinMode(pin_B, INPUT);
  digitalWrite(pin_B, HIGH); // enable pull-up
  
  lcd.clear();
  lcd.write("hello");
}

int cntr=0;

void loop() {
  encoder_A = digitalRead(pin_A);
  encoder_B = digitalRead(pin_B);   
  if ((!encoder_A) && (encoder_A_prev)) {
    // A has gone from high to low 
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
  
  lcd.cursorTo(1, 1);
  char buffer[10];
  itoa(cntr, buffer, 10);
  lcd.write(buffer); lcd.write("     ");
}  
  
