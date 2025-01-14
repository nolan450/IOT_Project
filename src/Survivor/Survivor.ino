#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
 
#define PIR_MOTION_SENSOR_PIN 14
#define RGB_PIN 26
#define NUM_LEDS 4
#define YELLOW_LED_PIN 12
#define LEFT_BUTTON_PIN 16
#define BUZZER_PIN 25
 
int pirStat = 0;
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  pinMode(YELLOW_LED_PIN, OUTPUT);      // Yellow LED pin
  pinMode(LEFT_BUTTON_PIN, INPUT);      // Button pin
  pinMode(BUZZER_PIN, OUTPUT);          // Buzzer pin
  pinMode(PIR_MOTION_SENSOR_PIN , INPUT); // PIR motion sensor pin
 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Bonjour !");
}
 
void loop() {
  pirStat = digitalRead(PIR_MOTION_SENSOR_PIN); // Check PIR sensor
 
  if (pirStat == HIGH) {
    String message = "Sonnez avant d'entrer !   "; // Message to show
    digitalWrite(YELLOW_LED_PIN, HIGH);  // Turn on yellow LED
    for (int i = 0; i < message.length(); i++) {
      if (digitalRead(PIR_MOTION_SENSOR_PIN) == LOW) {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Bonjour !");
        break;
      }
 
      if (digitalRead(LEFT_BUTTON_PIN) == LOW) { // Button pressed
        blinkingLeds();
        tone(BUZZER_PIN, 1000); // Activate buzzer at 1000Hz
      } else {
        leds.clear();
        leds.show();
        noTone(BUZZER_PIN); // Stop buzzer when button is not pressed
      }
 
      lcd.setCursor(0, 1);
      lcd.print(message.substring(i) + message.substring(0, i));
      delay(300); // Delay for scrolling effect
    }
  } else {
    if (digitalRead(LEFT_BUTTON_PIN) == LOW) { // Button pressed
      blinkingLeds();
      tone(BUZZER_PIN, 1000); // Activate buzzer at 1000Hz
    } else {
      leds.clear();
      leds.show();
      noTone(BUZZER_PIN); // Stop buzzer when button is not pressed
    }
 
    digitalWrite(YELLOW_LED_PIN, LOW);  // Turn off yellow LED
    lcd.setCursor(0, 1);
    lcd.print("                ");  // Clear second line of the LCD
  }
}
 
void blinkingLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(255, 0, 0)); // Set NeoPixel to red
  }
  leds.show();
  delay(5); // Short delay for blinking
  leds.clear();
  leds.show();
  delay(5); // Short delay for blinking
}
 
