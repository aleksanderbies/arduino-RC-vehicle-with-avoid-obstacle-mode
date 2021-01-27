#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 7 // radio PIN - activation
#define CSN_PIN 8 // radio PIN - in/out
const uint64_t pipe = 0xE8E8F0F0E1LL; //radio link address
#define trigPin 2 //distance sensor - speaker
#define echoPin 3 //distance sensor - microphone
#define PT 5 // right motors - forward
#define PP 6 // right motors - backward
#define LT 9 // left motors - forward
#define LP 10 // left motors - backward
RF24 radio(CE_PIN, CSN_PIN); // create radio link 
int joystick[6];  // joystick modes array
int  xAxis, yAxis; //var of joystick movement 

//change mode buttons
int buttonUp; //RC mode
int buttonDown; //avoid obstacles mode
int mode; //flag of modes

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  
  pinMode (PP,OUTPUT); 
  pinMode (PT,OUTPUT); 
  pinMode (LP,OUTPUT); 
  pinMode (LT,OUTPUT); 
  radio.begin(); 
  radio.openReadingPipe(1,pipe); 
  radio.startListening();
}

void loop() {
  if ( radio.available() )//condition of radio link, if it breaks vehicle will stop
  {
      radio.read( joystick, sizeof(joystick) ); //read from joystick
      xAxis = joystick[0];
      yAxis = joystick[1];
      buttonUp    = joystick[2];
      buttonDown  = joystick[4];
      // modes changing "UP" - RC, "DOWN" - avoid obstacles mode
      if (buttonUp == LOW){
        mode = 1;
      }
      else if(buttonDown == LOW ){
        mode = 2; 
      }
      if (mode == 1){ // RC
        if (yAxis < 5) {
          digitalWrite(PP, LOW);
          digitalWrite(PT, HIGH);
          digitalWrite(LT, HIGH);
          digitalWrite(LP, LOW);
          }
         else if (yAxis > 1020) {
          digitalWrite(PP, HIGH);
          digitalWrite(PT, LOW);
          digitalWrite(LT, LOW);
          digitalWrite(LP, HIGH);
          }
         else {
          digitalWrite(PP, LOW);
          digitalWrite(PT, LOW);
          digitalWrite(LT, LOW);
          digitalWrite(LP, LOW);
          }
         if (xAxis < 5) {
          digitalWrite(PP, HIGH);
          digitalWrite(PT, LOW);
          digitalWrite(LT, LOW);
          digitalWrite(LP, LOW);
          }
         if (xAxis > 1020) {
          digitalWrite(PP, LOW);
          digitalWrite(PT, LOW);
          digitalWrite(LT, LOW);
          digitalWrite(LP, HIGH);
          }
         }
    if (mode == 2){ //avoid obstacles mode
      int duration, distance;
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration/2) / 29.1;
      analogWrite (PP, 130); 
      analogWrite (LP, 130);
      delay(200);
      if (distance < 50){
        analogWrite(PP, 0);
        analogWrite(LP, 0);
        delay(200);
        analogWrite (PT, 130); 
        analogWrite (LT, 130);
        delay(150);
        analogWrite(LT, 0);
        delay(400);
        analogWrite (PT,0);
        delay(300);}
    }   
  }
}
