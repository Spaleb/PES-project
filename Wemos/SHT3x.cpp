#include "SHT3X.h"
#include <Wire.h>
#include <ESP8266WiFi.h>

extern WiFiClient client;

void SHT3xSensor::begin() {
  Wire.begin(D2, D1);      
  Wire.setClock(100000);   

  Wire.beginTransmission(SHT3X_ADRES);

  if (Wire.endTransmission() == 0) {
    sensorOk = true;
  } else {
    sensorOk = false;
  }
}

void SHT3xSensor::loop() {
  if (millis() - laatsteUpdate < 2000) return;
  laatsteUpdate = millis();

  if (leesSensor()) {

    if (client.connected()) {

      client.println("Temperatuur:" + String(temperatuur, 2));
      client.println("Luchtvochtigheid:" + String(luchtvochtigheid, 2));
      client.println("Ventilatiestand:" + String(getVentilatieStand()));
      client.println("Verwarming:" + String(isVerwarmingAan() ? 1 : 0));
    }
  } else {
  }
}

bool SHT3xSensor::leesSensor() {
  Wire.beginTransmission(SHT3X_ADRES);
  Wire.write(0x24);
  Wire.write(0x00);

  if (Wire.endTransmission() != 0) {
    sensorOk = false;
    return false;
  }

  delay(20);

  Wire.requestFrom(SHT3X_ADRES, (uint8_t)6);

  if (Wire.available() != 6) {
    sensorOk = false;
    return false;
  }

  uint8_t data[6];

  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();
  }

  uint16_t ruweTemperatuur = ((uint16_t)data[0] << 8) | data[1];
  uint16_t ruweLuchtvochtigheid = ((uint16_t)data[3] << 8) | data[4];

  temperatuur = -45.0 + 175.0 * ((float)ruweTemperatuur / 65535.0);
  luchtvochtigheid = 100.0 * ((float)ruweLuchtvochtigheid / 65535.0);

  sensorOk = true;
  return true;
}

float SHT3xSensor::getTemperatuur() {
  return temperatuur;
}

float SHT3xSensor::getLuchtvochtigheid() {
  return luchtvochtigheid;
}

int SHT3xSensor::getVentilatieStand() {
  if (luchtvochtigheid >= 70.0) return 3;
  if (luchtvochtigheid >= 60.0) return 2;
  if (luchtvochtigheid >= 50.0) return 1;

  return 0;
}

bool SHT3xSensor::isVerwarmingAan() {
  return temperatuur < 20.5;
}

bool SHT3xSensor::isOk() {
  return sensorOk;
}
