/*
Gas Detection Alert

Si du gaz est détecté, le ventilateur s'active, le buzzer sonne pour donner l'alerte et un message apparait sur l'écran LCD
*/

int rgbPin = 26;
int gasSensor = 23;
int buzzer = 25;
int windows = 5;
int fanDirectionPin = 18;
int fanPowerPin = 19;

#include <Wire.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo ServoWindows;
int pos = 0;

void setup() {
  ServoWindows.attach(windows);
  pinMode(gasSensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(rgbPin, OUTPUT);
  pinMode(fanPowerPin, OUTPUT);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Tout va bien");

}

void loop() {
  Serial.println(digitalRead(gasSensor));
  if(digitalRead(gasSensor) == 0){
    tone(buzzer, 589);
    digitalWrite(fanPowerPin, HIGH);
    lcd.clear();
    lcd.print("Alerte !");
    lcd.setCursor(3, 1);
    lcd.print("Fuite de gaz !");
    delay(300);
  }else {
    noTone(buzzer);
    digitalWrite(fanPowerPin, LOW);
  }

}
