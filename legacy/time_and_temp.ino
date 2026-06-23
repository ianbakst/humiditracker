#include <ESP8266WiFi.h>
#include <GxEPD2_4C.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include "DHTesp.h"
#include <time.h>


// ===== USER SETTINGS =====
const char* WIFI_SSID = "PorterSquared";
const char* WIFI_PASS = "theos_support_humans";
const uint32_t SLEEP_MINUTES = 10;
// Eastern Time (Massachusetts) with automatic DST
const char* TZ_INFO = "EST5EDT,M3.2.0,M11.1.0";
// =========================

// Display pins
#define EPD_BUSY D1
#define EPD_RST  D3   // moved from D0
#define EPD_DC   D2
#define EPD_CS   D4

// DHT11
#define DHTPIN  D6
#define DHTTYPE DHTesp::DHT11
DHTesp dht;

GxEPD2_4C<GxEPD2_154c_GDEM0154F51H, GxEPD2_154c_GDEM0154F51H::HEIGHT> display(
  GxEPD2_154c_GDEM0154F51H(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

void setup() {
  Serial.begin(115200);
  Serial.println("\nWaking up...");
  dht.setup(DHTPIN, DHTTYPE);
  // --- Read sensor first (DHT11 needs ~1s after power-up) ---
  delay(1500);
  float tempC = dht.getTemperature();
  float humidity = dht.getHumidity();
  bool sensorOK = !isnan(tempC) && !isnan(humidity);
  float tempF = sensorOK ? tempC * 9.0 / 5.0 + 32.0 : 0;

  // --- Connect WiFi and get time ---
  bool timeOK = false;
  struct tm timeinfo;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  uint32_t wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 15000) {
    delay(250);
  }
  if (WiFi.status() == WL_CONNECTED) {
    configTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");
    uint32_t ntpStart = millis();
    time_t now = 0;
    while (now < 1700000000 && millis() - ntpStart < 10000) {
      delay(250);
      now = time(nullptr);
    }
    if (now >= 1700000000) {
      localtime_r(&now, &timeinfo);
      timeOK = true;
    }
  }
  WiFi.disconnect(true);   // radio off, save power while drawing
  WiFi.mode(WIFI_OFF);

  // --- Draw ---
  char timeStr[6] = "--:--";
  char dateStr[12] = "";
  if (timeOK) {
    strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    strftime(dateStr, sizeof(dateStr), "%b %d", &timeinfo);
  }

  display.init(115200);
  display.setRotation(1);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    // Time, large, top
    display.setFont(&FreeMonoBold18pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(45, 40);
    display.print(timeStr);

    // Date
    display.setFont(&FreeMonoBold12pt7b);
    display.setCursor(60, 70);
    display.print(dateStr);

    // Temperature in red
    display.setFont(&FreeMonoBold18pt7b);
    display.setTextColor(GxEPD_RED);
    display.setCursor(30, 125);
    if (sensorOK) {
      display.print(tempF, 1);
      display.print(" F");
    } else {
      display.print("--.-");
    }

    // Humidity in yellow... actually black reads better
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(30, 170);
    if (sensorOK) {
      display.print(humidity, 0);
      display.print(" %RH");
    } else {
      display.print("-- %RH");
    }
  } while (display.nextPage());

  display.hibernate();  // put display in deep sleep too

  // --- Deep sleep ---
  Serial.println("Sleeping for 10 minutes...");
  ESP.deepSleep(SLEEP_MINUTES * 60ULL * 1000000ULL);
}

void loop() {
  // never reached - deep sleep restarts from setup()
}