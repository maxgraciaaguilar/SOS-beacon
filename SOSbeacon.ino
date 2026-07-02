/*
 * SOSbeacon.ino
 * Compatible with SOS beacon LED and speaker + potentiometer to control modes and color. On press, the selected mode will begin until pressed again (with exception of manual mode)
 * Maximiliano Gracia Aguilar (maxgraciaaguilar@hotmail.com)
 * 2026-06-26
 *
 * Assembly guide: https://docs.google.com/document/d/18mNmDa8XmLQRfkeWav0mc5fWfhDpotYLdkZFLXzf-O4/edit?tab=t.0
 *
 * 2026 Treobytes. All rights reserved.
 */

#include "pitches.h"
#include "SOSfunctions.h"

// EDITABLE PARAMETERS
int speakerTone = 550;  // 100-2000
int dit = 100;          // Morse code speed

void SOS() {

  // S
  for (int i = 0; i < 3 && toggle; ++i) {
    TurnOn(dit, speakerTone);
    TurnOff(dit);
  }
  
  SOSdelay(dit * 2);

  // O
  for (int i = 0; i < 3 && toggle; ++i) {
    TurnOn(dit * 3, speakerTone);
    TurnOff(dit);
  }
  
  SOSdelay(dit * 2);

  // S
  for (int i = 0; i < 3 && toggle; ++i) {
    TurnOn(dit, speakerTone);
    TurnOff(dit);
  } 

  SOSdelay(5000);
}

void alarm() {

  for (int i = 0; i < 200 && toggle; ++i) {
    TurnOn(10, speakerTone + i);
  } 
  for (int i = 0; i < 200 && toggle; ++i) {
    TurnOn(10, speakerTone + 200 - i);
  }
  TurnOff(1);

}

void jingle() {

  // Inspired from royalthumb https://wokwi.com/projects/386864340532349953

  static const int melody[] PROGMEM = {
    NOTE_E4, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_A3, 0,
    NOTE_D4, NOTE_F4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_A3, 0,
  };
 
  static const int noteDurations[] PROGMEM = {
    3, 6, 6, 3, 6, 6, 3, 6, 6, 3, 6, 6, 2, 6, 3, 3, 3, 3, 3, 2, 3, 6, 3, 6, 6, 2, 6, 3, 6, 6, 3, 6, 6, 3, 3, 3, 3, 3, 3,
  };
 
  int numNotes = sizeof(melody) / sizeof(melody[0]);
 
  for (int i = 0; i < numNotes && toggle; ++i) {
    int note = pgm_read_word_near(melody + i);
    int divider = pgm_read_word_near(noteDurations + i);
    int noteDuration = 1000 / divider;
    int gap = noteDuration * 0.30;   
 
    if (note == 0) {
      TurnOff(noteDuration + gap);  
    } else {
      TurnOn(noteDuration, note);  
      TurnOff(gap);                 
    }
  }
}

void manual() {

  updateColor();

  if (digitalRead(buttonPin) == LOW) {
    analogWrite(redPin, redVal);
    analogWrite(bluePin, blueVal);
    analogWrite(greenPin, greenVal);
    tone(speakerPin, speakerTone);
  }
  else {
    analogWrite(redPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(greenPin, 0);
    noTone(speakerPin);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), onPress, FALLING);

  analogWrite(redPin, 255); analogWrite(greenPin, 0); analogWrite(bluePin, 0); delay(1000);
  analogWrite(redPin, 0); analogWrite(greenPin, 255); analogWrite(bluePin, 0); delay(1000);
  analogWrite(redPin, 0); analogWrite(greenPin, 0); analogWrite(bluePin, 255); delay(1000);
  analogWrite(redPin, 0); analogWrite(greenPin, 0); analogWrite(bluePin, 0);
}

void loop()
{

  int modeReading = analogRead(modePin);
  int mode = map(modeReading, 0, 1023, 0, 4);

  if (mode > 3) mode = 3;                        
    mode += 1;
  
  switch(mode) {
    case 1:
      if (toggle) jingle();
      break;
    case 2:
      if (toggle) alarm();
      break;
    case 3:
      if (toggle) SOS();
      break;
    case 4:
      manual();
      break;
  }
  
}