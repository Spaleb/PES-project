#include "BedSensor.h"

/**
 * @brief Construct a new Bed Sensor:: Bed Sensor object
 * 
 * @param pin 
 */
BedSensor::BedSensor(int pin) {
  sensorPin = pin;
  bedPressure = false;
}
/**
 * @brief Returns if there is something measured on the bed pressure sensor.
 * Only when the return value is higher than 500 (to ignore random, small pressures).
 * 
 * @return true 
 * @return false 
 */
bool BedSensor::read() {
  sensorValue = analogRead(sensorPin);
  return (sensorValue > 500);
}

/**
 * @brief Checks if the status has changed
 * 
 * @param newValue 
 * @return true 
 * @return false 
 */
bool BedSensor::hasChanged(bool newValue) {
  if (newValue != bedPressure) {
    bedPressure = newValue;
    return true;
  }
  return false;
}

/**
 * @brief If the status has changed there will be send a message to the serial.
 * In the future this will send a message to the Pi.
 * 
 */
void BedSensor::update() {
  bool newValue = read();

  if (hasChanged(newValue)) {
    if (bedPressure) {
      Serial.println("BED ON");
    } else {
      Serial.println("BED OFF");
    }
  }
}
