// Creating a distance measurement tool with the SRF04 module  

// Define pins for SRF04 (change if needed)
#define TRIG_PIN D5
#define ECHO_PIN D6

long duration;
float distance;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("SRF04 Distance Measurement Started");
}

void loop() {
  // Clear trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send 10µs pulse to trigger
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width
  duration = pulseIn(ECHO_PIN, HIGH);

  // Convert to distance (cm)
  distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
