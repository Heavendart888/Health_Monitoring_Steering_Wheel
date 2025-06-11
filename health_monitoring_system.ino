#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "MAX3010x.h"
#include <HardwareSerial.h>
#include <TinyGPS++.h>

MAX30105 particleSensor;
HardwareSerial sim800(1); // SIM800L
HardwareSerial gpsSerial(2); // GPS

TinyGPSPlus gps;

#define RELAY_PIN 27
#define ECG_PIN 34

#define SIM_TX 17
#define SIM_RX 16

#define GPS_TX 4
#define GPS_RX 5

int bpm = 0;
unsigned long lastCheck = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Initialize SIM800
  sim800.begin(9600, SERIAL_8N1, SIM_RX, SIM_TX);
  delay(1000);

  // GPS Init
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // MAX30100 Init
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX3010x not found. Check wiring.");
    while (1);
  }

  particleSensor.setup(); 
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeIR(0x0A);
}

void sendSMS(String msg) {
  sim800.println("AT+CMGF=1");
  delay(500);
  sim800.println("AT+CMGS=\"+91XXXXXXXXXX\"");  // Replace with your number
  delay(500);
  sim800.print(msg);
  delay(200);
  sim800.write(26); // Ctrl+Z
  delay(2000);
}

String getGPS() {
  String gpsLink = "Location not found";
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid()) {
    gpsLink = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }
  return gpsLink;
}

void loop() {
  // ECG Reading
  int ecgVal = analogRead(ECG_PIN);

  // HR Reading
  long irValue = particleSensor.getIR();
  if (irValue < 50000) {
    Serial.println("Finger not detected");
    return;
  }

  bpm = particleSensor.getHeartRate();

  if (millis() - lastCheck > 5000) {
    lastCheck = millis();
    Serial.print("BPM: "); Serial.println(bpm);
    Serial.print("ECG: "); Serial.println(ecgVal);

    if (bpm < 45 || bpm > 130 || ecgVal < 100 || ecgVal > 900) {
      digitalWrite(RELAY_PIN, HIGH);
      String msg = "🚨 Driver Emergency Detected!\n";
      msg += "Heart Rate: " + String(bpm) + " bpm\n";
      msg += "ECG: " + String(ecgVal) + "\n";
      msg += "Location: " + getGPS();
      sendSMS(msg);
    } else {
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}
