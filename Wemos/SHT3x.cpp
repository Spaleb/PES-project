#include "SHT3X.h"
#include <Wire.h>
#include <ESP8266WiFi.h>

/**
 * @file SHT3X.cpp
 * @brief Implementatie voor het uitlezen en versturen van SHT3X-sensordata.
 */

 /** @brief WiFiClient uit main.cpp voor het versturen van meetdata. */
extern WiFiClient client;
extern char DEVICE_ID;


/**
 * @brief Start I2C en controleert of de SHT3X-sensor bereikbaar is.
 */
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

/**
 * @brief Leest periodiek de sensor uit en stuurt meetdata naar de server.
 */
void SHT3xSensor::loop() {
  if (millis() - laatsteUpdate < 10000) return;
  laatsteUpdate = millis();

  if (leesSensor()) {

    if (client.connected()) {

      client.println("Temperatuur:" + String(temperatuur, 2));
      client.println("Luchtvochtigheid:" + String(luchtvochtigheid, 2));
      client.println("Ventilatiestand:" + String(getVentilatieStand()));
      client.println("Verwarming:" + String(isVerwarmingAan() ? 1 : 0));
    }
  } else {
    // Sensorfout wordt verwerkt in leesSensor().
  }
}

/**
 * @brief Verwerkt ventilatiecommando's die via de server binnenkomen.
 * @param command Het ontvangen commando, bijvoorbeeld "VENTon" of "VENToff".
 * @param client WiFiClient waarmee een statusbericht teruggestuurd kan worden.
 */
void SHT3xSensor::handleCommand(String command, WiFiClient &client) 
{
	if (command == "VENToff")
        brandActief = true;
    else if (command == "VENTon") 
        brandActief = false;
}

/**
 * @brief Leest temperatuur en luchtvochtigheid uit via I2C.
 * @return true als 6 bytes succesvol zijn ontvangen, anders false.
 */
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

/**
 * @brief Geeft de laatst gemeten temperatuur terug.
 * @return Temperatuur in graden Celsius.
 */
float SHT3xSensor::getTemperatuur() {
  return temperatuur;
}

/**
 * @brief Geeft de laatst gemeten luchtvochtigheid terug.
 * @return Luchtvochtigheid in procenten.
 */
float SHT3xSensor::getLuchtvochtigheid() {
  return luchtvochtigheid;
}

/**
 * @brief Bepaalt de ventilatiestand.
 * @return Stand 0, 1, 2 of 3.
 */

int SHT3xSensor::getVentilatieStand() 
{
  /**
 * @brief //Als er brand is moet de laagste stand van de ventilatie gereturned worden. Anders mag de luchtvochtigheid weer gecheckt worden.
 * @return Stand 0,1,2,3
 */
  if (brandActief) return 0;

  if (luchtvochtigheid >= 70.0) return 3;
  if (luchtvochtigheid >= 60.0) return 2;
  if (luchtvochtigheid >= 50.0) return 1;

  return 0;
}

/**
 * @brief Controleert of de verwarming aan moet.
 * @return true als temperatuur lager is dan 20.5°C.
 */
bool SHT3xSensor::isVerwarmingAan() {
  return temperatuur < 20.5;
}

/**
 * @brief Geeft de sensorstatus terug.
 * @return true als de sensor goed werkt.
 */
bool SHT3xSensor::isOk() {
  return sensorOk;
}
