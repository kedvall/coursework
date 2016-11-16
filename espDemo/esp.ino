//Code for IOT Button
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
const int LED_PIN = 14; // this is the LED pin

#define IFTTT_URL "https://maker.ifttt.com/use/bk-FBOIZyOZKVylOaHW1YL"

void setupWifi(void);
bool postToIfttt(void);

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    // setup the led pin and turn it off
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    setupWifi();
}

void loop() {
    if(WiFi.status() == WL_CONNECTED)
    {
      if (postToIfttt())
      {
        digitalWrite(LED_PIN, HIGH);
      }
    }

    delay(2000); // wait for 10 seconds for the next event
    digitalWrite(LED_PIN, LOW); // turn off the LED
    WiFi.forceSleepBegin();
    WiFi.disconnect();
    while(1)
    {
      __asm__("nop");
    }
}


bool postToIfttt()
{
        HTTPClient http;

        uint httpCode;

        http.begin(IFTTT_URL);
        http.addHeader("Content-Type", "application/json");
        httpCode = http.POST(""); // to add more data

        http.end();

        if (httpCode == 200)
        {
          Serial.println("happy");
          return true;
        }
        else
        {
          Serial.print(httpCode); //print error code to terminal
          return false;
        }

}


void setupWifi()
{
      WiFi.begin("SSID", "PASSWORD");

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
