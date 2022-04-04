#include <MouseController.h>
#include "KeyboardParser.h"
#include <PS4USB.h>

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

//BTD bt(&usb); 
//PS4BT ps4bt(&bt);
//PS4USB ps4usb(&usb);

KbdRptParser parser;

MouseController mouse(usb);

void mouseMoved() {
  mouse_dx += mouse.getXChange();
  mouse_dy += mouse.getYChange();
}

void mouseDragged() {
  mouse_dx += mouse.getXChange();
  mouse_dy += mouse.getYChange();
}

void mousePressed() {
  if (mouse.getButton(LEFT_BUTTON)) {
    parser.mouse_left = true;
  }
  if (mouse.getButton(MIDDLE_BUTTON)) {
    parser.mouse_middle = true;
  }
  if (mouse.getButton(RIGHT_BUTTON)) {
    parser.mouse_right = true;
  }
}

void mouseReleased() {
  if (!mouse.getButton(LEFT_BUTTON) && parser.mouse_left) {
    parser.mouse_left = false;
  }
  if (!mouse.getButton(MIDDLE_BUTTON) && parser.mouse_middle) {
    parser.mouse_middle = false;
  }
  if (!mouse.getButton(RIGHT_BUTTON) && parser.mouse_right) {
    parser.mouse_right = false;
  }
}

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
  
  /*if (ps4bt.connected()) {    
    up |= ps4bt.getButtonPress(UP) || ps4bt.getAnalogHat(LeftHatY) < 64;
    down |= ps4bt.getButtonPress(DOWN) || ps4bt.getAnalogHat(LeftHatY) > 192;
    left |= ps4bt.getButtonPress(LEFT) || ps4bt.getAnalogHat(LeftHatX) < 64;
    right |= ps4bt.getButtonPress(RIGHT) || ps4bt.getAnalogHat(LeftHatX) > 192;
    fire |= ps4bt.getButtonPress(CROSS);
    mouse_right |= ps4bt.getButtonPress(CIRCLE);
    mouse_middle |= ps4bt.getButtonPress(TRIANGLE);

    if(mouse_dx == 0 && mouse_dy == 0) {
      mouse_dx = (ps4bt.getAnalogHat(RightHatX) > 192 ? 1 : 0) - (ps4bt.getAnalogHat(RightHatX) < 64 ? 1 : 0);
      mouse_dy = (ps4bt.getAnalogHat(RightHatY) > 192 ? 1 : 0) - (ps4bt.getAnalogHat(RightHatY) < 64 ? 1 : 0);
    }
  }*/

 /*if (ps4usb.connected()) {    
    up |= ps4usb.getButtonPress(UP) || ps4usb.getAnalogHat(LeftHatY) < 64;
    down |= ps4usb.getButtonPress(DOWN) || ps4usb.getAnalogHat(LeftHatY) > 192;
    left |= ps4usb.getButtonPress(LEFT) || ps4usb.getAnalogHat(LeftHatX) < 64;
    right |= ps4usb.getButtonPress(RIGHT) || ps4usb.getAnalogHat(LeftHatX) > 192;
    fire |= ps4usb.getButtonPress(CROSS);
    mouse_right |= ps4usb.getButtonPress(CIRCLE);
    mouse_middle |= ps4usb.getButtonPress(TRIANGLE);

    if(mouse_dx == 0 && mouse_dy == 0) {
      mouse_dx = (ps4usb.getAnalogHat(RightHatX) > 192 ? 1 : 0) - (ps4usb.getAnalogHat(RightHatX) < 64 ? 1 : 0);
      mouse_dy = (ps4usb.getAnalogHat(RightHatY) > 192 ? 1 : 0) - (ps4usb.getAnalogHat(RightHatY) < 64 ? 1 : 0);
    }
  }*/
  
  if(mouse_dx != 0 || mouse_dy != 0) {
    int dir_x = mouse_dx > 0 ? 1 : -1;
    mouse_dx = abs(mouse_dx);
    
    int steps_x = 4*mouse_dx;
    
    int dir_y = mouse_dy > 0 ? 1 : -1;
    mouse_dy = abs(mouse_dy);
    
    int steps_y = 4*mouse_dy;

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
