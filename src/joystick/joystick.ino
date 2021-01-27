#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define CE_PIN   9
#define CSN_PIN 10
#define xAxis 14     
#define yAxis 15     
const uint64_t pipe = 0xE8E8F0F0E1LL; // radio link address
RF24 radio(CE_PIN, CSN_PIN); 
int joystick[6]; 
int buttonUp    = 2;
int buttonDown  = 4;

void setup() {
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.stopListening();
  
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
  digitalWrite(buttonUp,LOW);
  digitalWrite(buttonDown,LOW);
}

void loop() {
  joystick[0] = analogRead(xAxis);
  joystick[1] = analogRead(yAxis);
  joystick[2] = digitalRead(buttonUp);
  joystick[4] = digitalRead(buttonDown);
  radio.write( joystick, sizeof(joystick) );
  delay(20);
}
