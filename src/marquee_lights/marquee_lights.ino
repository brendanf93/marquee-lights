#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "config.h"
#include "coordinates.h"

const char* ssid = NETWORK;
const char* password = PASS;

long currentMillis = 0;
long previousMillis = 0;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 821

float offset = 0;

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
class Pixel
{
  public:
    float x;
    float y;
    HsbColor colour;
};


Pixel L[177];
Pixel O[160];
Pixel V[233];
Pixel E[251];

Pixel pixels[LED_COUNT];

// Offset values are max from previous + previous offset + offset amount
float o_offset = 32 + 5;
float v_offset = 30 + o_offset + 5;
float e_offset = 31.75 + v_offset + 5;

void letter_setup()
{
  // L
  for(int i = 0; i < 177; i++)
  {
    L[i].x = XL[i];
    L[i].y = YL[i];
  }

  // O
  for(int i = 0; i < 160; i++)
  {
    O[i].x = XO[i] + o_offset;
    O[i].y = YO[i];
  }

  // V
  for(int i = 0; i < 233; i++)
  {
    V[i].x = XV[i] + v_offset;
    V[i].y = YV[i];
  }

  // E
  for(int i = 0; i < 251; i++)
  {
    E[i].x = XE[i] + e_offset;
    E[i].y = YE[i];
  }
  
//  for (int i = 0; i < 57; i++)
//  {
//    L[i].x = 0;
//    L[i].y = i;
//  }
//  for (int i = 57; i < 66; i++)
//  {
//    L[i].x = i - 56;
//    L[i].y = 56;
//  }
//  for (int i = 66; i < 114; i++)
//  {
//    L[i].x = 10;
//    L[i].y = 56 - (i - 66);
//  }
//  for (int i = 114; i < 136; i++)
//  {
//    L[i].x = 10 + (i - 114);
//    L[i].y = 8;
//  }
//  for (int i = 136; i < 145; i++)
//  {
//    L[i].x = 32;
//    L[i].y = 8 - (i - 136);
//  }
//  for (int i = 145; i < LED_COUNT; i++)
//  {
//    L[i].x = 32 - (i - 145);
//    L[i].y = 0;
//  }
//
//  // O
//  for (int i = 0; i < 49; i++)
//  {
//    //
//  }
//  for (int i = 49; i < 53; i++)
//  {
//    //
//  }
//  for (int i = 53; i < 78; i++)
//  {
//    //
//  }
//  for (int i = 78; i < 82; i++)
//  {
//    //
//  }
//  for (int i = 82; i < 131; i++)
//  {
//    //
//  }
//  for (int i = 131; i < 135; i++)
//  {
//    //
//  }
//  for (int i = 135; i < 160; i++)
//  {
//    //
//  }
//  for (int i = 160; i < 164; i++)
//  {
//    //
//  }
//
//  // V
//  for (int i = 0; i < 58; i++)
//  {
//    //
//  }
//  for (int i = 58; i < 68; i++)
//  {
//    //
//  }
//  for (int i = 68; i < 112; i++)
//  {
//    //
//  }
//  for (int i = 112; i < 155; i++)
//  {
//    //
//  }
//  for (int i = 155; i < 165; i++)
//  {
//    //
//  }
//  for (int i = 165; i < 224; i++)
//  {
//    //
//  }
//  for (int i = 224; i < 233; i++)
//  {
//    //
//  }
//
//  // E
//  for (int i = 0; i < 57; i++)
//  {
//    //
//  }
//  for (int i = 57; i < 68; i++)
//  {
//    //
//  }
//  for (int i = 68; i < 89; i++)
//  {
//    //
//  }
//  for (int i = 89; i < 99; i++)
//  {
//    //
//  }
//  for (int i = 99; i < 119; i++)
//  {
//    //
//  }
//  for (int i = 119; i < 131; i++)
//  {
//    //
//  }
//  for (int i = 131; i < 149; i++)
//  {
//    //
//  }
//  for (int i = 149; i < 159; i++)
//  {
//    //
//  }
//  for (int i = 159; i < 177; i++)
//  {
//    //
//  }
//  for (int i = 177; i < 189; i++)
//  {
//    //
//  }
//  for (int i = 189; i < 209; i++)
//  {
//    //
//  }
//  for (int i = 209; i < 218; i++)
//  {
//    //
//  }
//  for (int i = 218; i < 251; i++)
//  {
//    //
//  }

}




void setup()
{
  letter_setup();
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  strip.Begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.Show();  // Turn OFF all pixels ASAP
}

float val = 0.0005;
float c;

void loop()
{
  ArduinoOTA.handle();

  currentMillis = millis();

  for(int i = 0; i < 177; i++)
  {
    c = L[i].x/32.0; //140.75 // 140.75 is total x width with offsets
    strip.SetPixelColor(i, HsbColor(c, 1, 0.2));
  }
  strip.Show();

//  if (currentMillis - 10 > previousMillis)
//  {
//    for (int i = 0; i < 233; i++)
//    {
//      strip.SetPixelColor(i,HsbColor(c,1,0.5));
//    }
//    strip.Show();
//
//    c += val;
//
//    if(c > 0.9 || c < 0.76)
//    {
//      val *= -1;
//    }
//    
//  }

  //  for(int i = 0; i < LED_COUNT; i++)
  //  {
  //    float c = L[i].x/44 + offset;
  //    if(c > 1)
  //    {
  //      c-=1;
  //    }
  //    Serial.println(c);
  //    HsbColor colour = HsbColor(c, 1, 0.2);
  //    L[i].colour = colour;
  //    strip.SetPixelColor(i, colour);
  //
  //  }
  //  offset += 0.01;
  //  if(offset >= 1)
  //  {
  //    offset = 0;
  //  }


}
