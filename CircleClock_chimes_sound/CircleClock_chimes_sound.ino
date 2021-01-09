/* Simple Hr/Min clock with auto intensity
   12 hr / 60 min display on 24LED circle
   Fine if you just need the approx time!

   Press setup button to enter setup mode, hour led will flash, press
   button to increment. no press for 3 seconds causes minutes led to flash,
   press button to increment. No press for 3 seconds causes end of exit mode

   On the hour and half hour a LED pattern is displayed for 20 seconds
   On the quarter hour a different pattern is displayed, for 10 seconds

   On the hour a 'chime' sequence is played, but this is inhibited at night
   (defined by low detected light level) and a series of tones sounded according
   to the current hour. During the night these are very much shorter to make them less
   intrusive.  For silent operation, simply unplug the sound generator unit!

   The sound generator used was an old earpiece of about 1K ohm.
   It was plugged into a 3.5mm audio jack wired to +5 and to the soundPin.

   The pushbutton connects bewteen setupPin and )v with a pullup resistor (10K-ish)
   bewteen soundPin and +5.

   Geo Meadows
   02 Dec 2020 - first release 
   17 Dec 2020 - hour hand advanced after half hour
   22 Dec 2020 - hour tones split into groups of 3   
*/

#include <FastLED.h>
#include <SoftwareSerial.h>
#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_C4, NOTE_A3, NOTE_G3
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 4, 4, 4, 8
};


#define NUM_LEDS 24
#define MAX_LEDS 128
#define DATA_PIN 6 //LEDstrip
#define setupPin 5 //pushbutton
#define soundPin 8 //speaker (future option)
#define optAna A0  //light level sensor analogue input
#define COLOR_ORDER GRB

//set start time
int hr = 9;
int mins = 5;
//
//int lastsec = 0;

int button = 0;
int optValue = 0;
int optmin = 0;    //bright
int optmax = 1020; //dark
int brightness = 6;
int loopcnt;
int seccnt;
int m;
int daytone = 250;
int nighttone = 50;

unsigned long duration;
bool sethrmin = 0;
bool sethr = 0;
bool setmin = 0;
bool newbutton = 0;
bool lastbutton = 0;
bool chimeTime = 0;
bool night = 0;

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
/////////////////////////////////////////////
void setup() {
  pinMode(soundPin, OUTPUT);
  pinMode(setupPin, INPUT);
  Serial.begin(57600);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  FastLED.addLeds<WS2812, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( brightness );
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
    FastLED.show();
  }
}
/////////////////////////////////////////////
void loop() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  uint8_t  sec = 58;
  sec = (sec + (millis() / 1000)) % 60;
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

  // advance hrs/mins
  if ( lastSec != sec) {
    lastSec = sec;
    seccnt++;
    if (sec == 0) {
      mins = ((mins + 1) % 60);
      if (mins == 0) {
        hr = ((hr + 1) % 12);
      }
    }
    //output debug info. every second
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

  // check if time for 'chimes'

  if ( ( (mins == 0) || (mins == 30)) && (sec < 20) ) {
    if ((mins == 0) & (sec == 0)) {
      if (night == 0) {
        playMelody();
        delay (1000);
      }
      if (hr == 0) m = 12;
      else m = hr;
      for (int n = 0; n < m; n++) {
        if (night == 1) {
          tone(soundPin, 220, nighttone);
        }
        else {
          tone(soundPin, 220, daytone);
        }
        if ((n + 1) % 3 == 0) {delay (200);}  //split into groups of 3 tones
        delay(400);
      }

    }

    currentPalette = RainbowStripeColors_p;
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
    FastLED.delay(20);
  }

  else if ( ( (mins == 15) || (mins == 45)) && (sec < 10) ) {
    currentPalette = RainbowColors_p;
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
    FastLED.delay(20);
  }

  else {
    //set quarter hour markers
    if ((!setmin) && (!sethr)) {
      leds[loopcnt].setRGB(0, 0, 8);
      if ((loopcnt == 0) || (loopcnt == 6) || (loopcnt == 12) || (loopcnt == 18)) {
        leds[loopcnt].setRGB(0, 8, 32);
      }
    }

    //flash minutes LED if setting minutes
    if (!setmin) {
      leds[24 * mins / 60].setRGB(0, 64, 0);
    }
    else {
      if (loopcnt && 0x01) {
        leds[24 * mins / 60].setRGB(0, 64, 0);
      }
    }

    //flash hours LED if setting hours
    if (!sethr) {
      if (mins < 30) {
        leds[2 * hr].setRGB(64, 0, 0);
      }
      else {
        leds[(2 * hr) + 1].setRGB(64, 0, 0);
      }
    }
    else {
      if (loopcnt && 0x01) {
        leds[2 * hr].setRGB(64, 0, 0);
      }
    }
    FastLED.show();
  }

  optValue = analogRead(optAna);
  brightness = map(optValue, optmin, optmax, 100, 2); //adjust according to light sensor
  if (brightness < 50) {
    night = 1;
  }
  else {
    night = 0;
  }
  if (brightness < 20) brightness = 2;
  FastLED.setBrightness(brightness);
  FastLED.delay(40);  //adjust for one second sweep
}
//////////////////////////////////////////////////////////////

void playMelody() {
  for (int thisNote = 0; thisNote < 4; thisNote++) {      //no. of notes

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(soundPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void blip(int t) {
  digitalWrite(soundPin, LOW);
  delay(t);
  digitalWrite(soundPin, HIGH);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  uint8_t brightness = 25;
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}
