#pragma once
//
// Hardware configuration for the humiditracker (Wemos/LOLIN D1 mini).
// Pin names (D2, D5, ...) are the silkscreen labels on the D1 mini.
//

// ---------------------------------------------------------------------------
// Temp/humidity sensor (DHTesp)
// ---------------------------------------------------------------------------
#define DHT_PIN   D6                 // GPIO12 (also MISO, but the e-paper is
                                     // write-only so the pin is free to reuse)
#define DHT_MODEL DHTesp::DHT11

// ---------------------------------------------------------------------------
// E-paper display (GxEPD2, 4-color) on hardware SPI.
//
// SPI clock/data are fixed by the ESP8266 hardware SPI peripheral:
//   SCK  = D5 (GPIO14)
//   MOSI = D7 (GPIO13)   <- called "DIN" on most e-paper modules
//
// The four control pins below follow GxEPD2's own suggested wiring for the
// WeMos D1 mini (see the example headers in the GxEPD2 library).
// ---------------------------------------------------------------------------
#define EPD_CS    D8                 // GPIO15
#define EPD_DC    D3                 // GPIO0
#define EPD_RST   D4                 // GPIO2
#define EPD_BUSY  D2                 // GPIO4

// How often to refresh things. The e-paper is slow and wears with each
// full refresh, so update it far less often than we poll the sensor.
#define SENSOR_INTERVAL_MS   2000UL
#define DISPLAY_INTERVAL_MS  60000UL
