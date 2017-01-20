#include "pitches.h"

const int musicInputPin = 8;

// notes in the melody:
int melody[] = {NOTE_D4S, NOTE_F4, NOTE_F4S, NOTE_G4S, NOTE_A4S, NOTE_D5S, NOTE_C5S, NOTE_A4S, NOTE_D4S, NOTE_A4S, NOTE_G4S, 
NOTE_F4S, NOTE_F4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8, 8, 8, 8, 4, 8, 8, 4, 4, 8, 8, 8, 8
};

int numberOfNotes = 13;

void setup() {
  // iterate over the notes of the melody:
  for (int currentNote = 0; currentNote < numberOfNotes; currentNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 2000 / noteDurations[currentNote];
    tone(musicInputPin, melody[currentNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(musicInputPin);
  }
}

void loop() {
  // no need to repeat the melody.
}

