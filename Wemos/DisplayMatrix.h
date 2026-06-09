#ifndef DISPLAY_MATRIX_H
#define DISPLAY_MATRIX_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class DisplayMatrix {
public:
    void init();
    void show(const char* text);
    void update();
    void clearBrand();
    void on();
    void off();
    void brandOn();
    void handleCommand(const String& msg, WiFiClient& client);
};

#endif
