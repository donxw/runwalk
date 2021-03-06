#define audiopin 26  //gpio26
#define audiochannel 0

int walk_duration = 120; // set walk duration in seconds
int run_duration = 60; // set run duration in seconds


// ************* Image Bitmaps **********************************
// Images used in program were generated at http://javl.github.io/image2cpp/
const unsigned char myRunner [] PROGMEM = {
  // 'runner, 32x32px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x1f, 0xc0, 
  0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x4f, 0x80, 
  0x00, 0x03, 0xf2, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0x3c, 0x01, 0xff, 0xff, 0xfc, 
  0x00, 0xe1, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x07, 0xf8, 0x00, 
  0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x1e, 0xfe, 0x00, 
  0x00, 0x7e, 0x3e, 0x00, 0x07, 0xfc, 0x3e, 0x00, 0x3f, 0xfc, 0x3c, 0x00, 0x3f, 0xf8, 0x7c, 0x00, 
  0x3f, 0x80, 0x78, 0x00, 0x3c, 0x00, 0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x01, 0xf0, 0x00, 
  0x00, 0x03, 0xe0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char myWalker [] PROGMEM = {
  // 'walker, 32x32px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xfc, 0x00, 
  0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x80, 0x00, 
  0x00, 0x07, 0xe0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xf8, 0x00, 
  0x00, 0xe7, 0xf8, 0x00, 0x00, 0xef, 0xfe, 0x00, 0x01, 0xcf, 0xdf, 0x80, 0x01, 0xcf, 0xcf, 0xc0, 
  0x01, 0xcf, 0x83, 0xc0, 0x00, 0xcf, 0x80, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
  0x00, 0x1f, 0xc0, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x3d, 0xf0, 0x00, 
  0x00, 0x7c, 0xf0, 0x00, 0x00, 0x78, 0xf8, 0x00, 0x00, 0x78, 0x78, 0x00, 0x00, 0xf0, 0x7c, 0x00, 
  0x00, 0xf0, 0x3c, 0x00, 0x00, 0xe0, 0x1c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00
};

#include "Pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int melody_down[] = {
  NOTE_F6, NOTE_D6
};

int melody_up[] = {
  NOTE_D6, NOTE_F6
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int noteDurationsDown[] = {
  8, 4
};


void playdowntone() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurationsDown[thisNote];
    tone(audiopin, melody_down[thisNote], noteDuration, audiochannel);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.20;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(audiopin, audiochannel);
  }
}

void playuptone() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurationsDown[thisNote];
    tone(audiopin, melody_up[thisNote], noteDuration, audiochannel);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(audiopin, audiochannel);
  }
}

void playtone() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(audiopin, melody[thisNote], noteDuration, audiochannel);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(audiopin, audiochannel);
  }
}
