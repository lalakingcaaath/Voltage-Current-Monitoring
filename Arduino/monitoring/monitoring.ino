#include <ESP8266WiFi.h>       // REQUIRED: Added this library
#include <PubSubClient.h>

#define ADC_PIN A0
#define RELAY_PIN D1    
#define BUZZER_PIN D2   

/************ WIFI SETTINGS ************/
const char* ssid = "ZTE_2.4G_dXjS9n";
const char* password = "sRx7sz4x";

/************ HiveMQTT Credentials ************/
const char* mqtt_server = "b9f1563767a940eaaeb700bbbcb6f8c8.s1.eu.hivemq.cloud";
const int mqtt_port = 8883; 
const char* mqtt_user = "caseeey";
const char* mqtt_password = "Kuyamopio17";

// Define Secure Client for HiveMQ
WiFiClientSecure espClient;
PubSubClient client(espClient);

/************ SENSOR SETTINGS ************/
const int voltageSamples = 1000;
float voltageCalibration = 650.0;
const int currentSamples = 1000;
float acsSensitivity = 100.0; 
float maxVoltage = 260.0; 
float maxCurrent = 0.50; 

// Function Prototypes
float readZMPTVoltage();
float readACS712Current();
void setupWiFi();
void reconnect();

void setup() {
  Serial.begin(9600);
  
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Start with relay OFF (safe)
  digitalWrite(RELAY_PIN, HIGH);  
  digitalWrite(BUZZER_PIN, LOW); 

  // 1. Connect to Wi-Fi
  setupWiFi();

  // 2. Setup Secure MQTT
  // REQUIRED: This line allows the ESP to trust HiveMQ Cloud
  espClient.setInsecure(); 
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // 3. ADDED: Connection Maintenance
  // Without this, the device will not stay connected
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 

  // --- EXISTING SENSOR LOGIC (UNCHANGED) ---
  float acVoltage = readZMPTVoltage();
  float acCurrent = readACS712Current();

  // Relay protection logic
  bool fault = false;
  if (acVoltage > maxVoltage) fault = true;
  if (acCurrent > maxCurrent) fault = true;

  if (fault) {
    digitalWrite(RELAY_PIN, LOW);   // Cut power
    digitalWrite(BUZZER_PIN, HIGH); // Turn buzzer ON
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Allow power
    digitalWrite(BUZZER_PIN, LOW);  // Turn buzzer OFF
  }

  // --- ADDED: Publish Data to Dashboard ---
  // Send data every 2 seconds without blocking the loop
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 2000) {
    lastMsg = millis();

    // Create JSON payload
    String payload = "{\"voltage\": " + String(acVoltage, 1) + 
                     ", \"current\": " + String(acCurrent, 2) + 
                     ", \"status\": \"" + (fault ? "ALARM" : "NORMAL") + "\"}";
    
    Serial.print("Publishing: ");
    Serial.println(payload);
    
    // Send to HiveMQ topic
    client.publish("home/telemetry", payload.c_str());
  }
}

/* ================= WIFI SETUP ================= */
void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

/* ================= RECONNECT ================= */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random Client ID
    String clientId = "ESP8266-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5s");
      delay(5000);
    }
  }
}

/* ================= ZMPT101B VOLTAGE (UNCHANGED) ================= */
float readZMPTVoltage() {
  float voltageSumSquares = 0;
  float voltageOffset = 0;

  for (int i = 0; i < 300; i++) {
    voltageOffset += analogRead(ADC_PIN);
    delayMicroseconds(100);
  }
  voltageOffset /= 300.0;

  for (int i = 0; i < voltageSamples; i++) {
    float adcValue = analogRead(ADC_PIN);
    float centered = adcValue - voltageOffset;
    voltageSumSquares += centered * centered;
    delayMicroseconds(100);
  }

  float voltageRMS = sqrt(voltageSumSquares / voltageSamples);
  float sensorVoltage = (voltageRMS * 3.3) / 1023.0;
  return sensorVoltage * voltageCalibration;
}

/* ================= ACS712 CURRENT (UNCHANGED) ================= */
float readACS712Current() {
  float currentSumSquares = 0;
  float currentOffset = 0;

  for (int i = 0; i < 300; i++) {
    currentOffset += analogRead(ADC_PIN);
    delayMicroseconds(100);
  }
  currentOffset /= 300.0;

  for (int i = 0; i < currentSamples; i++) {
    float adcValue = analogRead(ADC_PIN);
    float centered = adcValue - currentOffset;
    currentSumSquares += centered * centered;
    delayMicroseconds(100);
  }

  float currentRMS = sqrt(currentSumSquares / currentSamples);
  float sensorVoltage = (currentRMS * 3.3) / 1023.0;
  return (sensorVoltage * 1000.0) / acsSensitivity;
}