#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// Sensor settings
#define GAS_SENSOR_PIN 23
#define HUMIDITY_TEMPERATURE_SENSOR_PIN 17
#define PIR_MOTION_SENSOR_PIN 14
#define DHT_TYPE 11

DHT dht(HUMIDITY_TEMPERATURE_SENSOR_PIN, DHT_TYPE);

// WiFi settings
const char *ssid = "Florian";             // Replace with your WiFi name
const char *password = "Nolan.Ammar15";   // Replace with your WiFi password

// MQTT Broker settings
const char* mqtt_broker = "f19f0caea42a4817b2b6cd269fd908c0.s1.eu.hivemq.cloud";  // EMQX broker endpoint
const char* mqtt_topic_gas_sensor = "gas";
const char* mqtt_topic_humidity_temp_sensor = "humidity_temperature";
const char* mqtt_topic_pir_motion_sensor = "pir_motion";
const char* mqtt_username = "Teste";  // MQTT username for authentication
const char* mqtt_password = "Test12345";  // MQTT password for authentication
const int mqtt_port = 8883;  // MQTT port (TCP)

float temp = NAN;
float hum = NAN;

WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp32-client-578"; //+ String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic_gas_sensor);
            mqtt_client.subscribe(mqtt_topic_humidity_temp_sensor);
            mqtt_client.subscribe(mqtt_topic_pir_motion_sensor);
            // Publish message upon successful connection
            // mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP8266 ^^");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            Serial.println(mqtt_broker);
            Serial.println(mqtt_username);
            Serial.println(mqtt_password);
            Serial.println(mqtt_port);
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void publishSensorInfos() {
    mqtt_client.publish(mqtt_topic_gas_sensor, String(digitalRead(GAS_SENSOR_PIN)).c_str());
    mqtt_client.publish(mqtt_topic_humidity_temp_sensor, String("Humidity :" + String(hum) + ", Temperature :" + String(temp)).c_str());
    mqtt_client.publish(mqtt_topic_pir_motion_sensor, String(digitalRead(PIR_MOTION_SENSOR_PIN)).c_str());
}

void setup() {
    pinMode(HUMIDITY_TEMPERATURE_SENSOR_PIN, INPUT);
    pinMode(GAS_SENSOR_PIN, INPUT);
    pinMode(PIR_MOTION_SENSOR_PIN, INPUT);

    dht.begin();
    Serial.begin(115200);
    connectToWiFi();
    espClient.setInsecure();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
}

void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }

    hum = dht.readHumidity();
    temp = dht.readTemperature();
    mqtt_client.loop();
    publishSensorInfos();
    delay(10000);
}