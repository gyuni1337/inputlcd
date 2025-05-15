#include <WiFiS3.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char ssid[] = "681";
char pass[] = "loshiq123";

WiFiServer server(80);

void connectToWiFi();

void connectToWiFi() {
  int attempts = 0;
  int maxAttempts = 30; 

  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, pass);
    delay(1000);  
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected!");
    lcd.clear();
    lcd.print("IP:");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());  
    server.begin();  
  } else {
    Serial.println("din nät funkar inte.");
    lcd.clear();
    lcd.print("Failed to connect.");
  }
}

String decodeUrl(String url) {
  url.replace("%20", " ");  // Replace %20 with space

  url.replace("%21", "!");  // Decode "!" from "%21"
  url.replace("%22", "\""); // Decode '"' from "%22"
  url.replace("%23", "#");  // Decode "#" from "%23"
  url.replace("%24", "$");  // Decode "$" from "%24"
  url.replace("%25", "%");  // Decode "%" from "%25"

  return url;
}

void setup() {
  lcd.begin(16, 2);
  lcd.print("Connecting...");

  Serial.begin(9600);
  
  connectToWiFi();  
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi lost, reconnecting...");
    connectToWiFi();  
  }

  // Process incoming client requests
  WiFiClient client = server.available();

  if (client) {
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (c == '\n') {
          if (request.indexOf("GET /?text=") >= 0) {
            int start = request.indexOf("text=") + 5;
            int end = request.indexOf(" ", start);
            String text = request.substring(start, end);
            text.replace("+", " ");  

            text = decodeUrl(text);  // Decode %20 and other special characters
            text.trim();  // Remove leading/trailing spaces
            text = text.substring(0, 16);  // Limit text to 16 characters for LCD display

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(text);  
          }


          client.println("HTTP/1.1 200 OK");
          client.println("Access-Control-Allow-Origin: *");  // Allow requests from any origin jävla CORS 
          client.println("Content-Type: text/plain");
          client.println("Connection: close");
          client.println();
          client.println("OK");
          break;
        }
      }
    }
    delay(1);
    client.stop();
  }
}
