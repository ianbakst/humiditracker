#pragma once
#include <Arduino.h>
#include <DHTesp.h>

#include "reading.h"

// Wraps the DHT temperature/humidity sensor.
class Sensor {
 public:
  void begin();
  Reading read();

  // Human-readable status of the most recent read (for logging).
  String statusString();

 private:
  DHTesp dht_;
};
