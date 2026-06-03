#include "LedStrip.h"

extern WiFiClient client;
extern char DEVICE_ID;

void LedStrip::begin() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();
    setLight(255, 255, 255, 0); //zet de lichten uit bij start up
    FastLED.show();
}

void LedStrip::handleCommand(String command, WiFiClient &client) 
{
  if (command == "red")
    setLight(255, 0, 0, 50); //Angstverlichtend, beter slapen.
  else if (command == "green")
    setLight(0, 255, 0, 50); //Stressverlagend.
  else if (command == "blue")
    setLight(0, 0, 255, 50); //Blauw licht voor alertheid en betere stemming overdag.
  else if (command == "on")
    setLight(255, 255, 255, 75); //Wit licht als aangegeven wordt dat de lampen aan moeten.
  else if (command == "off")
    setLight(255, 255, 255, 0); //Als brightness 0 is staan de lampen uit.
}

void LedStrip::setLight(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) 
{
    r = (r * brightness) / 100;
    g = (g * brightness) / 100;
    b = (b * brightness) / 100;

    for (int i = 0; i < NUM_LEDS; i++) 
        leds[i] = CRGB(r, g, b);
    
    FastLED.show();
}
