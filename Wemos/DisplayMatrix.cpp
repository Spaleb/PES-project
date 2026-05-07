#include "DisplayMatrix.h"

#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

/**
 * @file DisplayMatrix.cpp
 * @brief Controls the LED matrix display.
 */

// CONFIG
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN D7
#define CLK_PIN  D5
#define CS_PIN   D8

// DISPLAY OBJECT
static MD_Parola display(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
static bool active = false; ///< Display active flag


/**
 * @brief Initializes the display.
 */
void displayInit() {
  display.begin();
  display.setIntensity(5);
  display.displayClear();
}


/**
 * @brief Handles incoming commands.
 *
 * @param cmd Command character
 */
void displayHandleCommand(char cmd) {

  if (cmd == '3') {
    display.displayClear();

    display.displayText(
      "VERPLEEGKUNDIGE OK",
      PA_CENTER,
      100,
      0,
      PA_SCROLL_LEFT,
      PA_SCROLL_LEFT
    );

    display.displayReset();
    active = true;
  }
}


/**
 * @brief Updates the display.
 */
void displayUpdate() {
  if (!active) return;

  if (display.displayAnimate()) {
    display.displayReset();
  }
}
