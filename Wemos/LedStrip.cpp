#include "LedStrip.h"

extern WiFiClient client;
extern char DEVICE_ID;

static bool brandAlarm = false;
static uint8_t lastR = 0;
static uint8_t lastG = 0;
static uint8_t lastB = 0;
static uint8_t lastBrightness = 0;

void LedStrip::begin() 
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.show();
}

void LedStrip::handleCommand(String command, WiFiClient &client) 
{
 // Serial.println("Received LED command: " + command);

  if (brandAlarm && command != "LEDBRANDOFF")
  {
    //Serial.println("Brandalarm actief, lichtstatus mag niet veranderen!"); 
    return; //Er mag niks gebeuren met de ledstrip zolang het brandalarm actief is en deze niet opgegeven wordt.
  }
  else if (command == "red")
    setLight(255, 0, 0, 50); //Angstverlichtend, beter slapen.
  else if (command == "green")
    setLight(0, 255, 0, 50); //Stressverlagend.
  else if (command == "blue")
    setLight(0, 0, 255, 50); //Blauw licht voor alertheid en betere stemming overdag.
  else if (command == "LEDon")
    setLight(255, 255, 255, 75); //Wit licht als aangegeven wordt dat de lampen aan moeten.
  else if (command == "LEDoff")
    setLight(255, 255, 255, 0); //Als brightness 0 is staan de lampen uit.
  else if (command == "LEDBRANDON")
  {
    brandAlarm = true; //Dan is het brandalarm actief.

    //Worden de laatste waarden van de ledstrip opgeslagen, voor wanneer het brandalarm uiteindelijk weer zal worden opgeheven.
    lastR = leds[0].r;
    lastG = leds[0].g;
    lastB = leds[0].b;
    lastBrightness = 50;

    setLight(255,255,255,100); //Zet lichten voor brand op wit en maximale helderheid.
  } 
  else if (command == "LEDBRANDOFF")
  {
    brandAlarm = false; //Het brandalarm is niet meer actief.
    setLight(lastR, lastG, lastB, lastBrightness); //Het licht wordt gezet op de opgeslagen waarden voordat brand werd afgegeven.
  }   
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
