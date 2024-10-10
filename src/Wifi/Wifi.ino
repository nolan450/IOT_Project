#include <Wire.h>                // Inclure la bibliothèque pour l'I2C
#include <LiquidCrystal_I2C.h>   // Inclure la bibliothèque pour écran LCD I2C
#include <WiFi.h>                // Inclure la bibliothèque pour gérer le Wi-Fi
#include <WebServer.h>           // Inclure la bibliothèque pour le serveur Web

// Remplacez par les informations de votre réseau
const char* ssid = "Florian";          // Remplacez par votre SSID
const char* password = "Nolan.Ammar15";  // Remplacez par votre mot de passe

// Créez une instance de l'écran LCD I2C (adresse par défaut 0x27 et écran 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);
WebServer server(80); // Créer un serveur sur le port 80

void setup() {
  // Initialiser l'écran LCD
  lcd.init();
  lcd.backlight();  // Allumer le rétroéclairage de l'écran

  // Afficher un message pendant la connexion Wi-Fi
  lcd.setCursor(0, 0);
  lcd.print("Connexion au");
  lcd.setCursor(0, 1);
  lcd.print("Wi-Fi...");

  // Démarrer la connexion Wi-Fi
  WiFi.begin(ssid, password); // Correctement initialiser la connexion Wi-Fi

  // Attendre la connexion
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tentative de");
  lcd.setCursor(0, 1);
  lcd.print("connexion...");

  // Boucle jusqu'à ce que la connexion soit établie
  int attempts = 0;  // Compteur de tentatives
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Limiter à 20 tentatives
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Essai: ");
    lcd.print(attempts + 1);
    attempts++;
  }

  // Vérifier si connecté
  if (WiFi.status() == WL_CONNECTED) {
    // Une fois connecté, afficher l'adresse IP sur le LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecte!");
    Serial.println(WiFi.localIP());

    // Obtenir et afficher l'adresse IP sur la 2e ligne du LCD
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());

    // Configurer le serveur pour répondre à la racine
    server.on("/", []() {
      server.send(200, "text/plain", "Hello from ESP32!"); // Réponse du serveur
    });

    // Démarrer le serveur
    server.begin();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Echec Connexion");
    lcd.setCursor(0, 1);
    lcd.print("Rev. SSID/Mdp");
  }
}

void loop() {
  // Gérer les requêtes du serveur
  server.handleClient();

  // Ici, nous allons mettre à jour l'affichage toutes les 5 secondes
  static unsigned long lastUpdate = 0;  // Pour garder la trace du dernier temps d'actualisation
  unsigned long currentMillis = millis(); // Temps actuel

  // Vérifiez si 5 secondes se sont écoulées
  if (currentMillis - lastUpdate >= 5000) {
    lastUpdate = currentMillis; // Mettez à jour le dernier temps d'actualisation

    // Afficher à nouveau l'adresse IP
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecte!");
    lcd.setCursor(0, 1);
    Serial.println(WiFi.localIP());
    lcd.print(WiFi.localIP()); // Réaffiche l'adresse IP
  }
}
