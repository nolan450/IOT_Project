#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// UUID du service et de la caractéristique BLE
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-abcdef987654"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Périphérique connecté");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Périphérique déconnecté");
    }
};

void setup() {
  Serial.begin(115200);

  // Initialiser le périphérique BLE
  BLEDevice::init("ESP32_BLE");

  // Créer un serveur BLE
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Créer un service BLE
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Créer une caractéristique BLE
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  // Démarrer le service
  pService->start();

  // Démarrer la publicité BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  Serial.println("Serveur BLE démarré et en attente de connexion...");
}

void loop() {
  if (deviceConnected) {
    // Créer une chaîne de caractères personnalisée
    String message = "Valeur actuelle: " + String(value);
    pCharacteristic->setValue(message.c_str());  // Utiliser c_str() pour convertir en char*
    pCharacteristic->notify();  // Envoyer la notification avec le message
    Serial.println("Message envoyé: " + message);
    
    value++;  // Incrémenter la valeur pour la prochaine notification
    delay(1000);  // Envoyer une notification chaque seconde
  }

  // Gérer les connexions et déconnexions
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising();
    Serial.println("Redémarrage de la publicité BLE");
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
}
