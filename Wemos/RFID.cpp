#include "RFID.h"
#include <ESP8266WiFi.h>

extern WiFiClient client;
extern char DEVICE_ID;

RFID::RFID() : nfc(-1, -1) {}

void RFID::begin() {
  Wire.begin(D2, D1);
  Wire.setClock(100000);

  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();

  if (!version) {
    Serial.println("PN532 niet gevonden");
    while (1);
  }

  Serial.println("PN532 OK");
  client.println("PN532 OK");

  nfc.SAMConfig();
  Serial.println("Wacht op kaart...");
}

void RFID::update() {
  static unsigned long lastRead = 0;
  const unsigned long interval = 150;

  if (millis() - lastRead < interval) return;
  lastRead = millis();

  uint8_t uid[7];
  uint8_t uidLength;

  bool success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength,
    50
  );

  if (!success) return;

  // UID maken
  String uidStr = "";

  for (int i = 0; i < uidLength; i++) {
    if (uid[i] < 0x10) uidStr += "0";
    uidStr += String(uid[i], HEX);
  }

  uidStr.toUpperCase();

  Serial.println("Kaart: " + uidStr);

  // naar server sturen
  if (client.connected()) {
    client.println(uidStr);
  }
}
