#ifndef MOISTURESENSOR_H
#define MOISTURESENSOR_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

extern WiFiClient client;

class MoistureSensor {
  private:
    const int sensorPin = A0;
    const int dryValue = 700;  // Waarde in droge lucht
    const int wetValue = 300;  // Waarde in natte grond
    int lastMoisture = -1;

    int readMoisturePercent();

  public:
    void loop();
};

#endif // MOISTURESENSOR_H
