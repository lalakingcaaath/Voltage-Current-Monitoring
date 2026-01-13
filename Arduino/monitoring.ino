#define ADC_PIN A0
#define RELAY_PIN D1    // Relay control pin (change if needed)
#define BUZZER_PIN D2   // Buzzer pin

/************ ZMPT101B SETTINGS ************/
const int voltageSamples = 1000;
float voltageCalibration = 650.0;

/************ ACS712 SETTINGS ************/
const int currentSamples = 1000;
float acsSensitivity = 100.0;  // ACS712-20A

/************ SAFETY THRESHOLDS ************/
float maxVoltage = 260.0;   // Volts
float maxCurrent = 0.50;    // Amps

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Start with relay OFF (safe)
  digitalWrite(RELAY_PIN, HIGH);  // ACTIVE LOW relay
  digitalWrite(BUZZER_PIN, LOW);  // Buzzer OFF
}

void loop() {
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

  Serial.print("AC Voltage: ");
  Serial.print(acVoltage, 2);
  Serial.print(" V | AC Current: ");
  Serial.print(acCurrent, 2);
  Serial.print(" A | Relay: ");
  Serial.print(fault ? "OFF" : "ON");
  Serial.print(" | Buzzer: ");
  Serial.println(fault ? "ON" : "OFF");

  delay(1000);
}

/* ================= ZMPT101B VOLTAGE ================= */
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

/* ================= ACS712 CURRENT ================= */
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
