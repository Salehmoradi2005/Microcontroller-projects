/*
  ESP32 Web Server – Handle HTTP Requests
  ---------------------------------------
  Features:
  - Connects ESP32 to Wi-Fi
  - Creates a web server on port 80
  - Listens for commands via HTTP requests
  - Example endpoints:
      http://<ESP_IP>/on   -> turns LED on
      http://<ESP_IP>/off  -> turns LED off
      http://<ESP_IP>/status -> shows LED state

  Hardware:
  - ESP32 development board
  - LED (or use onboard LED on GPIO2)
*/

#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "Your-SSID";
const char* password = "Your-Password";

// Create web server on port 80
WebServer server(80);

// Define pin for LED
#define LED_PIN 2
bool ledState = false;

void handleOn() {
  digitalWrite(LED_PIN, HIGH);
  ledState = true;
  server.send(200, "text/plain", "LED is ON");
}

void handleOff() {
  digitalWrite(LED_PIN, LOW);
  ledState = false;
  server.send(200, "text/plain", "LED is OFF");
}

void handleStatus() {
  String message = ledState ? "LED is ON" : "LED is OFF";
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/status", handleStatus);

  server.onNotFound(handleNotFound);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
