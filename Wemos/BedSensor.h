#ifndef BEDSENSOR_H
#define BEDSENSOR_H

#include <Arduino.h>

/**
 * @brief This class is responsible for detecting pressure on the bed and
 * sending a message if the status of the recorded environment 
 * (the pressure) has changed.
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
