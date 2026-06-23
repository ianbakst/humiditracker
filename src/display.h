#pragma once
#include "reading.h"

// Drives the e-paper display. All GxEPD2 / panel / font details are kept
// inside display.cpp so the rest of the project never has to deal with them.
class Display {
 public:
  void begin();                          // init + show a blank screen
  void showReading(const Reading& r);    // render and refresh
};
