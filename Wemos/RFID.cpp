#include "RFID.h"
#include <ESP8266WiFi.h>

extern WiFiClient client;
extern char DEVICE_ID;

RFID::RFID() : nfc(-1, -1) {}

/**
 * @brief 
 * Deze functie initialiseert de RFID-module. Het maakt gebruik van de Wire-library om te communiceren met de PN532-module via I2C.
 * 
 */
void RFID::begin() {
  Wire.begin(D2, D1);
  Wire.setClock(100000);

  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();

  if (!version) {
    while (1);
  }

  nfc.SAMConfig();
}

/**
 * @brief 
 * Deze functie leest de RFID-tags. Het controleert regelmatig of er een nieuwe tag is gescand en stuurt de UID van de tag naar de server als deze is gescand.
 * Om te voorkomen dat dezelfde tag meerdere keren achter elkaar wordt verzonden, wordt er een cooldown-periode ingesteld waarin dezelfde tag niet opnieuw wordt verzonden. 
 * 
 */
void RFID::update() {
  static unsigned long lastRead = 0;
  static String lastUID = "";

  const unsigned long interval = 150;
  const unsigned long cooldown = 1000;

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

  if (!success) { // Geen tag gescand, stop de loop hier
    return;
  }

  String uidStr = "";

  for (int i = 0; i < uidLength; i++) {
    if (uid[i] < 0x10) uidStr += "0";
    uidStr += String(uid[i], HEX);
  }

  uidStr.toUpperCase();

  static unsigned long lastSentTime = 0;

  if (uidStr == lastUID && millis() - lastSentTime < cooldown) { // Zelfde tag binnen cooldown-periode, niet verzenden
    return;
  }

  lastUID = uidStr;
  lastSentTime = millis();

  client.println("ID:" + uidStr); // UID van de gescande tag naar de server sturen
}
