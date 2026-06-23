// humiditracker — ESP8266 (Wemos D1 mini) + DHT11 + 4-color e-paper
//
// Reads temperature/humidity and shows them on the e-paper. Serial still
// emits the JSON line the original sketch did, so downstream logging keeps
// working. The sensor and display details live in their own modules; this
// file just wires them together and handles timing.

#include <Arduino.h>

#include "config.h"
#include "display.h"
#include "sensor.h"

static Sensor sensor;
static Display display;

static unsigned long lastSensorRead = 0;
static unsigned long lastDisplayUpdate = 0;
static Reading latest;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println();
  Serial.println("Humiditracker starting");
  Serial.println("-------------------------------------");

  sensor.begin();
  display.begin();

  // Force a sensor read + display refresh on the first loop iterations.
  lastSensorRead = millis() - SENSOR_INTERVAL_MS;
  lastDisplayUpdate = millis() - DISPLAY_INTERVAL_MS;
}

void loop() {
  const unsigned long now = millis();

  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    lastSensorRead = now;
    Reading r = sensor.read();
    if (r.valid) {
      latest = r;
      Serial.printf("{\"humidity\": %.1f, \"temp\": %.1f, \"unit\": \"F\"}\n",
                    r.humidity, r.tempF);
    } else {
      Serial.printf("DHT read error: %s\n", sensor.statusString().c_str());
    }
  }

  if (now - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
    lastDisplayUpdate = now;
    display.showReading(latest);
  }
}
