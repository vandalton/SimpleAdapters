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

void handleNoteAction(byte note, bool pressed)
{
  if (note == 60) // C4
    key1 = pressed;
  else if (note == 61)
    key2 = pressed;
  else if (note == 62)
    key3 = pressed;
  else if (note == 63)
    key4 = pressed;
  else if (note == 64)
    key5 = pressed;
  else if (note == 65)
    key6 = pressed;
  else if (note == 66)
    key7 = pressed;
  else if (note == 67)
    key8 = pressed;
  else if (note == 68)
    key9 = pressed;
  else if (note == 69) 
    keyAsterisk = pressed;
  else if (note == 70)
    key0 = pressed;
  else if (note == 71)
    keyHash = pressed;

}

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{
  handleNoteAction(inNote, true);
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
  handleNoteAction(inNote, false);
}

void setup() {
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI); 
}

void loop() {
  bool row1 = digitalRead(IN_ROW_1);
  bool row2 = digitalRead(IN_ROW_2);
  bool row3 = digitalRead(IN_ROW_3);
  bool row4 = digitalRead(IN_ROW_4);

  if (!row1)
  {
    digitalWrite(OUT_COL_1, keyHash ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key0 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, keyAsterisk ? LOW : HIGH);
  }
  else if(!row2)
  {
    digitalWrite(OUT_COL_1, key9 ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key8 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, key7 ? LOW : HIGH);
  }
  else if(!row3)
  {
    digitalWrite(OUT_COL_1, key6 ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key5 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, key4 ? LOW : HIGH);
  }
  else if(!row4)
  {
    digitalWrite(OUT_COL_1, key3 ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key2 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, key1 ? LOW : HIGH);
  }
}
