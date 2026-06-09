#include "LedStrip.h"

extern WiFiClient client;
extern char DEVICE_ID;

static bool brandAlarm = false;
static uint8_t lastR = 0;
static uint8_t lastG = 0;
static uint8_t lastB = 0;
static uint8_t lastBrightness = 0;

/**
 * @brief Initialiseert de FastLED-bibliotheek en configureert de pinnen voor de LED-strip.
 * 
 */
void LedStrip::begin() 
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);//Registreer de WS2812 LED strip met de juiste pin en kleurvolgorde in FastLED.
    FastLED.clear(); // Wis eventueel achtergebleven kleurdata in het buffer, welke alle LEDs op zwart/uit zegt.
    FastLED.show(); //Toon de momenteel lege data op de led strip.
}

/**
 * @brief Verwerkt inkomende tekstcommando's van de server om de kleur en status van de LED strip aan te passen, met prioriteitsbeheer voor het brandalarm.
 * 
 * @param command Het ontvangen tekstcommando (bijv "red").
 * @param client Referentie naar de actieve WiFiClient instantie voor eventuele netwerkrespons.
 */
void LedStrip::handleCommand(String command, WiFiClient &client) 
{
  if (brandAlarm && command != "LEDBRANDOFF")
    return; //Er mag niks gebeuren met de ledstrip zolang het brandalarm actief is en deze niet opgegeven wordt met het ontvangen commando.
  else if (command == "red")
    setLight(255, 0, 0, 50); //Angstverlichtend, beter slapen.
  else if (command == "green")
    setLight(0, 255, 0, 50); //Stressverlagend.
  else if (command == "blue")
    setLight(0, 0, 255, 50); //Alertheid en betere stemming overdag.
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

/**
 * @brief Stelt de kleur en helderheid van de gehele LED-strip in door de RGB-waarden te schalen en de strip bij te werken.
 * 
 * @param r De rode kleurcomponent (0-255).
 * @param g De groene kleurcomponent (0-255).
 * @param b De blauwe kleurcomponent (0-255).
 * @param brightness Het helderheidspercentage (0-100%).
 */
void LedStrip::setLight(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) 
{
    r = (r * brightness) / 100; //Pas de helderheid met kleur toe op schaal.
    g = (g * brightness) / 100;
    b = (b * brightness) / 100;

    for (int i = 0; i < NUM_LEDS; i++)//Voor de hoeveelheid leds op de strip de kleur en geschaalde helderheid daarvoor aanpassen.
        leds[i] = CRGB(r, g, b);
    
    FastLED.show(); //Maak de verandering zichtbaar op de led strip.
}
