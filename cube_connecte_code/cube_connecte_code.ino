/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

# define NUM_LEDS 15
#define LEDS_TYPE WS2812B
#define RING_PIN 2
#define DHTTYPE DHT22
#define DHTPIN 4

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <DHT.h>
#include "FastLED.h"
CRGB leds[NUM_LEDS]; 
BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

int red, green, blue,lum;


void setup()
{
  Serial.begin(115200);
  delay(100);
  FastLED.addLeds<WS2812B, RING_PIN, GRB>(leds, NUM_LEDS);    // on précise le type de leds, le numéro de la broche, l'odre des couleurs, le tableau et le nombre de leds utilisées                
  FastLED.clear();
  BlynkEdgent.begin();
  dht.begin();
  timer.setInterval(1000L, dht_fonction); 

}

BLYNK_WRITE(V3)
{
  lum = param.asInt();
  data_couleur(red,green,blue,lum);
}
BLYNK_WRITE(V4)
{
  String r = param[0].asString();
  String g = param[1].asString();
  String b = param[2].asString();

  red = r.toInt();
  green = g.toInt();
  blue = b.toInt();
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  
  data_couleur(red,green,blue,lum);
}

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Serial.println(value);
  if (value == 1)
  {
    fill_solid(leds, NUM_LEDS, CRGB(red,green,blue));
    FastLED.show();
  }
  else
  {
   fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }
}
void loop() {
  BlynkEdgent.run();
  timer.run();
}

void dht_fonction()
{
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();

  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, hum);
  

}

void data_couleur(int r, int g, int b, int brightness)
{
  FastLED.setBrightness(brightness);  
  fill_solid(leds, NUM_LEDS, CRGB(red,green,blue));
  FastLED.show();
}

