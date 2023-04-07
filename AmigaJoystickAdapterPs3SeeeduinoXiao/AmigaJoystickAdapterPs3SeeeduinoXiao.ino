#include "KeyboardParser.h"
#include <PS3USB.h>

// Amiga joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - up, 2 - down, 3 - left, 4 - right, 5 - middle mouse button 6 - fire/left mouse button
// 7 - 5V (HIGH), 8 - GND (LOW), 9 - right mouse button

#define OUT_UP 0 // connect digital pin 2 of Seeeduino to pin 1 of joystick port
#define OUT_DOWN 1
#define OUT_LEFT 2
#define OUT_RIGHT 3
#define OUT_FIRE 10
#define OUT_MOUSE_RIGHT 5
#define OUT_MOUSE_RIGHT_SECOND 6
#define OUT_MOUSE_MIDDLE 4

int outputs[8] = {OUT_UP, OUT_DOWN, OUT_LEFT, OUT_RIGHT, OUT_FIRE, OUT_MOUSE_RIGHT, OUT_MOUSE_RIGHT_SECOND, OUT_MOUSE_MIDDLE};

volatile bool up = false, 
              down = false, 
              left = false,
              right = false, 
              fire = false,
              mouse_right = false,
              mouse_middle = false;

volatile int  mouse_dx = 0,
              mouse_dy = 0,
              pos_x = 0,
              pos_y = 0;

int pattern_a[4] = { 1, 1, 0, 0 };
int pattern_b[4] = { 0, 1, 1, 0 };

struct PortInit {
  PortInit() {        
    for(int i=0;i<8;++i)
    {
      pinMode(outputs[i], OUTPUT);
      digitalWrite(outputs[i], HIGH);
    }
  }
};

PortInit portInit;

USBHost usb;

PS3USB ps3usb(&usb);

KbdRptParser parser;

void setup() {   
  if (usb.Init() == -1) {
    while (1);
  }

  delay(20);
}

void loop() {
  usb.Task();

  up = parser.up;
  down = parser.down;
  left = parser.left;
  right = parser.right;
  fire = parser.fire || parser.mouse_left;
  mouse_right = parser.mouse_right;
  mouse_middle = parser.mouse_middle;

  if (ps3usb.PS3Connected) {    
    up |= ps3usb.getButtonPress(UP) || ps3usb.getButtonPress(CROSS) || ps3usb.getAnalogHat(LeftHatY) < 64;
    down |= ps3usb.getButtonPress(DOWN) || ps3usb.getAnalogHat(LeftHatY) > 192;
    left |= ps3usb.getButtonPress(LEFT) || ps3usb.getAnalogHat(LeftHatX) < 64;
    right |= ps3usb.getButtonPress(RIGHT) || ps3usb.getAnalogHat(LeftHatX) > 192;
    fire |= ps3usb.getButtonPress(SQUARE);
    mouse_right |= ps3usb.getButtonPress(CIRCLE);
    mouse_middle |= ps3usb.getButtonPress(TRIANGLE);

    if(mouse_dx == 0 && mouse_dy == 0) {
      mouse_dx = (ps3usb.getAnalogHat(RightHatX) > 192 ? 1 : 0) - (ps3usb.getAnalogHat(RightHatX) < 64 ? 1 : 0);
      mouse_dy = (ps3usb.getAnalogHat(RightHatY) > 192 ? 1 : 0) - (ps3usb.getAnalogHat(RightHatY) < 64 ? 1 : 0);
    }
  }

  if(mouse_dx != 0 || mouse_dy != 0) {
    int dir_x = mouse_dx > 0 ? 1 : -1;
    int steps_x = 4*abs(mouse_dx);
    int dir_y = mouse_dy > 0 ? 1 : -1;
    int steps_y = 4*abs(mouse_dy);

    int steps = max(steps_x, steps_y);

    while(steps-- > 0) {
      if(steps_x-- > 0) {
        digitalWrite(OUT_DOWN, pattern_a[pos_x]);
        digitalWrite(OUT_RIGHT, pattern_b[pos_x]);
        pos_x = (pos_x + dir_x + 4) % 4;
      }
      
      if(steps_y-- > 0) {
        digitalWrite(OUT_UP, pattern_a[pos_y]);
        digitalWrite(OUT_LEFT, pattern_b[pos_y]);
        pos_y = (pos_y + dir_y + 4) % 4;
      }
      
      delayMicroseconds(50);
    }

    mouse_dx = 0;
    mouse_dy = 0;

    delay(10);
  }
  else {
    digitalWrite(OUT_UP, up ? LOW : HIGH);
    digitalWrite(OUT_DOWN, down ? LOW : HIGH);
    digitalWrite(OUT_LEFT, left ? LOW : HIGH);
    digitalWrite(OUT_RIGHT, right ? LOW : HIGH);
  }

  digitalWrite(OUT_FIRE, fire ? LOW : HIGH);
  digitalWrite(OUT_MOUSE_RIGHT, mouse_right ? LOW : HIGH);
  digitalWrite(OUT_MOUSE_RIGHT_SECOND, mouse_right ? LOW : HIGH);
  digitalWrite(OUT_MOUSE_MIDDLE, mouse_middle ? LOW : HIGH);
}
