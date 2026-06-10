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
#define CLK_PIN D5
#define CS_PIN D8

static MD_Parola display(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

static bool active = false;
static bool brandAlarmActive = false;
static bool brandTextShown = false;
static char currentText[64] = { 0 };

/**
 * @brief 
 * Initialiseert het Matrixdisplay.
 */
void DisplayMatrix::init() {
  display.begin();
  display.displayClear();
}

/**
 * @brief 
 * Toont de opgegeven tekst op het Matrixdisplay.
 * @param text De tekst om weer te geven.
 */
void DisplayMatrix::show(const char* text) {

  if (brandAlarmActive) {

    if (!brandTextShown) {
      brandTextShown = true;
    } else {
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
    PA_SCROLL_LEFT);
  display.displayReset();
}

/**
 * @brief 
 * Deze functie wordt regelmatig aangeroepen in de hoofdloop. Als het display actief is, wordt de animatie van het display bijgewerkt. 
 * 
 */
void DisplayMatrix::update() {
  if (!active) return;

  if (display.displayAnimate()) {
    display.displayReset();
  }
}

/**
 * @brief 
 * Deze functie activeert het brandalarm op het Matrixdisplay.
 */
void DisplayMatrix::brandOn() {
  brandAlarmActive = true;
  brandTextShown = false;
  active = true;
}

/**
 * @brief 
 * Deze functie cleart het brandalarm op het Matrixdisplay.
 */
void DisplayMatrix::clearBrand() {
  brandAlarmActive = false;
  brandTextShown = false;
  active = false;

  memset(currentText, 0, sizeof(currentText));

  display.displayClear();
  display.displayReset();
}

/**
 * @brief 
 * Deze functie zet het Matrixdisplay aan.
 */
void DisplayMatrix::on() {
  if (brandAlarmActive) return;
  active = true;
}

/**
 * @brief 
 * Deze functie zet het Matrixdisplay uit.
 */
void DisplayMatrix::off() {
  if (brandAlarmActive) return;

  active = false;
  display.displayClear();
  display.displayReset();
}
/**
 * @brief 
 * Deze functie verwerkt inkomende commando's voor het Matrixdisplay. Het controleert het commando en voert de bijbehorende actie uit.
 * 
 * @param msg 
 * @param client 
 */
void DisplayMatrix::handleCommand(const String& msg, WiFiClient& client) {
  String command = msg;
  command.trim();

  int sep = command.indexOf(':');
  if (sep != -1) {
    command = command.substring(sep + 1);
  }

  if (command == "MATRIXClear") clearBrand();
  else if (command == "MATRIXON") on();
  else if (command == "MATRIXOFF") off();
  else if (command == "MATRIXBRANDON") brandOn();
  else show(command.c_str());
}
