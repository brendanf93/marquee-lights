#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "config.h"

const char* ssid = NETWORK;
const char* password = PASS;

long currentMillis = 0;
long previousMillis = 0;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 300

// Declare our NeoPixel strip object:
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_PIN);

NeoPixelAnimator animations(LED_COUNT, NEO_CENTISECONDS);
// setup() function -- runs once at startup --------------------------------
#if defined(NEOPIXEBUS_NO_STL)
// for AVR, you need to manage the state due to lack of STL/compiler support
// for Esp8266 you can define the function using a lambda and state is created for you
// see below for an example
struct MyAnimationState
{
  RgbColor StartingColor;  // the color the animation starts at
  RgbColor EndingColor; // the color the animation will end at
  AnimEaseFunction Easeing; // the acceleration curve it will use
};

MyAnimationState animationState[PixelCount];
// one entry per pixel to match the animation timing manager

void AnimUpdate(const AnimationParam& param)
{
  // first apply an easing (curve) to the animation
  // this simulates acceleration to the effect
  float progress = animationState[param.index].Easeing(param.progress);

  // this gets called for each animation on every time step
  // progress will start at 0.0 and end at 1.0
  // we use the blend function on the RgbColor to mix
  // color based on the progress given to us in the animation
  RgbColor updatedColor = RgbColor::LinearBlend(
                            animationState[param.index].StartingColor,
                            animationState[param.index].EndingColor,
                            progress);
  // apply the color to the strip
  strip.SetPixelColor(param.index, updatedColor);
}
#endif


void setup() {
  // Start a serial and WiFi connection
  Serial.begin(115200);

  strip.Begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.Show();            // Turn OFF all pixels ASAP
  
}

float offset = 0;

void loop() {
    
  currentMillis = millis();

  if (currentMillis - 10 > previousMillis)
  {
    for (int i = 0; i < LED_COUNT; i ++)
    {
      float hue = (float)i / LED_COUNT + offset;


      if (hue > 1)
      {
        hue -= 1;
      }
      strip.SetPixelColor(i, HsbColor(hue, 1, 1));
    }
    strip.Show();

    offset += 0.001;

    if (offset > 1)
    {
      offset = 0;
    }

    previousMillis = currentMillis;
  }
}
