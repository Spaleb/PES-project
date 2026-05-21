#ifndef LEDCHECK_H
#define LEDCHECK_H

#include <Arduino.h>

/**
 * @brief Class for controlling the built-in LED.
 */
class LedCheck {
  private:
    bool blink;
    bool ledState;
    unsigned long lastToggle;

  public:
    /**
     * @brief Constructor.
     */
    LedCheck();

    /**
     * @brief Initializes the LED.
     */
    void begin();

    /**
     * @brief Updates LED behavior.
     */
    void update();

    /**
     * @brief Handles incoming command.
     *
     * @param cmd Command character
     */
    void handleCommand(char cmd);
};

#endif
