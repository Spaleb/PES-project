#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

/**
 * @brief 
 * Deze klasse beheert de RFID-module. Het maakt gebruik van de Adafruit_PN532-library om te communiceren met de PN532-module via I2C.
 * 
 */
class RFID {
public:

    RFID();
    void begin();
    void update();

private:
    Adafruit_PN532 nfc;
    String lastUID = "";

    bool ready = false;
    unsigned long lastScanTime = 0;
};
