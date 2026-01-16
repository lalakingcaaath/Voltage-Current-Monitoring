#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <math.h>

/************ PIN DEFINITIONS ************/
#define VOLTAGE_PIN D3      // Voltage sensor enable/select pin
#define ADC_PIN A0          // Shared ADC input
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

// Secure MQTT client
WiFiClientSecure espClient;
PubSubClient client(espClient);

/************ SENSOR SETTINGS ************/
const int voltageSamples = 1000;
const int currentSamples = 1000;

float adcReference = 3.3;
float voltageCalibration = 235.0;   // Adjust during calibration
float acsSensitivity = 100.0;       // mV/A

float maxVoltage = 260.0;
float maxCurrent = 0.50;

/************ FUNCTION PROTOTYPES ************/
float readACVoltage();
float readACS712Current();
void setupWiFi();
void reconnect();

void setup() {
  Serial.begin(9600);

  pinMode(VOLTAGE_PIN, OUTPUT);   // Voltage sensor enable
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(VOLTAGE_PIN, LOW); // Sensor OFF initially
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, HIGH);

  setupWiFi();

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);

  randomSeed(analogRead(A0));
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float acVoltage = readACVoltage();
  float acCurrent = readACS712Current();

  bool fault = false;
  if (acVoltage > maxVoltage) fault = true;
  if (acCurrent > maxCurrent) fault = true;

  if (fault) {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  }

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 2000) {
    lastMsg = millis();

    String payload = "{\"voltage\": " + String(acVoltage, 1) +
                     ", \"current\": " + String(acCurrent, 2) +
                     ", \"status\": \"" + (fault ? "ALARM" : "NORMAL") + "\"}";

    Serial.println(payload);
    client.publish("home/telemetry", payload.c_str());
  }
}

/* ================= WIFI ================= */
void setupWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected");
}

/* ================= MQTT ================= */
void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("MQTT connected");
    } else {
      delay(5000);
    }
  }
}

/* ================= AC VOLTAGE (D3 CONTROLLED) ================= */
float readACVoltage() {
  float sumSquares = 0;
  float offset = 0;

  // Enable voltage sensor
  digitalWrite(VOLTAGE_PIN, HIGH);
  delay(2);  // Allow signal to stabilize

  // Offset calculation
  for (int i = 0; i < 300; i++) {
    offset += analogRead(ADC_PIN);
    delayMicroseconds(100);
  }
  offset /= 300.0;

  // RMS sampling
  for (int i = 0; i < voltageSamples; i++) {
    float sample = analogRead(ADC_PIN) - offset;
    sumSquares += sample * sample;
    delayMicroseconds(100);
  }

  // Disable voltage sensor
  digitalWrite(VOLTAGE_PIN, LOW);

  float adcRMS = sqrt(sumSquares / voltageSamples);
  float sensorVoltage = (adcRMS * adcReference) / 1023.0;

  return sensorVoltage * voltageCalibration;
}

/* ================= ACS712 CURRENT ================= */
float readACS712Current() {
  float sumSquares = 0;
  float offset = 0;

  for (int i = 0; i < 300; i++) {
    offset += analogRead(ADC_PIN);
    delayMicroseconds(100);
  }
  offset /= 300.0;

  for (int i = 0; i < currentSamples; i++) {
    float sample = analogRead(ADC_PIN) - offset;
    sumSquares += sample * sample;
    delayMicroseconds(100);
  }

  float adcRMS = sqrt(sumSquares / currentSamples);
  float sensorVoltage = (adcRMS * adcReference) / 1023.0;

  float current = (sensorVoltage * 1000.0) / acsSensitivity;
  return current - 0.07;
}
