#ifndef DISPLAY_MATRIX_H
#define DISPLAY_MATRIX_H

#include <Arduino.h>

/**
 * @file DisplayMatrix.h
 * @brief Interface for controlling the LED matrix display.
 */

/**
 * @brief Initializes the display.
 */
void displayInit();

/**
 * @brief Handles incoming commands.
 *
 * @param cmd Command character
 */
void displayHandleCommand(char cmd);

/**
 * @brief Updates the display.
 */
void displayUpdate();

#endif
