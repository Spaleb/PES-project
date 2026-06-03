#include "LedCheck.h"

LedCheck::LedCheck() {
  blink = false;
  ledState = HIGH;
  lastToggle = 0;
}

void LedCheck::begin() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED uit
}

void LedCheck::handleCommand(char cmd) {
  if (cmd == '1') {        // blink
    blink = true;
  }
  else if (cmd == '2') {   // uit
    blink = false;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void LedCheck::update() {
  if (blink && millis() - lastToggle >= 1000) {
    lastToggle = millis();
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}
