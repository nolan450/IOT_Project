#include <ESP32Servo.h>

int yellowLedPin = 12;
int humiditySensorPin = 17;
int steamSensorPin = 34;
int fanDirectionPin = 18;
int fanPowerPin = 19;
int pirMotionSensorPin = 14;
int leftButtonPin = 16;
int rightButtonPin = 27;
int rgbPin = 26;
int gasSensor = 23;
int buzzer = 25;
int windows = 5;
int door = 13;

int servoPin = windows;

#include <Wire.h>  // Bibliothèque pour la communication I2C
#include <MFRC522_I2C.h>  // Bibliothèque pour le RFID via I2C

#define RST_PIN 4  // RST sur GPIO 4
#define SDA_PIN 0x28  // Adresse I2C du module RFID (par défaut c'est souvent 0x28, vérifie la documentation de ton module)

MFRC522_I2C rfid(SDA_PIN, RST_PIN);  // Utiliser MFRC522_I2C pour la communication I2C

void setup() {
  Serial.begin(9600);  // Initialiser la communication série
  Wire.begin();  // Initialiser l'I2C
  
  rfid.PCD_Init();  // Initialiser le module RFID
  Serial.println("Module RFID initialisé.");
  Serial.println("Approche une carte RFID du lecteur...");
}

void loop() {
  // Vérifier si une carte est présente
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Lire la carte
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Afficher l'UID de la carte sur le moniteur série
  Serial.print("UID de la carte: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  // Halt PICC (pour arrêter la communication avec la carte)
  rfid.PICC_HaltA();
}
