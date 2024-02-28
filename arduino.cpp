#include <FastLED.h>

#define LED_PIN     9
#define NUM_LEDS    37
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define TRIG_PIN 10
#define ECHO_PIN 11

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  
  // Only consider distances within the 210 to 250 cm range
  if(distance >= 210 && distance <= 250) {
    int volumeLevel = map(distance, 210, 250, 0, 255); // Map distance to volume level
    volumeLevel = constrain(volumeLevel, 0, 255); // Constrain volume to valid range

    Serial.println(volumeLevel);
    
    int numLedsToLight = map(volumeLevel, 0, 255, 0, NUM_LEDS);
    for(int i = 0; i < NUM_LEDS; i++) {
      if(i < numLedsToLight) {
        leds[i] = CRGB::Green;
      } else {
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
  
  delay(500);
}
