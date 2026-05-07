#include "LedCheck.h"

/**
 * @brief Construct a new Led Check:: Led Check object
 * 
 */
LedCheck::LedCheck() {
  blink = false;
  ledState = HIGH;
  lastToggle = 0;
}

/**
 * @brief Initializes the LED.
 */
void LedCheck::begin() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

/**
 * @brief Handles incoming command.
 *
 * @param cmd Command character
 */
void LedCheck::handleCommand(char cmd) {
  if (cmd == '1') {        // blink
    blink = true;
  }
  else if (cmd == '2') {   // off
    blink = false;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

/**
 * @brief Updates LED state.
 */
void LedCheck::update() {
  if (blink && millis() - lastToggle >= 1000) {
    lastToggle = millis();
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}
