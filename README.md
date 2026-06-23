# humiditracker

ESP8266 (Wemos/LOLIN D1 mini) temperature + humidity tracker that reads a
DHT11 sensor and displays the values on a 4-color e-paper screen.

Built with [PlatformIO](https://platformio.org/) so the whole project —
editing, compiling, uploading, and the serial monitor — lives in VSCode.

## Hardware

| Part            | Detail                                   |
| --------------- | ---------------------------------------- |
| MCU board       | Wemos/LOLIN D1 mini (ESP8266)            |
| Sensor          | DHT11 temp/humidity                      |
| Display         | 1.54" 4-color e-paper (152x152), GxEPD2   |

### Wiring

DHT11:

| DHT11 | D1 mini       |
| ----- | ------------- |
| DATA  | D6 (GPIO12)   |
| VCC   | 3V3           |
| GND   | GND           |

E-paper (hardware SPI; wiring follows GxEPD2's suggested D1 mini layout):

| E-paper      | D1 mini      |
| ------------ | ------------ |
| BUSY         | D2 (GPIO4)   |
| RST          | D4 (GPIO2)   |
| DC           | D3 (GPIO0)   |
| CS           | D8 (GPIO15)  |
| CLK / SCK    | D5 (GPIO14)  |
| DIN / MOSI   | D7 (GPIO13)  |
| VCC          | 3V3          |
| GND          | GND          |

Pins are defined in [`include/config.h`](include/config.h).

## Working in VSCode

PlatformIO (extension `platformio.platformio-ide`) is already installed. Open
this folder in VSCode and use the PlatformIO toolbar at the bottom:

- **✓ Build** — compile
- **→ Upload** — compile + flash the board over USB
- **🔌 Monitor** — open the serial monitor (115200 baud)

### From the command line

The PlatformIO CLI lives at `~/.platformio/penv/bin/pio` (not on `PATH` by
default):

```bash
~/.platformio/penv/bin/pio run            # build
~/.platformio/penv/bin/pio run -t upload  # build + flash
~/.platformio/penv/bin/pio device monitor # serial monitor
```

Tip: `alias pio=~/.platformio/penv/bin/pio` in your shell profile.

## Customizing

- **Different board** — change `board = d1_mini` in
  [`platformio.ini`](platformio.ini) (e.g. `nodemcuv2`).
- **Different e-paper panel** — change the `EpdPanel` typedef in
  [`src/main.cpp`](src/main.cpp); valid 4-color classes are listed in the
  comment right above it.
- **Different sensor** — change `DHT_MODEL` in
  [`include/config.h`](include/config.h) (e.g. `DHTesp::DHT22`).

## Layout

```
platformio.ini      project + board + library config
include/config.h    pin assignments and timing
src/main.cpp        firmware
legacy/             original Arduino IDE sketches (reference only, not built)
```

The serial output keeps the original JSON line format, e.g.
`{"humidity": 41.0, "temp": 72.5, "unit": "F"}`.
