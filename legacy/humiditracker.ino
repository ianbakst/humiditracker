/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#include "DHTesp.h"
// #include <ESP8266WiFi.h>      // WIFI for ESP8266
// #include <WiFiClient.h>
// #include <ESP8266mDNS.h>      // mDNS for ESP8266
// #include <ESP8266WebServer.h> // webServer for ESP8266
// #include <ArduinoJson.h>      // json to process MQTT: ArduinoJson 6.11.4
// #include <PubSubClient.h>     // MQTT: PubSubClient 2.8.0


DHTesp dht;

// const char* ssid = "PorterSquared";
// const char* password = "theos_support_humans"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
  pinMode(D5, OUTPUT);
  digitalWrite(D5, LOW);
  Serial.begin(115200);
  dht.setup(D6, DHTesp::DHT11);

  //
  // Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while (!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  // Serial.println("Running Deep Sleep Firmware!");
  // Serial.println("-------------------------------------");

  // connect();

  // int temp = analogRead(A0);

  // double degreesC = (((temp / 1024.0) * 3.2) - 0.5) * 100.0;
  // double degreesF = degreesC * 1.8 + 32;

  // Serial.println();
  // Serial.print("Temperature C: ");
  // Serial.println(degreesC);
  // Serial.print("Temperature F: ");
  // Serial.println(degreesF);
  // Serial.println();

  // reportTemp(degreesC, degreesF);

  // Serial.println("Going into deep sleep for 20 seconds");
  // ESP.deepSleep(20e6); // 20e6 is 20 microseconds
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level
  float h = dht.getHumidity();
  float t = dht.getTemperature() * 9 / 5 + 32;
  Serial.print("{\"humidity\": ");
  Serial.print(h);
  Serial.print(", \"temp\": ");
  Serial.print(t);
  Serial.print("°F");
  Serial.print("}\n");
  delay(500);                      // Wait for half a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
}



/**
   An example showing how to put ESP8266 into Deep-sleep mode
*/

// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
// #include <Losant.h>

// // WiFi credentials.
// const char* WIFI_SSID = "wifi-ssid";
// const char* WIFI_PASS = "wifi-pass";

// // Losant credentials.
// const char* LOSANT_DEVICE_ID = "device-id";
// const char* LOSANT_ACCESS_KEY = "access-key";
// const char* LOSANT_ACCESS_SECRET = "access-key";

// WiFiClientSecure wifiClient;

// LosantDevice device(LOSANT_DEVICE_ID);

// void connect() {

//   // Connect to Wifi.
//   Serial.println();
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(WIFI_SSID);

//   WiFi.begin(WIFI_SSID, WIFI_PASS);

//   // WiFi fix: https://github.com/esp8266/Arduino/issues/2186
//   WiFi.persistent(false);
//   WiFi.mode(WIFI_OFF);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(WIFI_SSID, WIFI_PASS);

//   unsigned long wifiConnectStart = millis();

//   while (WiFi.status() != WL_CONNECTED) {
//     // Check to see if
//     if (WiFi.status() == WL_CONNECT_FAILED) {
//       Serial.println("Failed to connect to WiFi. Please verify credentials: ");
//       delay(10000);
//     }

//     delay(500);
//     Serial.println("...");
//     // Only try for 5 seconds.
//     if (millis() - wifiConnectStart > 15000) {
//       Serial.println("Failed to connect to WiFi");
//       return;
//     }
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());

//   Serial.println();
//   Serial.print("Connecting to Losant...");

//   Serial.print("Authenticating Device...");
//   HTTPClient http;
//   http.begin("http://api.losant.com/auth/device");
//   http.addHeader("Content-Type", "application/json");
//   http.addHeader("Accept", "application/json");

//   /* Create JSON payload to sent to Losant
//        {
//          "deviceId": "575ecf887ae143cd83dc4aa2",
//          "key": "this_would_be_the_key",
//          "secret": "this_would_be_the_secret"
//        }
//   */

//   StaticJsonBuffer<200> jsonBuffer;
//   JsonObject& root = jsonBuffer.createObject();
//   root["deviceId"] = LOSANT_DEVICE_ID;
//   root["key"] = LOSANT_ACCESS_KEY;
//   root["secret"] = LOSANT_ACCESS_SECRET;
//   String buffer;
//   root.printTo(buffer);

//   int httpCode = http.POST(buffer);

//   if (httpCode > 0) {
//     if (httpCode == HTTP_CODE_OK) {
//       Serial.println("This device is authorized!");
//     } else {
//       Serial.println("Failed to authorize device to Losant.");
//       if (httpCode == 400) {
//         Serial.println("Validation error: The device ID, access key, or access secret is not in the proper format.");
//       } else if (httpCode == 401) {
//         Serial.println("Invalid credentials to Losant: Please double-check the device ID, access key, and access secret.");
//       } else {
//         Serial.println("Unknown response from API");
//       }
//       Serial.println("Current Credentials: ");
//       Serial.println("Device id: ");
//       Serial.println(LOSANT_DEVICE_ID);
//       Serial.println("Access Key: ");
//       Serial.println(LOSANT_ACCESS_KEY);
//       Serial.println("Access Secret: ");
//       Serial.println(LOSANT_ACCESS_SECRET);
//       return;
//     }
//   } else {
//     Serial.println("Failed to connect to Losant API.");
//     return;
//   }

//   http.end();

//   device.connectSecure(wifiClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);

//   while (!device.connected()) {
//     delay(1000);
//     Serial.print(".");
//   }

//   Serial.println("Connected!");
//   Serial.println("This device is now ready for use!");
// }

// void reportTemp(double degreesC, double degreesF) {
//   StaticJsonBuffer<200> jsonBuffer;
//   JsonObject& root = jsonBuffer.createObject();
//   root["tempC"] = degreesC;
//   root["tempF"] = degreesF;
//   device.sendState(root);
// }
