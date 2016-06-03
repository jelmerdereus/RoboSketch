/* DS1307 RTC and WS2812 12x RGB LED ring as a watch */

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

// WS2812 "neopixel" 12x RGB LED functions
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 8 // digital 8
#define LED_COUNT 12 // 12 WS2812 RGB LEDs

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_DS1307 rtc;

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

/*#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
*/
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //
    //// This line sets the RTC with an explicit date & time, for example to set
    //// January 21, 2014 at 3am you would call:
    //// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  leds.begin();  // Call this to start up the LED strip.
  leds.show();   // ...but the LEDs don't actually update until you call this.
}

void loop () {
    DateTime now = rtc.now();

    clearLEDs();
    
    Serial.print(now.hour() % 12, DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.println();

    uint16_t hours = now.hour() % 12;
    uint16_t minutes = now.minute() / 5;

    if (hours == minutes) {    // overlapping hours and minutes? purple
      leds.setPixelColor(now.hour() % 12, 204, 0, 102);
    } else {
      leds.setPixelColor(now.hour() % 12, 253, 6, 6); // hours red
      leds.setPixelColor(now.minute() / 5, 6, 6, 253);  // minutes blue
    }

    leds.setBrightness(10);
    leds.show();
    delay(15000);
}

// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

