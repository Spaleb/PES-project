/**
 * @file RFID.h
 * @brief Header file for RFID handling using Adafruit PN532 over I2C.
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

/**
 * @class RFID
 * @brief Handles RFID/NFC card detection and UID processing using PN532.
 */
class RFID {
public:
    /**
     * @brief Constructs the RFID object.
     */
    RFID();

    /**
     * @brief Initializes I2C and PN532 module.
     */
    void begin();

    /**
     * @brief Periodically checks for RFID cards and processes UID.
     */
    void update();

private:
    /** PN532 NFC interface instance */
    Adafruit_PN532 nfc;

    /** Stores last detected UID to avoid duplicate reads */
    String lastUID = "";

    /** Indicates if the reader is ready (optional state flag) */
    bool ready = false;

    /** Timestamp of last scan in milliseconds */
    unsigned long lastScanTime = 0;
};
