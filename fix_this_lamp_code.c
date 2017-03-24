
// https://create.arduino.cc/editor/bischoff10/cc817285-0c92-48e4-92cd-5ebf34f6a19c/preview

// this constant won't change:
const int  buttonPin = 2;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 9; // Analog output pin that the LED is attached to
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int sensorValue = 0;        // value read from the pot
int brightValue = 0;        // value output to the PWM (analog out)
int flashSpeed = 0;
int fadeValue = 0;
int fadeSpeed = 0;
int fadeIncrement = 0;

const byte off[] = {0, 0, 0};
const byte red[] = {255, 0, 0};
const byte yellow[] = {255, 255, 0};
const byte green[] = {0, 255, 0};
const byte aqua[] = {0, 255, 255};
const byte blue[] = {0, 255, 255};
const byte purple[] = {255, 0, 255};

byte *colors[][3] = {{off}, {red}, {yellow}, {green}, {aqua}, {blue}, {purple}};
//   {off[] = {0, 0, 0}}, 
//   {red[] = {255, 0, 0}}, 
//   {yellow[] = {255, 255, 0}}, 
//   {green[] = {0, 255, 0}}, 
//   {aqua[] = {0, 255, 255}}, 
//   {blue[] = {0, 0, 255}}, 
//   {purple[] = {255, 0, 255}}
  
// };

// const byte colors[7][3];
// colors[0][0] = OFF[0]; colors[0][1] = OFF[1]; colors[0][2] = OFF[2];
// colors[1][0] = WHT[0]; colors[1][1] = WHT[1]; colors[1][2] = WHT[2];
// colors[2][0] = RED[0]; colors[2][1] = RED[1]; colors[2][2] = RED[2];
// colors[3][0] = YEL[0]; colors[3][1] = YEL[1]; colors[3][2] = YEL[2];
// colors[4][0] = GRN[0]; colors[4][1] = GRN[1]; colors[4][2] = GRN[2];
// colors[5][0] = AQU[0]; colors[5][1] = AQU[1]; colors[5][2] = AQU[2];
// colors[6][0] = BLU[0]; colors[6][1] = BLU[1]; colors[6][2] = BLU[2];
// colors[7][0] = PUR[0]; colors[7][1] = PUR[1]; colors[7][2] = PUR[2];

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  // byte colors[8][3] = {{off}, {red}, {yellow}, {green}, {aqua}, {blue}, {purple}};
  // colors[1][0] = 255;
  // attachInterrupt(digitalPinToInterrupt(buttonPin), flashingColors, CHANGE);
  Serial.begin(9600);
}


void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  lastButtonState = buttonState;
  
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  brightValue = map(sensorValue, 0, 1023, 0, 255);
  flashSpeed = map(sensorValue, 0, 1023, 0, 2046);
  fadeSpeed = map(sensorValue, 0, 1023, 1, 2046);
  // fadeIncrement = map(fadeSpeed, 1, 2046, 10, 1);

  if (buttonPushCounter == 1) {
    setColor(1);  // white
  // } else if (buttonPushCounter == 2) {
  //     setColor(brightValue, 0, 0);  // red
  // } else if (buttonPushCounter == 3) {
  //     setColor(brightValue, brightValue, 0);  // yellow
  // } else if (buttonPushCounter == 4) {
  //     setColor(0, brightValue, 0);  // green
  // } else if (buttonPushCounter == 5) {
  //     setColor(0, brightValue, brightValue);  // aqua
  // } else if (buttonPushCounter == 6) {
  //     setColor(0, 0, brightValue);  // blue
  // } else if (buttonPushCounter == 7) {
  //     setColor(brightValue, 0, brightValue);  // purple
  // } else if (buttonPushCounter == 8) {
  //     flashingColors(flashSpeed);
  // } else if (buttonPushCounter == 2) {
      // setColor(0, 0, 0);
      // fadingColors(); //red
  //     // red to yellow
  //     fadingColors(redPin, greenPin, false); //yellow
  //     // yellow to green
  //     fadingColors(false, greenPin, false); //green
  //     // green to aqua
  //     fadingColors(false, greenPin, bluePin); //aqua
  //     // aqua to blue
  //     fadingColors(false, false, bluePin); //blue
  //     //  blue to purple
  //     fadingColors(redPin, false, bluePin); //purple
  //     //  purple to red
      
  } else if (buttonPushCounter == 2) {
      digitalWrite(ledPin, HIGH);
  } else if (buttonPushCounter >= 3){
      digitalWrite(ledPin, LOW);
      setColor(0);
      buttonPushCounter = 0;
  }

}

void setColor(int color) {
  analogWrite(redPin, colors[color][0]);
  analogWrite(greenPin, colors[color][1]);
  analogWrite(bluePin, colors[color][2]);  
  Serial.println(&colors[color][0]);
}

// void flashingColors(int speed) {
//   setColor(255, 0, 0);  // red
//   delay(speed);
//   setColor(255, 255, 0);  // yellow
//   delay(speed);  
//   setColor(0, 255, 0);  // green
//   delay(speed);
//   setColor(0, 255, 255);  // aqua
//   delay(speed);
//   setColor(0, 0, 255);  // blue
//   delay(speed);
//   setColor(80, 0, 80);  // purple
//   delay(speed);
// }

// void fadingColors() {

//   for (int redValue= 0; redValue <= 255; redValue += 10){
//     analogWrite(redPin, redValue);
//     delay(fadeSpeed);
//     Serial.println(fadeSpeed);
//     if (redValue == 255) {
//       redValue -= 10;
//       analogWrite(redPin, redValue);
//       delay(fadeSpeed);
//     }
//   }
// }

// void PulsingColors(){
//   // fade in from min to max in increments of 5 points:
//   for (fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
//     // sets the value (range from 0 to 255):
//     analogWrite(redPin, fadeValue);
//     delay(fadeSpeed);
//     // analogWrite(greenPin, fadeValue);
//     // analogWrite(bluePin, fadeValue);
//     // takes input from pontentiometer for speed of fading
//     delay(fadeSpeed);
//   }
//   // delay(fadeSpeed);
//   // fade out from max to min in increments of 5 points:
//   for (fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
//     // sets the value (range from 0 to 255):
//     analogWrite(redPin, fadeValue);
//     // analogWrite(greenPin, fadeValue);
//     // analogWrite(bluePin, fadeValue);
//     // wait for 30 milliseconds to see the dimming effect
//     delay(fadeSpeed);
//   }
  // delay(fadeSpeed);
  //   for (fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
  //   // sets the value (range from 0 to 255):
  //   analogWrite(redPin, fadeValue);
  //   analogWrite(greenPin, fadeValue);
  //   // analogWrite(bluePin, fadeValue);
  //   // takes input from pontentiometer for speed of fading
  //   //delay(30)
  // }
  // delay(fadeSpeed);
  // // fade out from max to min in increments of 5 points:
  // for (fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
  //   // sets the value (range from 0 to 255):
  //   analogWrite(redPin, fadeValue);
  //   analogWrite(greenPin, fadeValue);
  //   // analogWrite(bluePin, fadeValue);
  //   // wait for 30 milliseconds to see the dimming effect
    
  // }
  // delay(fadeSpeed);
// }