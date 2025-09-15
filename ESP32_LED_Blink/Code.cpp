/*
  ESP32 Blink Example
  -------------------
  Blinks the onboard LED (GPIO 2) every 1 second.
*/

#define LED_PIN 2   // Onboard LED for most ESP32 boards

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // LED ON
  delay(1000);                  // Wait 1 second
  digitalWrite(LED_PIN, LOW);   // LED OFF
  delay(1000);                  // Wait 1 second
}
