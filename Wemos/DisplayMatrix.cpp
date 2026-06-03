#include "DisplayMatrix.h"

#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN D7
#define CLK_PIN  D5
#define CS_PIN   D8

static MD_Parola display(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

static bool active = false;
static char currentText[64];

void DisplayMatrix::init() {
  display.begin();
  display.displayClear();
}

void DisplayMatrix::show(const char* text) {
  memset(currentText, 0, sizeof(currentText));
  strncpy(currentText, text, sizeof(currentText) - 1);
  display.displayClear();
  display.displayText(
    currentText,
    PA_CENTER,
    50,
    1000,
    PA_SCROLL_LEFT,
    PA_SCROLL_LEFT
  );
  display.displayReset();
}

void DisplayMatrix::update() {
  if (!active) return;
  if (display.displayAnimate()) {
    display.displayReset();
  }
}

void DisplayMatrix::clearBrand() {
  display.displayClear();
  active = false;
}

void DisplayMatrix::on() {
  active = true;
}

void DisplayMatrix::off() {
  active = false;
  display.displayClear();
  display.displayReset();
}

void DisplayMatrix::handleCommand(const String& msg, WiFiClient& client) {
  if (msg == "MATRIXClear") {
    clearBrand();
  } else if (msg == "MATRIXON") {
    on();
  } else if (msg == "MATRIXOFF") {
    off();
  }
}
