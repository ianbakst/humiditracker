// humiditracker — ESP8266 (Wemos D1 mini) + DHT11 + 4-color e-paper
//
// Reads temperature/humidity from a DHT11 and shows them on a GxEPD2
// 4-color e-paper display. Serial still emits the JSON line the original
// sketch did, so any downstream logging keeps working.

#include <Arduino.h>
#include <DHTesp.h>

// --- E-paper (GxEPD2, 4-color) ---------------------------------------------
#include <GxEPD2_4C.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "config.h"


using EpdPanel = GxEPD2_154c_GDEM0154F51H;

// Cap the page buffer so it fits ESP8266 RAM. 4-color = 2 bits/pixel, so a
// row is WIDTH/4 bytes; GxEPD2 pages the draw if the full panel won't fit.
#define MAX_DISPLAY_BUFFER_SIZE 16000UL
#define MAX_HEIGHT(EPD)                                                        \
  (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 4)                   \
       ? EPD::HEIGHT                                                           \
       : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 4))

GxEPD2_4C<EpdPanel, MAX_HEIGHT(EpdPanel)> display(
    EpdPanel(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

DHTesp dht;

static unsigned long lastSensorRead = 0;
static unsigned long lastDisplayUpdate = 0;
static float lastTempF = NAN;
static float lastHumidity = NAN;

// Layout tuned for the 1.54" 152x152 panel.
static void drawReadings(float tempF, float humidity) {
  display.setRotation(0);  // 152x152, square
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    display.setFont(&FreeSansBold9pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(6, 18);
    display.print("Humiditracker");
    display.drawLine(6, 26, 146, 26, GxEPD_BLACK);

    char buf[24];

    display.setFont(&FreeSansBold18pt7b);
    display.setTextColor(GxEPD_RED);
    if (isnan(tempF)) {
      snprintf(buf, sizeof(buf), "--.- F");
    } else {
      snprintf(buf, sizeof(buf), "%.1f F", tempF);
    }
    display.setCursor(8, 90);
    display.print(buf);

    display.setFont(&FreeSansBold12pt7b);
    display.setTextColor(GxEPD_BLACK);
    if (isnan(humidity)) {
      snprintf(buf, sizeof(buf), "-- %% RH");
    } else {
      snprintf(buf, sizeof(buf), "%.0f%% RH", humidity);
    }
    display.setCursor(8, 130);
    display.print(buf);
  } while (display.nextPage());
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println();
  Serial.println("Humiditracker starting");
  Serial.println("-------------------------------------");

  dht.setup(DHT_PIN, DHT_MODEL);

  display.init(115200);  // pass baud so GxEPD2 can log on the same Serial
  drawReadings(NAN, NAN);
  display.hibernate();  // lowest power; image is retained

  // Force a sensor read + display refresh on the first loop iterations.
  lastSensorRead = millis() - SENSOR_INTERVAL_MS;
  lastDisplayUpdate = millis() - DISPLAY_INTERVAL_MS;
}

void loop() {
  const unsigned long now = millis();

  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    lastSensorRead = now;

    float h = dht.getHumidity();
    float tF = dht.getTemperature() * 9.0f / 5.0f + 32.0f;

    if (dht.getStatus() == DHTesp::ERROR_NONE) {
      lastHumidity = h;
      lastTempF = tF;
      Serial.printf("{\"humidity\": %.1f, \"temp\": %.1f, \"unit\": \"F\"}\n",
                    h, tF);
    } else {
      Serial.printf("DHT read error: %s\n", dht.getStatusString());
    }
  }

  if (now - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
    lastDisplayUpdate = now;
    drawReadings(lastTempF, lastHumidity);
    display.hibernate();
  }
}
