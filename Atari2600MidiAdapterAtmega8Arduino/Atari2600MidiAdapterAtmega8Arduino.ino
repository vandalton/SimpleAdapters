#include <MIDI.h>

// Atari 2600 joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - row 1, 2 - row 2, 3 - row 3, 4 - row 4, 5 - col 1
// 6 - col 3, 7 - 5V (HIGH), 8 - GND (LOW), 9 - col 2

#define OUT_COL_3 9 // connect digital pin 7 of Atmega to pin 6 of joystick port
#define OUT_COL_2 8
#define OUT_COL_1 7

#define IN_ROW_4 6
#define IN_ROW_3 5
#define IN_ROW_2 4
#define IN_ROW_1 3

int inputs[4] = { IN_ROW_1, IN_ROW_2, IN_ROW_3, IN_ROW_4 };
int outputs[3] = { OUT_COL_1, OUT_COL_2, OUT_COL_3 };

volatile bool key1 = false, 
              key2 = false, 
              key3 = false,
              key4 = false, 
              key5 = false,
              key6 = false,
              key7 = false,
              key8 = false,
              key9 = false,
              key0 = false,
              keyHash = false,
              keyAsterisk = false;

struct PortInit {
  PortInit() {        
    for(int i=0;i<3;++i)
    {
      pinMode(outputs[i], OUTPUT);
      digitalWrite(outputs[i], HIGH);
    }
    
    for(int i=0;i<4;++i)
      pinMode(inputs[i], INPUT);
  }
};

PortInit portInit;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI); 
}

void loop() {
  if (MIDI.read())
  {
    MIDI.sendNoteOn(42, 127, 1);
  }
}
