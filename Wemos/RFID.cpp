#include "RFID.h"
#include <ESP8266WiFi.h>

extern WiFiClient client;
extern char DEVICE_ID;

/**
 * @brief Constructor for RFID handler.
 * Initializes PN532 instance with default I2C pins.
 */
RFID::RFID() : nfc(-1, -1) {}

/**
 * @brief Initializes the PN532 RFID module and I2C communication.
 * 
 * Sets up Wire (I2C), starts PN532, verifies firmware version,
 * configures Secure Access Module (SAM), and prepares for card reading.
 */
void RFID::begin() {
  Wire.begin(D2, D1);
  Wire.setClock(100000);

  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();

  if (!version) {
    Serial.println("PN532 niet gevonden");
    while (1);
  }

  nfc.SAMConfig();
}

/**
 * @brief Polls for RFID cards and sends UID when a new card is detected.
 * 
 * Reads passive ISO14443A tags at a fixed interval, converts UID to
 * uppercase hex string, and sends it over Serial and WiFi client if new.
 */
void RFID::update() {
  static unsigned long lastRead = 0;
  static String lastUID = "";

  const unsigned long interval = 150;
  const unsigned long cooldown = 1000; // 1 second

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

  if (!success) {
    return;
  }

  String uidStr = "";

  for (int i = 0; i < uidLength; i++) {
    if (uid[i] < 0x10) uidStr += "0";
    uidStr += String(uid[i], HEX);
  }

  uidStr.toUpperCase();

  static unsigned long lastSentTime = 0;

  // prevent spam
  if (uidStr == lastUID && millis() - lastSentTime < cooldown) {
    return;
  }

  lastUID = uidStr;
  lastSentTime = millis();

  client.println("ID:" + uidStr);
}
