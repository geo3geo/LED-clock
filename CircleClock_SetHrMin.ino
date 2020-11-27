/* Simple Hr/Min clock with auto intensity
   12 hr / 60 min display on 24LED circle
   Fine if you just need the approx time!

   Press setup button to enter setup mode, hour led will flash, press
   button to increment. no press for 3 seconds causes minutes led to flash, 
   press button to increment. No press for 3 seconds causes end of exit mode

   Geo Meadows  27Nov2020
*/

#include <FastLED.h>
#include <SoftwareSerial.h>
#define NUM_LEDS 24
#define MAX_LEDS 128
#define DATA_PIN 6 //LEDstrip
#define setupPin 5 //pushbutton
#define soundPin 4 //speaker (future option)
#define optAna A0  //light level sensor analogue input
#define COLOR_ORDER GRB

//set start time
int hr = 9;
int mins = 30;
//
//int lastsec = 0;

int button = 0;
int optValue = 0;
int optmin = 0;    //bright
int optmax = 1020; //dark
int brightness = 6;
int loopcnt;
int seccnt;
unsigned long duration;
bool sethrmin = 0;
bool sethr = 0;
bool setmin = 0;
bool newbutton = 0;
bool lastbutton = 0;

CRGB leds[NUM_LEDS];
/////////////////////////////////////////////
void setup() {
  pinMode(soundPin, OUTPUT);
  pinMode(setupPin, INPUT);
  Serial.begin(57600);
  FastLED.addLeds<WS2812, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( brightness );
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
    FastLED.show();
  }
}
/////////////////////////////////////////////
void loop() {
  uint8_t  sec = (sec + (millis() / 1000)) % 60;
  static uint8_t lastSec = 99;

  button = !digitalRead(setupPin);
  if (lastbutton == 0) {
    if (button) {
      newbutton = 1;
      lastbutton = 1;
    }
  }
  else            // lastbutton == 1
    if (!button) {
      lastbutton = 0;
    }

  if (sethrmin) {
    if (sethr) {
      if (seccnt > 3) {
        sethr = 0;
        setmin = 1;
        seccnt = 0;
      }
      if (newbutton) {
        hr = (hr + 1) % 12;
        newbutton = 0;
        seccnt = 0;;
      }
    }
    else {
      if (seccnt > 3) {
        setmin = 0;
        sethrmin = 0;
      }
      if (newbutton) {
        mins = ((mins + 2) % 60);
        newbutton = 0;
        seccnt = 0;;
      }
    }
  }
  else  {
    if (newbutton) {
      sethrmin = 1;
      sethr = 1;
      seccnt = 0;
      newbutton = 0;
    }
  }
  ////////////////////////
  if ( lastSec != sec) {
    lastSec = sec;
    seccnt++;
    if (sec == 0) {
      mins = ((mins + 1) % 60);
      if (mins == 0) {
        hr = ((hr + 1) % 12);
      }
    }
    Serial.print(hr);
    Serial.print(":");
    Serial.print(mins);
    Serial.print(".");
    Serial.print(sec);
    Serial.print("  B:");
    Serial.print(brightness);
    Serial.println(" ");
  }
  loopcnt = (loopcnt + 1) % 24;
  clearLeds();

  if ((!setmin) && (!sethr)) {
    leds[loopcnt].setRGB(0, 0, 1);
    if ((loopcnt == 0) || (loopcnt == 6) || (loopcnt == 12) || (loopcnt == 18)) {
      leds[loopcnt].setRGB(0, 8, 16);
    }
  }
  if (!setmin) {
    leds[24 * mins / 60].setRGB(0, 64, 0);
  }
  else {
    if (loopcnt && 0x01) {
      leds[24 * mins / 60].setRGB(0, 64, 0);
    }
  }

  if (!sethr) {
    leds[2 * hr].setRGB(64, 0, 0);
  }
  else {
    if (loopcnt && 0x01) {
      leds[2 * hr].setRGB(64, 0, 0);
    }
  }

  FastLED.show();
  optValue = analogRead(optAna);
  brightness = map(optValue, optmin, optmax, 100, 2);
  if (brightness < 10) brightness = 2;
  FastLED.setBrightness(brightness);
  FastLED.delay(40);  //adjust for one second sweep
}
///////////////////////////////////////////////
void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}
