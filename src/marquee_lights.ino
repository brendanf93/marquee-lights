#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "config.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 300

float leds[LED_COUNT];

uint8_t prefix[] = {'s','n','d'};

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
  RgbColor StartingColor;   // the color the animation starts at
  RgbColor EndingColor;     // the color the animation will end at
  AnimEaseFunction Easeing; // the acceleration curve it will use
};

MyAnimationState animationState[PixelCount];
// one entry per pixel to match the animation timing manager

void AnimUpdate(const AnimationParam &param)
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

void setup()
{
  // Start a serial connection
  Serial.begin(115200);

  strip.Begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.Show();  // Turn OFF all pixels ASAP
}

void loop()
{
  for(uint8_t i = 0; i < sizeof(prefix); i++)
  {
    waitLoop: while(!Serial.available());;

    if(prefix[i] == Serial.read()) continue;

    i = 0;
    goto waitLoop;
  }

  // Set all led values to zero
  memset(leds, 0, LED_COUNT*sizeof(byte));

  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    byte brightness;

    while(!Serial.available())
    brightness = Serial.read();

    strip.SetPixelColor(i, HsbColor(1, 1, (float)(brightness/100)));
  }
  strip.Show();

}