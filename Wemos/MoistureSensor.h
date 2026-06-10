#ifndef MOISTURESENSOR_H
#define MOISTURESENSOR_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

extern WiFiClient client;

/**
 * @brief 
 * Deze klasse beheert de vochtigheidssensor. Het leest de analoge waarde van de sensor, zet deze om naar een percentage en stuurt een bericht naar de server als er een significante verandering in vochtigheid is gedetecteerd.
 * 
 */
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

#endif
