#include <WiFi.h>
#include <WebServer.h>
#include <esp_random.h>
#include <HomeSpan.h>
#include "index_html.h"
#include "klima.h"

// Wi-Fi bilgileri
const char* ssid     = "";
const char* password = "";

#ifndef LED_BUILTIN
  #define LED_BUILTIN 8
#endif

#ifndef RGB_BUILTIN
  #define RGB_BUILTIN 8
#endif

const int ledPin = LED_BUILTIN;
bool ledState = false;

uint8_t currentR = 0;
uint8_t currentG = 0;
uint8_t currentB = 0;

// NodeMCU heartbeat takibi
unsigned long lastNodemcuHeartbeat = 0;
const unsigned long nodemcuTimeout = 15000;

// Sistem degiskenleri
int plantMoisture = 68;
float distanceCm = 24.5;
bool tvPowerState = true;
int tvVolume = 18;

WebServer server(80);

void hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
  s /= 100.0f;
  v /= 100.0f;
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
  float m = v - c;
  float r_f = 0, g_f = 0, b_f = 0;
  if (h >= 0 && h < 60) { r_f = c; g_f = x; b_f = 0; }
  else if (h >= 60 && h < 120) { r_f = x; g_f = c; b_f = 0; }
  else if (h >= 120 && h < 180) { r_f = 0; g_f = c; b_f = x; }
  else if (h >= 180 && h < 240) { r_f = 0; g_f = c; b_f = x; }
  else if (h >= 240 && h < 300) { r_f = x; g_f = 0; b_f = c; }
  else { r_f = c; g_f = 0; b_f = x; }
  r = (uint8_t)((r_f + m) * 255);
  g = (uint8_t)((g_f + m) * 255);
  b = (uint8_t)((b_f + m) * 255);
}

void setRgbColor(uint8_t r, uint8_t g, uint8_t b) {
  currentR = r;
  currentG = g;
  currentB = b;

#ifdef RGB_BUILTIN
  neopixelWrite(RGB_BUILTIN, r, g, b);
#endif

  digitalWrite(ledPin, (r > 0 || g > 0 || b > 0) ? HIGH : LOW);
}

// HomeKit RGB LED servisi
struct DEV_RgbLED : Service::LightBulb {
  SpanCharacteristic *power;
  SpanCharacteristic *H;
  SpanCharacteristic *S;
  SpanCharacteristic *V;

  DEV_RgbLED() : Service::LightBulb() {
    power = new Characteristic::On();
    H = new Characteristic::Hue(0);
    S = new Characteristic::Saturation(0);
    V = new Characteristic::Brightness(100);
  }

  boolean update() override {
    if (power->getNewVal() == 0) {
      setRgbColor(0, 0, 0);
      ledState = false;
    } else {
      ledState = true;
      float hue = H->getNewVal();
      float sat = S->getNewVal();
      float val = V->getNewVal();
      uint8_t r, g, b;
      hsvToRgb(hue, sat, val, r, g, b);
      setRgbColor(r, g, b);
    }
    return true;
  }
};

// HomeKit Saksi servisi
struct DEV_PlantWater : Service::Switch {
  SpanCharacteristic *power;
  DEV_PlantWater() : Service::Switch() {
    power = new Characteristic::On();
  }
  boolean update() override {
    if (power->getNewVal() == 1) {
      plantMoisture = min(100, plantMoisture + 15);
      Serial.println("HomeKit: Saksi sulama tetiklendi");
      power->setVal(0);
    }
    return true;
  }
};

// HomeKit TV servisi
struct DEV_TvPower : Service::Switch {
  SpanCharacteristic *power;
  DEV_TvPower() : Service::Switch() {
    power = new Characteristic::On(true);
  }
  boolean update() override {
    tvPowerState = power->getNewVal();
    Serial.printf("HomeKit: TV -> %s\n", tvPowerState ? "ACIK" : "KAPALI");
    return true;
  }
};

DEV_RgbLED *devRgbLed = nullptr;
DEV_PlantWater *devPlantWater = nullptr;
DEV_TvPower *devTvPower = nullptr;
DEV_Klima *devKlima = nullptr;

void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

// Tum durum verisi
void handleApiStatus() {
  bool nodemcuOnline = (lastNodemcuHeartbeat > 0) && ((millis() - lastNodemcuHeartbeat) < nodemcuTimeout);
  int lastSeenSec = nodemcuOnline ? (int)((millis() - lastNodemcuHeartbeat) / 1000) : -1;

  String json = "{";
  json += "\"device\":\"ESP32-C6\",";
  json += "\"led\":" + String(ledState ? "true" : "false") + ",";
  json += "\"r\":" + String(currentR) + ",";
  json += "\"g\":" + String(currentG) + ",";
  json += "\"b\":" + String(currentB) + ",";
  json += "\"nodemcu_online\":" + String(nodemcuOnline ? "true" : "false") + ",";
  json += "\"nodemcu_last_seen_sec\":" + String(lastSeenSec) + ",";
  json += "\"moisture\":" + String(plantMoisture) + ",";
  json += "\"distance\":" + String(distanceCm, 1) + ",";
  json += "\"tv_power\":" + String(tvPowerState ? "true" : "false") + ",";
  json += "\"tv_vol\":" + String(tvVolume) + ",";
  json += "\"klima_power\":" + String(klimaPowerState ? "true" : "false") + ",";
  json += "\"homekit\":\"\",";
  json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
  json += "\"uptime_ms\":" + String(millis());
  json += "}";

  server.send(200, "application/json", json);
}

void updateNodemcuHeartbeat() {
  lastNodemcuHeartbeat = millis();
}

void handleApiPing() {
  updateNodemcuHeartbeat();
  server.send(200, "application/json", "{\"status\":\"pong\"}");
}

void handleLedToggle() {
  updateNodemcuHeartbeat();
  ledState = !ledState;
  if (ledState) {
    uint8_t r = (esp_random() % 225) + 30;
    uint8_t g = (esp_random() % 225) + 30;
    uint8_t b = (esp_random() % 225) + 30;
    setRgbColor(r, g, b);
    Serial.printf("NodeMCU Buton: RGB acildi (R=%d, G=%d, B=%d)\n", r, g, b);
  } else {
    setRgbColor(0, 0, 0);
    Serial.println("NodeMCU Buton: RGB kapatildi");
  }
  handleApiStatus();
}

void handleApiRgb() {
  if (server.hasArg("action") && server.arg("action") == "toggle") {
    ledState = !ledState;
    if (ledState) {
      uint8_t r = (esp_random() % 225) + 30;
      uint8_t g = (esp_random() % 225) + 30;
      uint8_t b = (esp_random() % 225) + 30;
      setRgbColor(r, g, b);
    } else {
      setRgbColor(0, 0, 0);
    }
  } else if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    uint8_t r = server.arg("r").toInt();
    uint8_t g = server.arg("g").toInt();
    uint8_t b = server.arg("b").toInt();
    ledState = (r > 0 || g > 0 || b > 0);
    setRgbColor(r, g, b);
  }
  handleApiStatus();
}

void handleApiPlant() {
  if (server.hasArg("action") && server.arg("action") == "water") {
    plantMoisture = min(100, plantMoisture + 15);
  }
  handleApiStatus();
}

void handleApiTv() {
  if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");
    if (cmd == "power") tvPowerState = !tvPowerState;
    else if (cmd == "vol_up") tvVolume = min(100, tvVolume + 2);
    else if (cmd == "vol_down") tvVolume = max(0, tvVolume - 2);
  }
  handleApiStatus();
}

void handleApiDistance() {
  if (server.hasArg("val")) {
    distanceCm = server.arg("val").toFloat();
  }
  handleApiStatus();
}

void handleApiKlima() {
  if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");
    if (cmd == "1" || cmd == "on") {
      klimaAc();
      if (devKlima && devKlima->power) devKlima->power->setVal(1);
    } else if (cmd == "0" || cmd == "off") {
      klimaKapat();
      if (devKlima && devKlima->power) devKlima->power->setVal(0);
    } else if (cmd == "toggle") {
      if (klimaPowerState) {
        klimaKapat();
        if (devKlima && devKlima->power) devKlima->power->setVal(0);
      } else {
        klimaAc();
        if (devKlima && devKlima->power) devKlima->power->setVal(1);
      }
    }
  }
  handleApiStatus();
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Bulunamadi");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\nESP32-C6 Baslatiliyor...");

  pinMode(ledPin, OUTPUT);
  setRgbColor(0, 0, 0);

  // IR LED pini
  pinMode(kIrLedPin, OUTPUT);
  digitalWrite(kIrLedPin, LOW);

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
    Serial.println("\nWiFi baglandi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }

  // HomeSpan port ve eslesme ayarlari
  homeSpan.setPortNum(8080);
  homeSpan.setPairingCode("");
  homeSpan.setControlPin(0);
  homeSpan.begin(Category::Lighting, "ESP32 Akıllı Ev");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("ESP32-C6 Hub");
      new Characteristic::Manufacturer("Zafer IoT");
      new Characteristic::SerialNumber("ESP32C6-001");
      new Characteristic::Model("Akıllı Ev Hub");
      new Characteristic::Identify();

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Salon RGB LED");
    devRgbLed = new DEV_RgbLED();

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Saksı Sulama");
    devPlantWater = new DEV_PlantWater();

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("TV Kumandası");
    devTvPower = new DEV_TvPower();

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Klima");
    devKlima = new DEV_Klima();

  // Web server route tanimlari
  server.on("/", handleRoot);
  server.on("/api/status", handleApiStatus);
  server.on("/api/ping", handleApiPing);
  server.on("/api/rgb", handleApiRgb);
  server.on("/api/plant", handleApiPlant);
  server.on("/api/tv", handleApiTv);
  server.on("/api/distance", handleApiDistance);
  server.on("/api/klima", handleApiKlima);
  server.on("/led/toggle", handleLedToggle);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web server baslatildi (Port 80)");
  Serial.println("HomeSpan baslatildi (Port 8080)");
  Serial.println("IR klima kontrolu aktif (GPIO 15)");
}

void loop() {
  homeSpan.poll();
  server.handleClient();

  // Seri porttan komut takibi ('1' veya '0')
  if (Serial.available() > 0) {
    char komut = Serial.read();
    if (komut == '1') {
      klimaAc();
      if (devKlima && devKlima->power) devKlima->power->setVal(1);
    } else if (komut == '0') {
      klimaKapat();
      if (devKlima && devKlima->power) devKlima->power->setVal(0);
    }
  }
}
