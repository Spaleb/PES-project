#include "MoistureSensor.h"

int MoistureSensor::readMoisturePercent() {
  int sensorValue = analogRead(sensorPin);
  int moisturePercent = map(sensorValue, dryValue, wetValue, 0, 100);
  return constrain(moisturePercent, 0, 100);
}

void MoistureSensor::loop() {
  int moisture = readMoisturePercent();

  if ((moisture - lastMoisture) > 5 || (lastMoisture - moisture) > 5) {
    client.println("MOI:" + String(moisture));

    lastMoisture = moisture;
  }
}
