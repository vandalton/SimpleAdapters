#include <MIDI.h>

// Atari 2600 joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - row 1, 2 - row 2, 3 - row 3, 4 - row 4, 5 - col 1
// 6 - col 3, 7 - 5V (HIGH), 8 - GND (LOW), 9 - col 2

#define OUT_COL_1 9 // connect digital pin 7 of Atmega to pin 6 of joystick port
#define OUT_COL_2 8
#define OUT_COL_3 7

#define OUT_2_COL_1 2
#define OUT_2_COL_2 19
#define OUT_2_COL_3 18

#define IN_ROW_1 6
#define IN_ROW_2 5
#define IN_ROW_3 4
#define IN_ROW_4 3

#define IN_2_ROW_1 17
#define IN_2_ROW_2 16
#define IN_2_ROW_3 15
#define IN_2_ROW_4 14

int inputs[8] = { IN_ROW_1, IN_ROW_2, IN_ROW_3, IN_ROW_4, IN_2_ROW_1, IN_2_ROW_2, IN_2_ROW_3, IN_2_ROW_4 };
int outputs[6] = { OUT_COL_1, OUT_COL_2, OUT_COL_3, OUT_2_COL_1, OUT_2_COL_2, OUT_2_COL_3 };

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

volatile bool key2_1 = false, 
              key2_2 = false, 
              key2_3 = false,
              key2_4 = false, 
              key2_5 = false,
              key2_6 = false,
              key2_7 = false,
              key2_8 = false,
              key2_9 = false,
              key2_0 = false,
              key2_Hash = false,
              key2_Asterisk = false;

struct PortInit {
  PortInit() {        
    for(int i=0;i<6;++i)
    {
      pinMode(outputs[i], OUTPUT);
      digitalWrite(outputs[i], HIGH);
    }
    
    for(int i=0;i<8;++i)
      pinMode(inputs[i], INPUT);
  }
};

PortInit portInit;

MIDI_CREATE_DEFAULT_INSTANCE();

void handleNoteAction(byte note, bool pressed)
{
  if (note == 48) // C3
    key1 = pressed;
  else if (note == 49)
    key2 = pressed;
  else if (note == 50)
    key3 = pressed;
  else if (note == 51)
    key4 = pressed;
  else if (note == 52)
    key5 = pressed;
  else if (note == 53)
    key6 = pressed;
  else if (note == 54)
    key7 = pressed;
  else if (note == 55)
    key8 = pressed;
  else if (note == 56)
    key9 = pressed;
  else if (note == 57) 
    keyAsterisk = pressed;
  else if (note == 58)
    key0 = pressed;
  else if (note == 59)
    keyHash = pressed;
    
  if (note == 60) // C4
    key2_1 = pressed;
  else if (note == 61)
    key2_2 = pressed;
  else if (note == 62)
    key2_3 = pressed;
  else if (note == 63)
    key2_4 = pressed;
  else if (note == 64)
    key2_5 = pressed;
  else if (note == 65)
    key2_6 = pressed;
  else if (note == 66)
    key2_7 = pressed;
  else if (note == 67)
    key2_8 = pressed;
  else if (note == 68)
    key2_9 = pressed;
  else if (note == 69) 
    key2_Asterisk = pressed;
  else if (note == 70)
    key2_0 = pressed;
  else if (note == 71)
    key2_Hash = pressed;
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
  MIDI.read();
  
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

  bool row2_1 = digitalRead(IN_2_ROW_1);
  bool row2_2 = digitalRead(IN_2_ROW_2);
  bool row2_3 = digitalRead(IN_2_ROW_3);
  bool row2_4 = digitalRead(IN_2_ROW_4);

  if (!row2_1)
  {
    digitalWrite(OUT_2_COL_1, key2_Hash ? LOW : HIGH);
    digitalWrite(OUT_2_COL_2, key2_0 ? LOW : HIGH);
    digitalWrite(OUT_2_COL_3, key2_Asterisk ? LOW : HIGH);
  }
  else if(!row2_2)
  {
    digitalWrite(OUT_2_COL_1, key2_9 ? LOW : HIGH);
    digitalWrite(OUT_2_COL_2, key2_8 ? LOW : HIGH);
    digitalWrite(OUT_2_COL_3, key2_7 ? LOW : HIGH);
  }
  else if(!row2_3)
  {
    digitalWrite(OUT_2_COL_1, key2_6 ? LOW : HIGH);
    digitalWrite(OUT_2_COL_2, key2_5 ? LOW : HIGH);
    digitalWrite(OUT_2_COL_3, key2_4 ? LOW : HIGH);
  }
  else if(!row2_4)
  {
    digitalWrite(OUT_2_COL_1, key2_3 ? LOW : HIGH);
    digitalWrite(OUT_2_COL_2, key2_2 ? LOW : HIGH);
    digitalWrite(OUT_2_COL_3, key2_1 ? LOW : HIGH);
  }
}
