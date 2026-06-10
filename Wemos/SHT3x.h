#ifndef SHT3X_H
#define SHT3X_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

/**
 * @file SHT3X.h
 * @brief Klasse voor het uitlezen van een SHT3X temperatuur- en luchtvochtigheidssensor.
 */

/**
 * @class SHT3xSensor
 * @brief Leest de SHT3X-sensor uit en bepaalt ventilatie- en verwarmingsstatus.
 */
class SHT3xSensor {
  private:
/** @brief I2C-adres van de SHT3X-sensor. */
    static const uint8_t SHT3X_ADRES = 0x44;

/** @brief Laatst gemeten temperatuur in graden Celsius. */
    float temperatuur = 0.0;
    
/** @brief Laatst gemeten luchtvochtigheid in procenten. */
    float luchtvochtigheid = 0.0;

/** @brief Geeft aan of de sensor correct werkt. */
    bool sensorOk = false;

/** @brief Tijdstip van de laatste meting in milliseconden. */
    unsigned long laatsteUpdate = 0;

/**
 * @brief Leest de sensor uit via I2C.
 * @return true als het uitlezen gelukt is, anders false.
*/
    bool leesSensor();
    
    bool brandActief = false;

  public:
  /** @brief Initialiseert de sensor en I2C-communicatie. */
    void begin();

  /** @brief Leest elke 10 seconden de sensor uit en verstuurt de meetdata. */
    void loop();

    /**
 * @brief Verwerkt een ontvangen commando voor ventilatie.
 * @param command Het ontvangen commando als String.
 * @param client WiFiClient waarmee eventueel een reactie teruggestuurd kan worden.
 */
    void handleCommand(String command, WiFiClient &client);
   /**
    * @brief Geeft de temperatuur terug.
    * @return Temperatuur in graden Celsius.
    */
    float getTemperatuur();

    /**
     * @brief Geeft de luchtvochtigheid terug.
     * @return Luchtvochtigheid in procenten.
     */
    float getLuchtvochtigheid();

    /**
     * @brief Bepaalt de ventilatiestand op basis van luchtvochtigheid.
     * @return Ventilatiestand van 0 t/m 3.
     */
    int getVentilatieStand();

    /**
     * @brief Bepaalt of de verwarming aan moet.
     * @return true als temperatuur lager is dan 20.5°C.
     */
    bool isVerwarmingAan();

    /**
     * @brief Geeft terug of de sensor correct werkt.
     * @return true als de sensor goed werkt.
     */
    bool isOk();
};

#endif
