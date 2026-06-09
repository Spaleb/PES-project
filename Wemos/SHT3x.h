#ifndef SHT3X_H
#define SHT3X_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class SHT3xSensor {
  private:
    static const uint8_t SHT3X_ADRES = 0x44;

    float temperatuur = 0.0;
    float luchtvochtigheid = 0.0;
    bool sensorOk = false;
    unsigned long laatsteUpdate = 0;

    bool leesSensor();
    
    bool brandActief = false;

  public:
    void begin();
    void loop();
    void handleCommand(String command, WiFiClient &client);

    float getTemperatuur();
    float getLuchtvochtigheid();

    int getVentilatieStand();
    bool isVerwarmingAan();

    bool isOk();
};

#endif
