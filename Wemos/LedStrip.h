/**
 * @file Doxygen
 * @author your name (you@domain.com)
 * @brief Header-bestand voor de LedStrip klasse die een WS2812B FastLED strip aansturt via WiFi-commando's op een ESP8266.
 * @version 1.
 * @date 2026-06-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <FastLED.h>
#include <ESP8266WiFi.h>

// Config
#define LED_PIN D4
#define NUM_LEDS 10

/**
 * @brief Klasse voor het beheren en aansturen van een RGB LED strip o.b.v. inkomende netwerkcommando's.
 * 
 */
class LedStrip {
public:
    /**
     * @brief Initialiseert de FastLED library en configureert de pinnen voor de LED strip.
     * 
     */
    void begin();
    /**
     * @brief Verwerkt  updates van de LED strip, welke continue in de loop moet worden aangeroepen.
     * 
     */
    void update();
    /**
     * @brief Ontvangt, ontleedt en voert tekstcommando's uit die afkomstig zijn van de server via een WiFi-verbinding.
     * 
     * @param msg Het ontvangen tekstcommando van de server.
     * @param client De actieve WiFi-client waarmee eventueel een antwoord kan worden teruggestuurd.
     */
    void handleCommand(String msg, WiFiClient &client);// Ontvangt de tekst commando's van server

private:
    /**
     * @brief Array die de RBG kleurgegevens per individuele LED op de strip bevat.
     * 
     */
    CRGB leds[NUM_LEDS];
    /**
     * @brief Stelt de kleur en helderheid van de gehele LED-strip lokaal in.
     * 
     * @param r De rode kleurcomponent (0-255).
     * @param g De groene kleurcomponent (0-255).
     * @param b De blauwe kleurcomponent (0-255).
     * @param brightness De helderheid van de LED strip.
     */
    void setLight(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
};
#endif
