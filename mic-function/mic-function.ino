
// GPIO pin declarations:
// input
#define analogIn  A0  // pin to connect mic module
#define digitalIn 2   // digital input
#define resetPIN  3   // reset button
// output



int dig;            // digital input read value
int ana;            // analog input read value (signal from mic module)
bool reset;         // resets meanValue and calibrates

int sound = 0;      // value of the volume
int mean = 0;       // value when no sound
int sum = 0;        // temporary variable to find average
int counter;        // value to count average to

//function to reset the mean value
int meanValue(){
  for(counter = 1; counter <= 10; counter++){  
      sum = sum + analogRead(analogIn);      
      delay(100);
    }
  mean = sum/(counter-1);
  if (counter > 10) {
    sum = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(analogIn, INPUT);
  pinMode(digitalIn, INPUT);
  pinMode(resetPIN, INPUT);

  Serial.begin(9600);

  delay(3000);

  for(counter = 1; counter <= 10; counter++){  
      sum = sum + analogRead(analogIn);      
      delay(100);
  }
  mean = sum/(counter-1);
  if (counter > 10) {
    sum = 0;
  }
  
}

void loop() {
  
  ana = analogRead(analogIn);
  reset = digitalRead(resetPIN);
  delay(100);

  //Function to calibrate the sensor to mean value
  if (reset) {
    meanValue();
  }
  else {
    sound = abs(ana - mean);
  }
  Serial.println(sound);
}
