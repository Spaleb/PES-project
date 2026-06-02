#include "MoistureSensor.h"

int MoistureSensor::readMoisturePercent() {
  int sensorValue = analogRead(sensorPin);
  int moisturePercent = map(sensorValue, dryValue, wetValue, 0, 100);
  return constrain(moisturePercent, 0, 100);
}

void MoistureSensor::loop() {
  int moisture = readMoisturePercent();

  if ((moisture - lastMoisture) > 2 || (lastMoisture - moisture) > 2) {
    //Serial.println("MOI:" + String(moisture));
    client.println("MOI:" + String(moisture));

    lastMoisture = moisture;
  }
}
