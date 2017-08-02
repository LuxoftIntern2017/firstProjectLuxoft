#include <FS.h>
#include <Arduino.h>

/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 4

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Legit Network"
#define WLAN_PASS       "P@$$word"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "MqttLuxoft"
#define AIO_KEY         "57f5a981a9bc4a85980ea65738ad68b2"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);
/****************************** Feeds ***************************************/

// Setup a feed called 'slider' for subscribing to changes on the slider
Adafruit_MQTT_Subscribe hue = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Hue", MQTT_QOS_1);
Adafruit_MQTT_Subscribe saturation = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Saturation", MQTT_QOS_1);
Adafruit_MQTT_Subscribe value = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Value", MQTT_QOS_1);

/*************************** Sketch Code ************************************/
double hueVal=0, saturationVal=0, valueVal=0;

void huecallback(double data)
{
  //control the RGB LEDS with the data
  hueVal = data;

}
void saturationcallback(double data)
{
  //control the RGB LEDS with the data
  saturationVal = data;
}
void valuecallback(double data)
{
  //control the RGB LEDS with the data
  valueVal = data;
}

void setup() {
  Serial.begin(115200);
  delay(10);
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println();
  WiFiManager wifiManager;

  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);
  if (!wifiManager.autoConnect("Configurable ESP")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  hue.setCallback(huecallback);
  saturation.setCallback(saturationcallback);
  value.setCallback(valuecallback);

  mqtt.subscribe(&hue);
  mqtt.subscribe(&saturation);
  mqtt.subscribe(&value);
}

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets and callback em' busy subloop
  // try to spend your time here:
  mqtt.processPackets(1000);
 }

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
