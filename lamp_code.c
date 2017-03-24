//
// Constants
//

// pins
const int led_pin = 13;
const int red_pin = 11;
const int grn_pin = 10;
const int blu_pin = 9;
const int but_pin = 2;
const int analogInPin = A0;

// analog input max
const int ANALOGMAX = 1023;

// RBG colors
const byte OFF[3] = {0,0,0};
const byte WHT[3] = {255,255,255};
const byte RED[3] = {255,0,0};
const byte GRN[3] = {0,255,0};
const byte BLU[3] = {0,0,255};
const byte YEL[3] = {255,255,0};
const byte AQU[3] = {0,255,255};
const byte PUR[3] = {255,0,255};

//
// Globals
//

// program state
int state = 0;

// pot value
int sensorValue = 0;

// pointer to color in color ALL_COLORS array
byte colorPointer = 0;

// cycle colors
byte ALL_COLORS[8][3] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

void setup() {
  // initialize output pins
  pinMode(led_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(grn_pin, OUTPUT);
  pinMode(blu_pin, OUTPUT);
  
  // assign colors to cycle
  ALL_COLORS[0][0] = OFF[0]; ALL_COLORS[0][1] = OFF[1]; ALL_COLORS[0][2] = OFF[2];
  ALL_COLORS[1][0] = WHT[0]; ALL_COLORS[1][1] = WHT[1]; ALL_COLORS[1][2] = WHT[2];
  ALL_COLORS[2][0] = RED[0]; ALL_COLORS[2][1] = RED[1]; ALL_COLORS[2][2] = RED[2];
  ALL_COLORS[3][0] = YEL[0]; ALL_COLORS[3][1] = YEL[1]; ALL_COLORS[3][2] = YEL[2];
  ALL_COLORS[4][0] = GRN[0]; ALL_COLORS[4][1] = GRN[1]; ALL_COLORS[4][2] = GRN[2];
  ALL_COLORS[5][0] = AQU[0]; ALL_COLORS[5][1] = AQU[1]; ALL_COLORS[5][2] = AQU[2];
  ALL_COLORS[6][0] = BLU[0]; ALL_COLORS[6][1] = BLU[1]; ALL_COLORS[6][2] = BLU[2];
  ALL_COLORS[7][0] = PUR[0]; ALL_COLORS[7][1] = PUR[1]; ALL_COLORS[7][2] = PUR[2];
 
  // For more info see https://www.allaboutcircuits.com/technical-articles/using-interrupts-on-arduino/
  // attach interrupt for button on pin 2 mapping to interrupt vector memory 0
  // pin 3 on Uno would map to interrupt vector memory 1
  // second argument in the name of the interrupt service routine (function)
  // this function handles the interrupt
  // third argument is the interrupt mode:
  // RISING, FALLING, CHANGE, LOW
  // choose RISING as the pin sees a low to high transition when being pressed
  attachInterrupt(0, buttonInterrupt, CHANGE);
 
  // initialize serial communication:
  Serial.begin(9600);
}
 
void loop() {
  // read the raw pot input:
  sensorValue = analogRead(analogInPin);
 
  if (state < (sizeof(ALL_COLORS) / 3)) {
    handleSolidCycle();
  } else if (state == (sizeof(ALL_COLORS) / 3)) {
    handleFlashCycle();
  } else if (state == (sizeof(ALL_COLORS) / 3) + 1) {
    handleFadeCycle();
  } else if (state == (sizeof(ALL_COLORS) / 3) + 2){
    handleGlowCycle();
  } else if (state >= (sizeof(ALL_COLORS) / 3) + 3){
    state = 0;
  }
}
 
void buttonInterrupt() {
  // initial set last millis value to 0
  static unsigned long millisLast = 0;
  
  // get current millis value
  unsigned long millisNow = millis();
  
  // If interrupts come faster than debounce time, assume it's a bounce and ignore
 if (millisNow - millisLast > 25) {
    
    // rising edge, button down
    if (digitalRead(but_pin) == HIGH) {
      // increment button push counter
      state++;
      
      // log the push and counter
      Serial.println("down");
      Serial.print("number of button pushes: ");
      Serial.println(state);
      
    // falling edge, button up
    } else {
      // log the push
      Serial.println("up");
    }
 }
 
 // assign interrupt time stamp
 millisLast = millisNow;
}
 
// Map float values
float mapfloat (float input, float input0, float inputf, float output0, float outputf) {
  return output0 + ((outputf - output0) / (inputf - input0)) * (input - input0);
}
 
// Handle incrementing the color ALL_COLORS array
void cycleColorPointer() {
  if (colorPointer < (sizeof(ALL_COLORS) / 3) - 1) {
    colorPointer++;
  } else {
    colorPointer = 2;
  }
}
 
byte nextColorPointer() {
  byte nextColor;
  
  if (colorPointer < (sizeof(ALL_COLORS) / 3) - 1) {
    nextColor = colorPointer + 1;
  } else {
    nextColor = 2;
  }
  
  return nextColor;
}
 
// Takes an array of 3 bytes representing RGB levels
void setColor(byte *levels, float brightness) {
  byte red_lvl = (byte) (levels[0] * brightness * 1.00);
  byte grn_lvl = (byte) (levels[1] * brightness * 0.75);
  byte blu_lvl = (byte) (levels[2] * brightness * 0.75);
  analogWrite(red_pin, red_lvl);
  analogWrite(grn_pin, grn_lvl);
  analogWrite(blu_pin, blu_lvl);
}
 
void handleSolidCycle() {
  // get current requested brightness as ratio
  float brightness = mapfloat((float) sensorValue, 0.0, (float) ANALOGMAX, 0.0, 1.0);
  
  // set color
  setColor(ALL_COLORS[state], brightness);
}
 
void handleFlashCycle() {
  // get current requested brightness as ratio
  // float brightness = mapfloat((float) sensorValue, 0.0, (float) ANALOGMAX, 0.0, 1.0);
  float brightness = 0.5;
  
  // get current requested cycle delay in ms from 127 to 4096
  unsigned long delay = (unsigned long) mapfloat((float) sensorValue, 0.0, (float) ANALOGMAX, 1.0, 4096.0);
  
  // initial set last millis value to 0
  static unsigned long millisStart = 0;
  
  // get current millis value
  unsigned long millisNow = millis();
  
  // check if time to update
  if (millisNow - millisStart > delay) {
    
    // assign new target millis
    millisStart = millis();
    
    // cycle pointer
    cycleColorPointer();
    
    // set color
    setColor(ALL_COLORS[colorPointer], brightness);
  }
}
 
void handleFadeCycle() {
  // get current requested brightness as ratio
  // float brightness = mapfloat((float) sensorValue, 0.0, (float) ANALOGMAX, 0.0, 1.0);
  float brightness = 0.5;
  
  // get current requested cycle delay in ms from 127 to 4096
  unsigned long delay = (unsigned long) mapfloat((float) sensorValue, 0.0, (float) ANALOGMAX, 1.0, 4096.0);
  
  // initial set last millis value to 0
  static unsigned long millisStart = 0;
  
  // get current millis value
  unsigned long millisNow = millis();
  
  // check if time to assign new target millis and update color
  if (millisNow - millisStart > delay) {
    
    // assign new target millis
    millisStart = millis();
    
    // cycle pointer
    cycleColorPointer();
  }
  
  // get current and next color RGB values
  byte thisColor[3];
  byte nextColor[3];
  thisColor[0]= ALL_COLORS[colorPointer][0];
  thisColor[1]= ALL_COLORS[colorPointer][1];
  thisColor[2]= ALL_COLORS[colorPointer][2];
  nextColor[0]= ALL_COLORS[nextColorPointer()][0];
  nextColor[1]= ALL_COLORS[nextColorPointer()][1];
  nextColor[2]= ALL_COLORS[nextColorPointer()][2];
  
  // make array RGB values to send
  byte sendColor[3];
  
  // find current times in floats
  float float_millisNow = (float) millisNow;
  float float_millisStart = (float) millisStart;
  float float_millisTarget = float_millisStart + ((float) delay);
  
  // convert cycle delay to level change delay for current and next color RGB values
  for (byte rgb = 0; rgb < 3; rgb++) {
    sendColor[rgb] = (byte) mapfloat(float_millisNow, float_millisStart, float_millisTarget, (float) thisColor[rgb], (float) nextColor[rgb]);
  }
  
  setColor(sendColor, brightness);
}
 
void handleGlowCycle() {
  // get current requested cycle delay in ms from 127 to 4096
  unsigned long delay = (unsigned long) mapfloat((float) sensorValue, 0.0, (float) ANALOGMAX, 1.0, 4096.0);
  
  // assign a glowing direction
  static bool glowing = true;
  
  // initial set last millis value to 0
  static unsigned long millisStart = 0;
  
  // get current millis value
  unsigned long millisNow = millis();
  
  // find current times in floats
  float float_millisNow = (float) millisNow;
  float float_millisStart = (float) millisStart;
  float float_millisTarget = float_millisStart + ((float) delay);
  
  // convert cycle delay to brightness change
  float brightness = mapfloat(float_millisNow, float_millisStart, float_millisTarget, 0.0, 1.0);
  
  // check glowing direction
  if (!glowing) {
    brightness = 1.0 - brightness;
  }
  
  setColor(ALL_COLORS[colorPointer], brightness);
  
  // check if time to assign new target millis and update color
  if (millisNow - millisStart > delay) {
    
    // assign new target millis
    millisStart = millis();
    
    // check glowing direction
    if (!glowing) {
      // cycle pointer
      cycleColorPointer();
    }
    
    glowing = !glowing;
  }
}