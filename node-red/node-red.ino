// Include necessary libraries
#include "DHT.h"
#include <ArduinoJson.h> // Library for creating JSON objects

// --- Pin Definitions ---
#define DHTPIN 4          // Pin connected to the DHT22 data pin
#define DHTTYPE DHT22     // Define the type of DHT sensor

const int ldrPin = 34;    // LDR is connected to ADC1_CH6 (GPIO 34)

// Relay control pins
const int relay1Pin = 25;
const int relay2Pin = 26;
const int relay3Pin = 27;
const int relay4Pin = 33;

// Initialize DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// Timer for sending data periodically
unsigned long previousMillis = 0;
const long interval = 2000; // Send data every 2 seconds

void setup() {
  // Start serial communication at 115200 baud rate
  Serial.begin(115200);

  // Initialize the DHT sensor
  dht.begin();

  // Set relay pins as outputs
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay4Pin, OUTPUT);

  // Initialize relays to OFF state.
  // Note: Most relay modules are active-low, meaning HIGH turns them OFF.
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, HIGH);
  digitalWrite(relay4Pin, HIGH);
}

void loop() {
  // --- Part 1: Read incoming commands from Node-RED ---
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any whitespace

    // Control relays based on command
    if (command == "R1_ON")  { digitalWrite(relay1Pin, LOW); }
    if (command == "R1_OFF") { digitalWrite(relay1Pin, HIGH); }
    if (command == "R2_ON")  { digitalWrite(relay2Pin, LOW); }
    if (command == "R2_OFF") { digitalWrite(relay2Pin, HIGH); }
    if (command == "R3_ON")  { digitalWrite(relay3Pin, LOW); }
    if (command == "R3_OFF") { digitalWrite(relay3Pin, HIGH); }
    if (command == "R4_ON")  { digitalWrite(relay4Pin, LOW); }
    if (command == "R4_OFF") { digitalWrite(relay4Pin, HIGH); }
  }

  // --- Part 2: Read sensors and send data to Node-RED periodically ---
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read sensor values
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Read temperature as Celsius
    int ldrValue = analogRead(ldrPin);

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("{\"error\":\"Failed to read from DHT sensor!\"}");
      return;
    }

    // Create a JSON object to send
    StaticJsonDocument<200> doc;
    doc["temp"] = temperature;
    doc["humid"] = humidity;
    doc["ldr"] = ldrValue;

    // Serialize JSON to string and print to Serial
    serializeJson(doc, Serial);
    Serial.println(); // Print a newline to signify end of message
  }
}
