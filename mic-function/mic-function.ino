
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
#define CHIPSET   WS2812  // chip of LED strip
#define COLOUR_ORD RGB    // order of declaring colours
#define BRIGHTNESS 120    // brightness of LED strip

CRGB leds[NUM_LEDS]; // creates array with NUM_LEDS number of elements

// Time values
#define second    1000
#define tenthSec  100
#define hundSec   10

int dig;            // digital input read value
int ana;            // analog input read value (signal from mic module)
bool reset;         // resets meanValue and calibrates

int sound = 0;      // value of the volume
int maxSound = 50; // maximum sound value <variable>
int mean = 0;       // value when no sound
int sum = 0;        // temporary variable to find average
int counter;        // value to count average to

int ledToLight;     // stores the number of LEDs to light
int greenLight  = NUM_LEDS / 1.5; 
int yellowLight = greenLight + (NUM_LEDS / 5);
int redLight    = NUM_LEDS / 5;

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
//  Serial.println("In Light()");
  
  ledToLight = map(sound, 0, maxSound, 1, NUM_LEDS); 
  
  if (sound >= maxSound) {
    maxSound = sound;
  }
//  Serial.print("  |  ");
//  Serial.print(ana);
//  Serial.print("  |  ");
//  Serial.print(mean);
//  Serial.print("  |  ");
//  Serial.print(sound);
//  Serial.print("  |  ");
//  Serial.print(maxSound);
//  Serial.print("  |  ");
//  Serial.println(ledToLight);
  return;
}

void volumeMeter(){
//  Serial.println("In volumeMeter()");
  
  int off = ledToLight + 1;
//  Serial.print(ledToLight);
//  Serial.print("  |  ");
//  Serial.println(off);

  for(int i = 0; i < ledToLight; i++){
    if(i >= yellowLight) {
      leds[i] = CRGB::Green;
    }
    else if (i >= greenLight){
      leds[i] = CRGB::Yellow;
    }
    else{
      leds[i] = CRGB::Red;
    }
//    Serial.print("*");
  }
//  
  for(off; off <= NUM_LEDS; off++){
    leds[off] = CRGB::Black;
    
//    Serial.print("o");
    //FastLED.show();
  }
  FastLED.show();
  return;
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Begin Setup");

  FastLED.addLeds<CHIPSET, ledOut, COLOUR_ORD>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  
  Serial.println("Set up FastLED");
  
  pinMode(analogIn, INPUT);
  pinMode(digitalIn, INPUT);
  pinMode(resetPIN, INPUT);
  Serial.println("Set up pins");

  delay(3000);

  Serial.println("Calibrating");
  for(counter = 1; counter <= 10; counter++){  
      sum = sum + analogRead(analogIn);      
      delay(tenthSec);
      Serial.print(".");
  }
  if (counter > 10) {
    Serial.println("Done Calibration!");
    mean = sum/(counter-1);
    sum = 0;
  }

  Serial.println(greenLight);
  Serial.println(yellowLight);
  Serial.println(redLight);

  
  Serial.println("End of Setup");
}

void loop() {
//  Serial.println("Begin loop");
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
//  Serial.println("Going to light()");
  light();        // decides number of LEDs to light
//  Serial.println("Going to volumeMeter()");
  volumeMeter();  // turns LEDs on depending on light()
  
//  Serial.print(ana);
//  Serial.print("  |  ");
//  Serial.println(mean);
//  Serial.print("  |  ");
  Serial.println(sound);
//  Serial.println("  |  ");
  
  
//  Serial.println(sound);
}
