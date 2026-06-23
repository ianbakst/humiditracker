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
// These deliberately AVOID the ESP8266 boot-strapping pins. GPIO0/2/15 must
// sit at specific levels at reset, and an attached display can hold them in the
// wrong state, blocking boot/flashing. GPIO2 (D4) is also the onboard LED.
// Only DC stays on GPIO0, which is safe because it idles high (internal pull-up)
// and nothing external pulls it during flashing.
// ---------------------------------------------------------------------------
#define EPD_CS    D1                 // GPIO5  (was D8/GPIO15 - strap)
#define EPD_DC    D3                 // GPIO0  (safe: pulled high at boot)
#define EPD_RST   D2                 // GPIO4  (was D4/GPIO2 - strap + LED)
#define EPD_BUSY  D0                 // GPIO16 (was D2/GPIO4)

// How often to refresh things. The e-paper is slow and wears with each
// full refresh, so update it far less often than we poll the sensor.
#define SENSOR_INTERVAL_MS   2000UL
#define DISPLAY_INTERVAL_MS  60000UL
