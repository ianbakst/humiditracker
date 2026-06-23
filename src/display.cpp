#include "display.h"

#include <GxEPD2_4C.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "config.h"

// Which 4-color panel do you have? This is set to the 1.54" 152x152 (GDEM0154F51H).
// Swap this one typedef for your panel; valid 4-color classes in this version:
//   GxEPD2_154c_GDEM0154F51H   (1.54")  <-- current
//   GxEPD2_213c_GDEY0213F51    (2.13")
//   GxEPD2_266c_GDEY0266F51H   (2.66")
//   GxEPD2_290c_GDEY029F51H    (2.9")
//   GxEPD2_350c_GDEM035F51     (3.5")
//   GxEPD2_420c_GDEY0420F51    (4.2")
//   GxEPD2_579c_GDEY0579F51    (5.79")
//   GxEPD2_750c_GDEM075F52     (7.5")
//   GxEPD2_1160c_GDEY116F51    (11.6")
using EpdPanel = GxEPD2_154c_GDEM0154F51H;

// Cap the page buffer so it fits ESP8266 RAM. 4-color = 2 bits/pixel, so a
// row is WIDTH/4 bytes; GxEPD2 pages the draw if the full panel won't fit.
#define MAX_DISPLAY_BUFFER_SIZE 16000UL
#define MAX_HEIGHT(EPD)                                                        \
  (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 4)                   \
       ? EPD::HEIGHT                                                           \
       : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 4))

static GxEPD2_4C<EpdPanel, MAX_HEIGHT(EpdPanel)> epd(
    EpdPanel(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

// Layout tuned for the 1.54" 152x152 panel.
static void draw(const Reading& r) {
  epd.setRotation(0);  // 152x152, square
  epd.setFullWindow();
  epd.firstPage();
  do {
    epd.fillScreen(GxEPD_WHITE);

    epd.setFont(&FreeSansBold9pt7b);
    epd.setTextColor(GxEPD_BLACK);
    epd.setCursor(6, 18);
    epd.print("Humiditracker");
    epd.drawLine(6, 26, 146, 26, GxEPD_BLACK);

    char buf[24];

    epd.setFont(&FreeSansBold18pt7b);
    epd.setTextColor(GxEPD_RED);
    if (r.valid) {
      snprintf(buf, sizeof(buf), "%.1f F", r.tempF);
    } else {
      snprintf(buf, sizeof(buf), "--.- F");
    }
    epd.setCursor(8, 90);
    epd.print(buf);

    epd.setFont(&FreeSansBold12pt7b);
    epd.setTextColor(GxEPD_BLACK);
    if (r.valid) {
      snprintf(buf, sizeof(buf), "%.0f%% RH", r.humidity);
    } else {
      snprintf(buf, sizeof(buf), "-- %% RH");
    }
    epd.setCursor(8, 130);
    epd.print(buf);
  } while (epd.nextPage());
}

void Display::begin() {
  epd.init(115200);  // pass baud so GxEPD2 can log on the same Serial
  showReading(Reading{});
}

void Display::showReading(const Reading& r) {
  draw(r);
  epd.hibernate();  // lowest power; image is retained
}
