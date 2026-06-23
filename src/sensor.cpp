#include "sensor.h"

#include "config.h"

void Sensor::begin() { dht_.setup(DHT_PIN, DHT_MODEL); }

Reading Sensor::read() {
  Reading r;
  const float humidity = dht_.getHumidity();
  const float tempF = dht_.getTemperature() * 9.0f / 5.0f + 32.0f;
  if (dht_.getStatus() == DHTesp::ERROR_NONE) {
    r.humidity = humidity;
    r.tempF = tempF;
    r.valid = true;
  }
  return r;
}

String Sensor::statusString() { return dht_.getStatusString(); }
