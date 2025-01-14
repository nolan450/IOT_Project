#include <Wire.h>
#include <Adafruit_Si7021.h>
#include <DHT.h>
 
// Configuration du capteur DHT
#define DHTPIN 17      // Broche numérique connectée au capteur DHT
#define DHTTYPE DHT11  // Changez en DHT22 si vous utilisez ce modèle
 
DHT dht(DHTPIN, DHTTYPE);  // Initialisation du capteur DHT
 
float temp = NAN;  // Variable pour la température
float hum = NAN;   // Variable pour l'humidité
 
void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT Sensor Test!"));
 
  dht.begin();  // Initialisation du capteur DHT
}
 
void loop() {
  // Lecture de la température et de l'humidité
  hum = dht.readHumidity();
  temp = dht.readTemperature();
 
  // Vérification des erreurs
  if (isnan(temp) || isnan(hum)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    // Affichage des valeurs lues
    Serial.print(F("Temperature: "));
    Serial.print(temp);
    Serial.println(F(" °C"));
 
    Serial.print(F("Humidity: "));
    Serial.print(hum);
    Serial.println(F(" %"));
  }
 
  delay(2000);  // Pause de 2 secondes entre les lectures
}
