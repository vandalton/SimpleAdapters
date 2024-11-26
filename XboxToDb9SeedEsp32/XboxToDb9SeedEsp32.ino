#include <Bluepad32.h>

// Amiga joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - up, 2 - down, 3 - left, 4 - right, 5 - middle mouse button 
// 6 - fire/left mouse button 7 - 5V (HIGH), 8 - GND (LOW), 9 - right mouse button

#define OUT_UP D0 // connect digital pin 2 of Seeeduino to pin 1 of joystick port
#define OUT_DOWN D1
#define OUT_LEFT D2
#define OUT_RIGHT D3
#define OUT_FIRE D10
#define OUT_MOUSE_RIGHT D5
#define OUT_MOUSE_RIGHT_SECOND D6
#define OUT_MOUSE_MIDDLE D4

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

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }
}

void processGamepad(ControllerPtr ctl) {
  int dpad = ctl->dpad();
  
  up = ctl->a() || (ctl->dpad() & 1) || ctl->axisY() < -256;
  down = (ctl->dpad() & 2) || ctl->axisY() > 256;
  left = (ctl->dpad() & 8) || ctl->axisX() < -256;
  right = (ctl->dpad() & 4) || ctl->axisX() > 256;
  
  fire = ctl->x();
  mouse_right = ctl->b();
  mouse_middle = ctl->y();

  if (mouse_dx == 0 && mouse_dy == 0) {
    mouse_dx = (ctl->axisRX() > 256 ? 1 : 0) - (ctl->axisRX() < -256 ? 1 : 0);
    mouse_dy = (ctl->axisRY() > 256 ? 1 : 0) - (ctl->axisRY() < -256 ? 1 : 0);
  }
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData() && myController->isGamepad()) {
          processGamepad(myController);
        }
    }
}

void setup() {
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.enableVirtualDevice(false);
}

void loop() {
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    if (mouse_dx != 0 || mouse_dy != 0) {
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
  
    delay(10);
}
  
