/*
Gas Detection Alert

Si du gaz est détecté, le ventilateur s'active, le buzzer sonne pour donner l'alerte et un message apparait sur l'écran LCD.
La LED RGB clignote en rouge.
*/

#include <Wire.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define RGB_PIN 26
#define NUM_LEDS 4
#define GAS_SENSOR_PIN 23
#define BUZZER_PIN 25
#define WINDOWS_PIN 5
#define FAN_POWER_PIN 19

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo ServoWindows;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

int pos = 0;

void setup() {
  ServoWindows.attach(WINDOWS_PIN);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_POWER_PIN, OUTPUT);
  
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Tout va bien");

  leds.begin();
  leds.show();
  ServoWindows.write(0);
}

void loop() {
  int gasDetected = digitalRead(GAS_SENSOR_PIN);

  if (gasDetected == 0) {
    digitalWrite(FAN_POWER_PIN, HIGH);
    ServoWindows.write(120);
    blinkingLeds();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Alerte !");
    lcd.setCursor(1, 1);
    lcd.print("Fuite de gaz !");
    for(int i = 0; i < 1; i++){
      tone(BUZZER_PIN, 589); 
      delay(500);
      tone(BUZZER_PIN, 700); 
      delay(500);
    }
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(FAN_POWER_PIN, LOW);
    
    leds.clear();
    leds.show();
    ServoWindows.write(0);

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Tout va bien");
  }

  delay(500);
}

void blinkingLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(255, 0, 0));
  }
  leds.show();
  delay(5);
  leds.clear();
  leds.show();
  delay(5);
}
