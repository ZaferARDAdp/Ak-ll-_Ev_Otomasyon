#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <Adafruit_SHT31.h>


const char* ssid     = "WIFI_ADI";
const char* password = "WIFI_SIFRESI";
const char* esp32_ip = "ESP32_IP_ADRESI";

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif
const int ledPin    = LED_BUILTIN;
const int buttonPin = 0; 

#define I2C_SDA D2 // GPIO4
#define I2C_SCL D1 // GPIO5

VL53L0X tofSensor;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

bool tofReady = false;
bool shtReady = false;

// Sensor degerleri
float currentTemp = 0.0;
float currentHum  = 0.0;
float currentDistCm = 999.0;

// Esik degerleri (Otomasyon)
const float TEMP_AC_THRESHOLD = 25.0; // 25 C uzeri ac
const float TEMP_HYSTERESIS   = 24.0; // 24 C alti reset
bool tempTriggered = false;

// Jest zamanlamasi
unsigned long lastGestureTime = 0;
const unsigned long gestureCooldown = 2500;
bool gestureWaitingRelease = false;

// Telemetri periyodu (3 sn)
unsigned long lastTelemetryTime = 0;
const unsigned long telemetryInterval = 3000;

// Buton debounce
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150;

void blinkLed(int ms = 60) {
  digitalWrite(ledPin, LOW);
  delay(ms);
  digitalWrite(ledPin, HIGH);
}

// ESP32ye HTTP GET istegi
bool sendHttpRequest(String path) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi baglantisi yok, baglanmaya calisiliyor...");
    WiFi.begin(ssid, password);
    return false;
  }

  WiFiClient client;
  client.setNoDelay(true);
  client.setTimeout(1500);

  HTTPClient http;
  http.setTimeout(1500);

  String url = "http://" + String(esp32_ip) + path;
  Serial.print("Istek: ");
  Serial.println(url);

  http.begin(client, url);
  int httpCode = http.GET();

  bool success = false;
  if (httpCode > 0) {
    Serial.printf("ESP32 Yanit: %d\n", httpCode);
    blinkLed(70);
    success = true;
  } else {
    Serial.printf("HTTP Hata: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return success;
}

void sendKlimaCommand(const char* cmd) {
  if (strcmp(cmd, "1") == 0 || strcmp(cmd, "on") == 0) {
    Serial.println("Klima AC komutu gonderiliyor...");
  } else {
    Serial.println("Klima KAPAT komutu gonderiliyor...");
  }
  
  String path = "/api/klima?cmd=" + String(cmd);
  sendHttpRequest(path);
}

void sendTelemetryData() {
  if (WiFi.status() != WL_CONNECTED) return;

  // Sensor verileri
  String sensorPath = "/api/sensor?temp=" + String(currentTemp, 1) +
                      "&hum=" + String(currentHum, 1) +
                      "&dist=" + String(currentDistCm, 1);
  sendHttpRequest(sensorPath);

  // Mesafe verisi
  String distPath = "/api/distance?val=" + String(currentDistCm, 1);
  sendHttpRequest(distPath);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\nNodeMCU baslatiliyor...");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(buttonPin, INPUT_PULLUP);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);

  // SHT3x baslat
  if (sht31.begin(0x44)) {
    Serial.println("SHT3x baslatildi (0x44)");
    shtReady = true;
  } else if (sht31.begin(0x45)) {
    Serial.println("SHT3x baslatildi (0x45)");
    shtReady = true;
  } else {
    Serial.println("SHT3x bulunamadi!");
  }

  // VL53L0X baslat
  tofSensor.setTimeout(500);
  if (tofSensor.init()) {
    tofSensor.setSignalRateLimit(0.1);
    tofSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    tofSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    tofSensor.setMeasurementTimingBudget(50000);

    Serial.println("VL53L0X baslatildi");
    tofReady = true;
  } else {
    Serial.println("VL53L0X bulunamadi!");
  }

  // WiFi baglantisi
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);

  Serial.print("WiFi baglaniliyor: ");
  Serial.println(ssid);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi baglandi.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("ESP32 Hedef IP: ");
    Serial.println(esp32_ip);
    blinkLed(300);
  } else {
    Serial.println("\nWiFi baglantisi kurulamadi!");
  }
}

void loop() {
  unsigned long now = millis();

  // 1. VL53L0X mesafe okuma (~80ms)
  static unsigned long lastDistanceRead = 0;
  if (tofReady && (now - lastDistanceRead >= 80)) {
    lastDistanceRead = now;
    uint16_t distMm = tofSensor.readRangeSingleMillimeters();
    if (tofSensor.timeoutOccurred() || distMm >= 8190 || distMm > 1200) {
      currentDistCm = 999.0;
    } else {
      currentDistCm = distMm / 10.0;
    }
  }

  // 2. SHT3x sicaklik ve nem okuma (~1000ms)
  static unsigned long lastSensorRead = 0;
  if (shtReady && (now - lastSensorRead >= 1000)) {
    lastSensorRead = now;
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      currentTemp = t;
      currentHum  = h;
    }
  }

  // 3. El hareketi (jest) kontrolu
  if (currentDistCm > 40.0 || currentDistCm >= 999.0) {
    gestureWaitingRelease = false;
  }

  if (!gestureWaitingRelease && (now - lastGestureTime >= gestureCooldown)) {
    // Yakin hareket: 1-10 cm -> Klima AC
    if (currentDistCm > 1.0 && currentDistCm < 10.0) {
      Serial.printf("[JEST] Yakin el algilandi: %.1f cm -> Klima AC\n", currentDistCm);
      sendKlimaCommand("1");
      lastGestureTime = now;
      gestureWaitingRelease = true;
    }
    // Uzak hareket: 20-35 cm -> Klima KAPAT
    else if (currentDistCm >= 20.0 && currentDistCm <= 35.0) {
      Serial.printf("[JEST] Uzak el algilandi: %.1f cm -> Klima KAPAT\n", currentDistCm);
      sendKlimaCommand("0");
      lastGestureTime = now;
      gestureWaitingRelease = true;
    }
  }

  // 4. Sicaklik esik kontrolu
  if (shtReady && currentTemp > 0) {
    if (currentTemp >= TEMP_AC_THRESHOLD && !tempTriggered) {
      Serial.printf("[OTO] Sicaklik yuksek: %.2f C -> Klima AC\n", currentTemp);
      sendKlimaCommand("1");
      tempTriggered = true;
    }
    else if (currentTemp < TEMP_HYSTERESIS && tempTriggered) {
      Serial.printf("[OTO] Sicaklik normale dondu: %.2f C\n", currentTemp);
      tempTriggered = false;
    }
  }

  // 5. Buton kontrolu
  int currentButtonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    if (now - lastDebounceTime > debounceDelay) {
      lastDebounceTime = now;
      Serial.println("[BUTON] Klima toggle gonderiliyor");
      sendHttpRequest("/api/klima?cmd=toggle");
    }
  }
  lastButtonState = currentButtonState;

  // 6. Seri port log ve telemetri aktarimi
  if (now - lastTelemetryTime >= telemetryInterval) {
    lastTelemetryTime = now;

    Serial.printf("[CANLI] Sicaklik: %.1f C | Nem: %%%.1f | Mesafe: ", currentTemp, currentHum);
    if (currentDistCm >= 999.0) {
      Serial.print("Menzil Disi");
    } else {
      Serial.printf("%.1f cm", currentDistCm);
    }
    Serial.printf(" | WiFi: %s\n", (WiFi.status() == WL_CONNECTED) ? "Bagli" : "Kopuk");

    sendTelemetryData();
  }

  delay(30);
}
