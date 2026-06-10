#ifndef BEDSENSOR_H
#define BEDSENSOR_H

#include <Arduino.h>

/**
 * @brief // Klasse voor het meten van of er druk is op het bed.
 * Deze klasse maakt gebruik van een analoge sensor die de druk op het bed meet.
 * 
 */
class BedSensor {
  private:
    int sensorPin;
    int sensorValue;
    bool bedPressure;

  public:
    BedSensor(int pin);
    void update();
    bool hasChanged(bool newValue);
    bool read();
};

#endif
