#include "DisplayMatrix.h"

#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN D7
#define CLK_PIN  D5
#define CS_PIN   D8

static MD_Parola display(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

static bool active = false;

static char currentText[64];

// init
void displayInit() {
  display.begin();
  display.displayClear();
}

void displayShow(const char* text) {

  active = true;

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

void displayUpdate() {

  if (!active) return;

  if (display.displayAnimate()) {
    display.displayReset();
  }
}
