/*
Wifi
Met en place une connexion wifi et permet de gérer l'état de la LED de la maison via un bouton sur une page web.
*/

#include <Wire.h>                
#include <LiquidCrystal_I2C.h>   
#include <WiFi.h>                
#include <WebServer.h>           



String header;
String output12State = "off";
unsigned long currentTime = millis();
unsigned long previousTime = 0; 


const long timeoutTime = 2000;



const int output12 = 12;

const char* ssid = "Florian";          
const char* password = "Nolan.Ammar15";  


LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiServer server(80); 

void setup() {

  pinMode(output12, OUTPUT);
  

  digitalWrite(output12, LOW);
 
  lcd.init();
  lcd.backlight();  
  lcd.setCursor(0, 0);
  lcd.print("Connexion au");
  lcd.setCursor(0, 1);
  lcd.print("Wi-Fi...");

  // Démarrer la connexion Wi-Fi
  WiFi.begin(ssid, password); 

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tentative de");
  lcd.setCursor(0, 1);
  lcd.print("connexion...");

  
  int attempts = 0;  
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { 
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Essai: ");
    lcd.print(attempts + 1);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecte!");
    Serial.println(WiFi.localIP());

    
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());

   

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
  
  WiFiClient client = server.available();

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                   
         
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            
            if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              output12State = "on";
              digitalWrite(output12, HIGH);
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("GPIO 12 off");
              output12State = "off";
              digitalWrite(output12, LOW);
            } 
            
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
           
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            
            client.println("<p>LED Jaune - State " + output12State + "</p>");
                   
            if (output12State=="off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println();
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";

    // Fermer la connexion
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

  }
  




  
  static unsigned long lastUpdate = 0;  
  unsigned long currentMillis = millis(); 

  if (currentMillis - lastUpdate >= 5000) {
    lastUpdate = currentMillis; 

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecte!");
    lcd.setCursor(0, 1);
    Serial.println(WiFi.localIP());
    lcd.print(WiFi.localIP()); 
  }
}