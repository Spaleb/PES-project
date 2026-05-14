#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

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
