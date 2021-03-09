
// GPIO pin declarations:
// input
#define analogIn  A0      // pin to connect mic module
#define digitalIn 2       // digital input
#define resetPIN  3       // reset button
// output
#define ledOut    6       // data pin for LED

// fastLED definitions
#include <FastLED.h>      // fastLED library

#define NUM_LEDS  37      // number of LED in strip
#define CHIPSET   WS2811  // chip of LED strip
#define COLOUR_ORD RGB    // order of declaring colours
#define BRIGHTNESS 120    // brightness of LED strip

CRGBArray<NUM_LEDS> leds; // creates array with NUM_LEDS number of elements

// Time values
#define second    1000
#define tenthSec  100
#define hundSec   10

int dig;            // digital input read value
int ana;            // analog input read value (signal from mic module)
bool reset;         // resets meanValue and calibrates

int sound = 0;      // value of the volume
int maxSound = 500; // maximum sound value <variable>
int mean = 0;       // value when no sound
int sum = 0;        // temporary variable to find average
int counter;        // value to count average to

int ledToLight;     // stores the number of LEDs to light

//function to reset the mean value over 1 second
int meanValue(int timeValue){
  int meanVal = 0;
  sum = 0;
  for(counter = 1; counter <= 10; counter++){  
      sum = sum + analogRead(analogIn);      
      delay(timeValue);
    }
  if (counter > 10) {
    meanVal = sum/(counter-1);
    sum = 0;
  }
  return meanVal;
}


void light(){
  ledToLight = map(sound, 0, maxSound, 1, NUM_LEDS); 
  
  if (sound >= maxSound) {
    maxSound = sound;
  }
  
//  Serial.print(sound);
//  Serial.print("  |  ");
//  Serial.print(maxSound);
//  Serial.print("  |  ");
//  Serial.println(ledToLight);
}

void setup() {

  FastLED.addLeds<CHIPSET, ledOut, COLOUR_ORD>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode(analogIn, INPUT);
  pinMode(digitalIn, INPUT);
  pinMode(resetPIN, INPUT);

  Serial.begin(9600);

  delay(3000);

  for(counter = 1; counter <= 10; counter++){  
      sum = sum + analogRead(analogIn);      
      delay(tenthSec);
  }
  if (counter > 10) {
    mean = sum/(counter-1);
    sum = 0;
  }
  
}

void loop() {
  
  ana = analogRead(analogIn);
  reset = digitalRead(resetPIN);
  delay(hundSec);

  //Function to calibrate the sensor to mean value
  if (reset) {
    mean = meanValue(tenthSec);
  }
  else {
    sound = meanValue(hundSec);
    sound = abs(sound - mean);
  }
  light();  // decides number of LEDS to light

//  Serial.print(ana);
//  Serial.print("  |  ");
//  Serial.print(mean);
//  Serial.print("  |  ");
//  Serial.print(sound);
//  Serial.println("  |  ");
  
  
  Serial.println(sound);
}
