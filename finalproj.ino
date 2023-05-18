/*
FILE:         finalproj.ino
AUTHOR:       Spencer Garcia
DESCRIPTION:  final project for swamp cooler
DATE:         5/9/2023
NOTES:

*/
#include <LiquidCrystal.h>
#include "Stepper.h"
#include <EduIntro.h>
#include <Wire.h>
#include <RTClib.h>

DHT11 dht11(38);
int C, H;
float F;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define STEPS 32
Stepper stepper(STEPS, 46, 50, 48, 52);
int stepsreq;
#define left 51
#define right 53

#define DHT11 38

#define fan 6
#define Direction 7
#define button 13
#define green 22
#define yellow 24
#define blue 26
byte toggleState = LOW;
int input = false;
byte lastButton = LOW;

DS1307 rtc;
char buf[20];

void setup() 
{
Serial.begin(9600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin();
#endif
  rtc.begin();
if(!rtc.isrunning())
{
  Serial.println("RTC is not running");
  rtc.adjust(DateTime(__DATE__, __TIME__));
}
lcd.begin(16,2);
pinMode(fan, OUTPUT);
pinMode(Direction, OUTPUT);
pinMode(button, INPUT_PULLUP);
pinMode(green, OUTPUT);
pinMode(yellow, OUTPUT);
pinMode(blue, OUTPUT);
pinMode(left, INPUT_PULLUP);
pinMode(right, INPUT_PULLUP);

stepper.setSpeed(200);
}


void loop() {
  byte buttonstate = digitalRead(button);

  byte buttonleft = digitalRead(left);
  byte buttonright = digitalRead(right);

  DateTime now = rtc.now();

  if(buttonleft == LOW)
  {
    stepper.step(-100);
    delay(100);
  }
  if(buttonright == LOW)
  {
    stepper.step(100);
    delay(100);
  }

  if(buttonstate == LOW)
  {
    lastButton = !lastButton;
    toggleState = lastButton;
    //digitalWrite(green, lastButton);
  }
  while(digitalRead(button) == LOW)
    delay(70);
    

  if(toggleState == HIGH)
  {
  
    digitalWrite(yellow, LOW);
    dht11.update();
    F = dht11.readFahrenheit();
    H = dht11.readHumidity();
    //delay(10000);
    //RUNNING
    if(F > 78)
    {
      Serial.println("Running state active at ");
      Serial.println(now.tostr(buf));
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(fan,HIGH);
      digitalWrite(Direction, HIGH);
    }
    else//IDLE
    {
      Serial.println("Idle state active at ");
      Serial.println(now.tostr(buf));
      digitalWrite(fan, LOW);
      digitalWrite(green, HIGH);
      digitalWrite(blue, LOW);
    }

  lcd.setCursor(0, 0);
    lcd.print("Temp = ");
    lcd.print(F);   
    lcd.print(" F");
  lcd.setCursor(0, 1);
    lcd.print("Humidity = ");
    lcd.print(H);
    lcd.print("%");
  
    delay(500);            
    lcd.clear();
  }
  else
  {
    lcd.print("Off, plz turn on");
    Serial.println("Disabled at ");
    Serial.println(now.tostr(buf));
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(fan, LOW);
    delay(700);
    lcd.clear();
  }
    
}
