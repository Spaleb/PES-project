#include "DisplayMatrix.h"

#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

extern WiFiClient client;
extern char DEVICE_ID;

#define DATA_PIN D7
#define CLK_PIN  D5
#define CS_PIN   D8

static MD_Parola display(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

static bool active = false;
static bool brandAlarmActive = false;
static bool brandTextShown = false;
static char currentText[64] = {0};

void DisplayMatrix::init() {
  display.begin();
  display.displayClear();
}

void DisplayMatrix::show(const char* text) {

  if (brandAlarmActive) {

    if (!brandTextShown) {
      brandTextShown = true;
    } 
    else {
      return; 
    }
  }

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

void DisplayMatrix::brandOn() {
  brandAlarmActive = true;
  brandTextShown = false;
  active = true;
  client.println("brand aan");
}

void DisplayMatrix::clearBrand() {
  brandAlarmActive = false;
  brandTextShown = false;
  active = false;

  memset(currentText, 0, sizeof(currentText));

  display.displayClear();
  display.displayReset();
}

void DisplayMatrix::on() {
  if (brandAlarmActive) return;
  active = true;
}

void DisplayMatrix::off() {
  if (brandAlarmActive) return;

  active = false;
  display.displayClear();
  display.displayReset();
}

void DisplayMatrix::handleCommand(const String& msg, WiFiClient& client) {
  String command = msg;
  command.trim();   // verwijdert \n en \r

  int sep = command.indexOf(':');
  if (sep != -1) {
    command = command.substring(sep + 1);
  }

  if      (command == "MATRIXClear")   clearBrand();
  else if (command == "MATRIXON")      on();
  else if (command == "MATRIXOFF")     off();
  else if (command == "MATRIXBRANDON") brandOn();
  else                                 show(command.c_str());
}
