#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <FastLED.h>
#include <ESP8266WiFi.h>

// Config
#define LED_PIN D4
#define NUM_LEDS 10

class LedStrip {
public:
    void begin();
    void update();

    // Ontvangt de tekst commando's van server
    void handleCommand(String msg, WiFiClient &client);

private:
    CRGB leds[NUM_LEDS];

    // Blijft bestaan voor lokale aansturing
    void setLight(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
};

#endif
