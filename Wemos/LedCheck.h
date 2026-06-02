#ifndef LEDCHECK_H
#define LEDCHECK_H

#include <Arduino.h>

class LedCheck {
  private:
    bool blink;
    bool ledState;
    unsigned long lastToggle;

  public:
    LedCheck();
    void begin();
    void update();
    void handleCommand(char cmd);
};

#endif
