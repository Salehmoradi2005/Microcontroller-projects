//ESP8266 web server for controlling an LED over Wi-Fi with smooth fading effects and direct brightness adjustment via URL commands.
//It creates a HTTP req server and listen to it via the local_ip address , You can send requests to run commands

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define GPIO D5

const char* ssid = "Your-SSID";
const char* password = "Your-Password";

IPAddress local_ip(192,168,1,199);
IPAddress gateway(192,168,1,2);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8,8,8,8);

bool lState = false;
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(GPIO, OUTPUT);
  digitalWrite(GPIO, LOW);
  analogWriteRange(255);

  if(!WiFi.config(local_ip , gateway , subnet , primaryDNS)){
    Serial.print("Failed to configure");
  }
  WiFi.begin(ssid , password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Wifi is connected and ready");

  server.on("/switch" , []() {
    if(!lState){
      digitalWrite(GPIO, LOW);
      for (int i = 0; i < 255; i++) {
        analogWrite(GPIO, i);
        delay(1);
      }
      digitalWrite(GPIO, HIGH);
      lState = !lState;
    }else {
      digitalWrite(GPIO, HIGH);
      for (int i = 255; i >= 0; i--) {
        analogWrite(GPIO, i);
        delay(1);
      }
      digitalWrite(GPIO, LOW);
      lState = !lState;
    }
    server.send(200 , "text/plain" , "LED Switched !");
  });

    server.onNotFound([]() {
    String path = server.uri();
    if (path.startsWith("/on/")) {
      String valStr = path.substring(4); 
      int value = valStr.toInt();        

      if (value >= 0 && value <= 255) {
        analogWrite(GPIO, value); 
        server.send(200, "text/plain", "LED set to " + String(value));
      } else {
        server.send(400, "text/plain", "Value out of range (0-255)");
      }
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });
  server.begin();
}

void loop() {
  server.handleClient();
}
