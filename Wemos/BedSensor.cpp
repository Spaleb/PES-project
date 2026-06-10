#include <ESP8266WiFi.h>
#include "BedSensor.h"

extern WiFiClient client;

/**
 * @brief Construeert een nieuw BedSensor:: BedSensor object.
 * 
 * @param pin Het pin waar de bed sensor op aangesloten is
 */
BedSensor::BedSensor(int pin) {
  sensorPin = pin;
  bedPressure = false;
}

/**
 * @brief Leest de waarde van de bed sensor.
 * 
 * @return true Als er druk op de bed sensor is en groter is dan 500 (drempelwaarde).
 * @return false Als er geen druk op de bed sensor is of als de waarde kleiner is dan of gelijk aan 500 (drempelwaarde).
 */
bool BedSensor::read() {
  sensorValue = analogRead(sensorPin);
  return (sensorValue > 500);
}

/**
 * @brief Controlleert of de status van de bed sensor is veranderd sinds de laatste keer dat deze functie is aangeroepen.
 * 
 * @param newValue De nieuwe waarde van de bed sensor.
 * @return true Als de status is veranderd.
 * @return false Als de status niet is veranderd.
 */
bool BedSensor::hasChanged(bool newValue) {
  if (newValue != bedPressure) {
    bedPressure = newValue;
    return true;
  }
  return false;
}

/**
 * @brief Update de status van de bed sensor en stuurt een 
 * bericht naar de client als er een verandering is in de status van de bed sensor.
 * 
 */
void BedSensor::update() {
  bool newValue = read();

  if (hasChanged(newValue)) {
    if (bedPressure) {
      Serial.println("BED:ON");
      client.println("BED:ON");
    } else {
      Serial.println("BED:OFF");
      client.println("BED:OFF");
    }
  }
}
