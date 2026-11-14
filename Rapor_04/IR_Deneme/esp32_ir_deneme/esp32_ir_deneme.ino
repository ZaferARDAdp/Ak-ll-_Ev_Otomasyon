#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2 // VS1838B Alici 
#define IR_SEND_PIN    3 // IR LED Transistor 

uint16_t lastCommand = 0;
uint16_t lastAddress = 0;
decode_type_t lastProtocol = UNKNOWN;

void setup() {
  Serial.begin(115200);

  unsigned long start = millis();
  while (!Serial && millis() - start < 3000);

  Serial.println("ESP32-C6 IR Test Baslatildi");
  Serial.println("Komutlar: 'T' veya 'SEND' -> son kodu gonder | 'TEST' -> NEC test");

  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  IrSender.begin(IR_SEND_PIN);
}

void loop() {
  // Sinyal yakalama
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
      Serial.println("\n[IR KODU ALINDI]");
      Serial.print("Protokol : ");
      Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));
      Serial.print("Adres    : 0x");
      Serial.println(IrReceiver.decodedIRData.address, HEX);
      Serial.print("Komut    : 0x");
      Serial.println(IrReceiver.decodedIRData.command, HEX);
      Serial.print("Ham Kod  : 0x");
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

      lastAddress = IrReceiver.decodedIRData.address;
      lastCommand = IrReceiver.decodedIRData.command;
      lastProtocol = IrReceiver.decodedIRData.protocol;
    }
    IrReceiver.resume();
  }

  
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toUpperCase();

    if (input == "T" || input == "SEND") {
      if (lastProtocol != UNKNOWN) {
        Serial.print("Gonderiliyor -> Adres: 0x");
        Serial.print(lastAddress, HEX);
        Serial.print(" Komut: 0x");
        Serial.println(lastCommand, HEX);

        IrSender.write(lastProtocol, lastAddress, lastCommand);
        IrReceiver.start();
        Serial.println("Sinyal gonderildi.");
      } else {
        Serial.println("Hafizada kayitli kod yok.");
      }
    } 
    else if (input == "TEST") {
      Serial.println("Test sinyali (NEC) gonderiliyor...");
      IrSender.sendNEC(0x00, 0x12, 0);
      IrReceiver.start();
      Serial.println("Test sinyali gonderildi.");
    }
  }
}
