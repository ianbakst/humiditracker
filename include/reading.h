#pragma once
#include <math.h>

// A single temperature/humidity measurement, passed from the sensor to
// whatever consumes it (display, serial, and later WiFi/MQTT).
struct Reading {
  float tempF = NAN;
  float humidity = NAN;
  bool valid = false;  // false if the last sensor read failed
};
