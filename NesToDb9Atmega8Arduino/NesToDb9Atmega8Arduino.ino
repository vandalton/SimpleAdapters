 #include <avr/io.h>
#include <NESpad.h>
#include <EEPROM.h>

// Amiga joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - up, 2 - down, 3 - left, 4 - right, 5 - middle mouse button 6 - fire/left mouse button
// 7 - 5V (HIGH), 8 - GND (LOW), 9 - right mouse button

#define OUT_UP 7 
#define OUT_DOWN 5
#define OUT_LEFT 20
#define OUT_RIGHT 21
#define OUT_FIRE 4

#define STROBE 14
#define CLOCK 16
#define DATA 15

NESpad nes = NESpad(STROBE, CLOCK, DATA);

byte mode = 0;
byte selectAndStartCurrent = 0;
byte selectAndStartPrev = 0;

void setup() 
{
  pinMode(OUT_UP, OUTPUT);
  pinMode(OUT_DOWN, OUTPUT);
  pinMode(OUT_LEFT, OUTPUT);
  pinMode(OUT_RIGHT, OUTPUT);
  pinMode(OUT_FIRE, OUTPUT);

  mode = EEPROM.read(0) % 2;
}

void loop()
{  
  byte state = nes.buttons();

  digitalWrite(OUT_UP, (state & NES_UP) || (mode == 0 && (state & NES_A)) || (mode == 1 && (state & NES_B)) ? LOW : HIGH);
  digitalWrite(OUT_DOWN, (state & NES_DOWN) ? LOW : HIGH);
  digitalWrite(OUT_LEFT, (state & NES_LEFT) ? LOW : HIGH);
  digitalWrite(OUT_RIGHT, (state & NES_RIGHT) ? LOW : HIGH);
  digitalWrite(OUT_FIRE, (mode == 0 && (state & NES_B)) || (mode == 1 && (state & NES_A)) ? LOW : HIGH);

  selectAndStartCurrent = (state & (NES_SELECT | NES_START));
  if(selectAndStartPrev && !selectAndStartCurrent)
  {
    mode = (mode + 1) % 2;
    EEPROM.write(0, mode);
  }

  selectAndStartPrev = selectAndStartCurrent;

  delay(1);
}
