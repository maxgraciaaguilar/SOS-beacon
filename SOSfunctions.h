#ifndef SOS_FUNCTIONS
#define SOS_FUNCTIONS


int redPin = 9;
int bluePin = 5;
int greenPin = 6;
int buttonPin = 2;
int speakerPin = 7;

int redVal = 0;    
int blueVal = 0;     
int greenVal = 0;     
extern int speakerTone;
extern int dit;

bool buttonStatus = 0;
bool lastButtonStatus = 0;
volatile byte toggle = 0;

int modePin = A1;
int colorPin = A0;


void SOSdelay(int ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (!toggle) return;
  }
}


void onPress() {
  static unsigned long lastPress = 0;
  unsigned long now = micros();
  if (now - lastPress > 200000) {
    toggle = !toggle;
    lastPress = now;
  }
}

// Made by Claude AI
void hueToRGB(int hue, int &r, int &g, int &b) {
  int sector = hue / 60;            // which 60-degree slice of the wheel (0-5)
  int offset = hue % 60;            // how far into that slice (0-59)
  int rising  = offset * 255 / 60;  // ramp up   0 -> 255 across the slice
  int falling = 255 - rising;       // ramp down 255 -> 0 across the slice
 
  switch (sector) {
    case 0:  r = 255;     g = rising;  b = 0;       break; // red   -> yellow
    case 1:  r = falling; g = 255;     b = 0;       break; // yellow-> green
    case 2:  r = 0;       g = 255;     b = rising;  break; // green -> cyan
    case 3:  r = 0;       g = falling; b = 255;     break; // cyan  -> blue
    case 4:  r = rising;  g = 0;       b = 255;     break; // blue  -> magenta
    default: r = 255;     g = 0;       b = falling; break; // magenta-> red
  }
}

void updateColor() {

  int colorReading = analogRead(colorPin);
  int hue = map(colorReading, 0, 1023, 0, 359);

  hueToRGB(hue, redVal, blueVal, greenVal);

}

void TurnOn(int delayTime, int speakerTone) {
  updateColor();
  analogWrite(redPin, redVal);
  analogWrite(bluePin, blueVal);
  analogWrite(greenPin, greenVal);
  tone(speakerPin, speakerTone);
  SOSdelay(delayTime);
}

void TurnOff(int delayTime) {
  analogWrite(redPin, 0);
  analogWrite(bluePin, 0);
  analogWrite(greenPin, 0);
  noTone(speakerPin);
  SOSdelay(delayTime);
}



#endif