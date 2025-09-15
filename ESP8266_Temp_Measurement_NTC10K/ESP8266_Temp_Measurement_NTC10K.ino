/*
  ESP8266 + NTC 10k Thermistor Temperature Measurement
  ----------------------------------------------------
  Hardware:
  - ESP8266 board (e.g., NodeMCU, Wemos D1 mini)
  - NTC 10k thermistor
  - 10k resistor (for voltage divider)
  
  Wiring:
  - Connect one leg of the thermistor to 3.3V
  - Connect the other leg of the thermistor to A0 (analog input) AND to a 10k resistor
  - Connect the other leg of the 10k resistor to GND
  => This creates a voltage divider: Thermistor + Fixed resistor

  Formula:
  - Read analog voltage (0–1023 on ESP8266 ADC)
  - Calculate thermistor resistance
  - Use Beta parameter equation to calculate temperature

  Notes:
  - Default Beta (B value) is around 3950 (check your thermistor datasheet)
  - R0 = 10kΩ at T0 = 25°C
  - ADC of ESP8266 is 10-bit (0–1023), reference voltage = 3.3V
*/

#define SERIES_RESISTOR 10000    // 10k fixed resistor
#define NOMINAL_RESISTANCE 10000 // 10k thermistor at 25°C
#define NOMINAL_TEMPERATURE 25   // °C
#define B_COEFFICIENT 3950       // Beta parameter (typical 3950)
#define ADC_MAX 1023.0           // 10-bit ADC
#define VCC 3.3                  // ESP8266 ADC reference voltage

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("NTC 10k Thermistor Temperature Measurement");
}

void loop() {
  // 1. Read raw ADC value (0–1023)
  int adcValue = analogRead(A0);

  // 2. Convert ADC to voltage
  float voltage = adcValue * (VCC / ADC_MAX);

  // 3. Calculate thermistor resistance using voltage divider formula
  // R_therm = R_fixed * (Vcc / Vout - 1)
  float resistance = SERIES_RESISTOR * (VCC / voltage - 1);

  // 4. Calculate temperature using Beta parameter equation
  // T = 1 / (1/T0 + (1/B) * ln(R/R0))
  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE;      // (R/R0)
  steinhart = log(steinhart);                       // ln(R/R0)
  steinhart /= B_COEFFICIENT;                       // (1/B) * ln(R/R0)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15); // + (1/T0)
  steinhart = 1.0 / steinhart;                      // Invert
  steinhart -= 273.15;                              // Convert to °C

  // 5. Print results
  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print("  | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V  | Resistance: ");
  Serial.print(resistance, 0);
  Serial.print(" ohms  | Temperature: ");
  Serial.print(steinhart, 2);
  Serial.println(" °C");

  delay(1000);
}
